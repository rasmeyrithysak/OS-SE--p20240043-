// Task 1A: Particle Pair Buffer - BEFORE semaphores (intentionally unsafe)
// No synchronization = race conditions, broken pairs, buffer violations
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>

const int BUFFER_CAPACITY = 100;  // 50 pairs

struct Particle {
    std::string id;
    bool empty = true;
};

Particle buffer[BUFFER_CAPACITY];
int head = 0;
int tail = 0;
int count = 0;

int produced_pairs = 0;
int packaged_pairs = 0;

bool running = true;

void producer(int machine_id) {
    int pair_id = 0;
    while (running) {
        pair_id++;
        std::string p1 = "M" + std::to_string(machine_id) + "-" + std::to_string(pair_id) + "-P1";
        std::string p2 = "M" + std::to_string(machine_id) + "-" + std::to_string(pair_id) + "-P2";

        if (count + 2 > BUFFER_CAPACITY) {
            std::cout << "\n[ERROR] The producing machine is broken\n";
            running = false;
            return;
        }

        buffer[tail % BUFFER_CAPACITY].id    = p1;
        buffer[tail % BUFFER_CAPACITY].empty = false;
        tail = (tail + 1) % BUFFER_CAPACITY;

        std::this_thread::sleep_for(std::chrono::microseconds(1));

        buffer[tail % BUFFER_CAPACITY].id    = p2;
        buffer[tail % BUFFER_CAPACITY].empty = false;
        tail = (tail + 1) % BUFFER_CAPACITY;

        count += 2;
        produced_pairs++;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void consumer() {
    while (running) {
        if (count < 2) {
            std::cout << "\n[ERROR] The packaging machine is broken\n";
            running = false;
            return;
        }

        std::string item1 = buffer[head % BUFFER_CAPACITY].id;
        buffer[head % BUFFER_CAPACITY].empty = true;
        head = (head + 1) % BUFFER_CAPACITY;

        std::string item2 = buffer[head % BUFFER_CAPACITY].id;
        buffer[head % BUFFER_CAPACITY].empty = true;
        head = (head + 1) % BUFFER_CAPACITY;

        count -= 2;

        std::string base1 = item1.substr(0, item1.rfind('-'));
        std::string base2 = item2.substr(0, item2.rfind('-'));

        if (base1 != base2) {
            std::cout << "\n[ERROR] Pairs are incorrect\n";
            std::cout << "  Got: " << item1 << " + " << item2 << "\n";
            running = false;
            return;
        }

        packaged_pairs++;
        std::cout << "Produced pairs: " << produced_pairs
                  << " | Packaged pairs: " << packaged_pairs
                  << " | Buffer particles: " << count << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

int main() {
    std::cout << "=== Task 1A: Particle Buffer WITHOUT Semaphores ===\n";
    std::cout << "Buffer capacity: " << BUFFER_CAPACITY << " particles (50 pairs)\n";
    std::cout << "Producers: 3  |  Consumer: 1\n";
    std::cout << "Expected: race conditions, broken pairs, or buffer violations\n\n";

    std::vector<std::thread> producers;
    for (int i = 1; i <= 3; i++)
        producers.emplace_back(producer, i);

    std::thread cons(consumer);

    for (auto& t : producers) t.join();
    cons.join();

    std::cout << "\n[STOPPED]\n";
    return 0;
}
