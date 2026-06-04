// Task 1B: Particle Pair Buffer - AFTER semaphores (correct)
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <semaphore.h>
#include <chrono>
#include <atomic>
#include <csignal>

const int BUFFER_CAPACITY = 100;

struct Particle {
    std::string id;
    bool empty = true;
};

Particle buffer[BUFFER_CAPACITY];
int head = 0;
int tail = 0;

sem_t empty_pairs;
sem_t full_pairs;
sem_t mutex;

std::atomic<int> produced_pairs{0};
std::atomic<int> packaged_pairs{0};
std::atomic<bool> running{true};

void handle_signal(int) { running = false; }

void producer(int machine_id) {
    int pair_id = 0;
    while (running) {
        pair_id++;
        std::string p1 = "M" + std::to_string(machine_id) + "-" + std::to_string(pair_id) + "-P1";
        std::string p2 = "M" + std::to_string(machine_id) + "-" + std::to_string(pair_id) + "-P2";

        sem_wait(&empty_pairs);
        sem_wait(&mutex);

        buffer[tail].id    = p1;
        buffer[tail].empty = false;
        tail = (tail + 1) % BUFFER_CAPACITY;

        buffer[tail].id    = p2;
        buffer[tail].empty = false;
        tail = (tail + 1) % BUFFER_CAPACITY;

        produced_pairs++;

        sem_post(&mutex);
        sem_post(&full_pairs);

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void consumer() {
    while (running) {
        sem_wait(&full_pairs);
        sem_wait(&mutex);

        if (buffer[head].empty || buffer[(head + 1) % BUFFER_CAPACITY].empty) {
            std::cout << "\n[ERROR] The packaging machine is broken\n";
            running = false;
            sem_post(&mutex);
            return;
        }

        std::string item1 = buffer[head].id;
        buffer[head].empty = true;
        head = (head + 1) % BUFFER_CAPACITY;

        std::string item2 = buffer[head].id;
        buffer[head].empty = true;
        head = (head + 1) % BUFFER_CAPACITY;

        std::string base1 = item1.substr(0, item1.rfind('-'));
        std::string base2 = item2.substr(0, item2.rfind('-'));

        if (base1 != base2) {
            std::cout << "\n[ERROR] Pairs are incorrect\n";
            std::cout << "  Got: " << item1 << " + " << item2 << "\n";
            running = false;
            sem_post(&mutex);
            return;
        }

        packaged_pairs++;

        int buf_count = 0;
        for (int i = 0; i < BUFFER_CAPACITY; i++)
            if (!buffer[i].empty) buf_count++;

        std::cout << "Produced pairs: " << produced_pairs
                  << " | Packaged pairs: " << packaged_pairs
                  << " | Buffer particles: " << buf_count
                  << "  [OK: " << item1 << " + " << item2 << "]\n";

        sem_post(&mutex);
        sem_post(&empty_pairs);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    signal(SIGINT, handle_signal);

    sem_init(&empty_pairs, 0, 50);
    sem_init(&full_pairs,  0,  0);
    sem_init(&mutex,       0,  1);

    std::cout << "=== Task 1B: Particle Buffer WITH Semaphores ===\n";
    std::cout << "Buffer capacity: " << BUFFER_CAPACITY << " particles (50 pairs)\n";
    std::cout << "Producers: 3  |  Consumer: 1\n";
    std::cout << "Semaphores: empty_pairs(50), full_pairs(0), mutex(1)\n";
    std::cout << "Press Ctrl+C to stop.\n\n";

    std::vector<std::thread> producers;
    for (int i = 1; i <= 3; i++)
        producers.emplace_back(producer, i);

    std::thread cons(consumer);

    for (auto& t : producers) t.join();
    cons.join();

    sem_destroy(&empty_pairs);
    sem_destroy(&full_pairs);
    sem_destroy(&mutex);

    std::cout << "\n[STOPPED] Final — Produced: " << produced_pairs
              << " | Packaged: " << packaged_pairs << "\n";
    return 0;
}
