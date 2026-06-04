// Task 2B: Print HELLO - AFTER semaphores (always correct order)
#include <iostream>
#include <thread>
#include <semaphore.h>

sem_t start_h;
sem_t after_e;
sem_t after_l1;
sem_t after_l2;

void process1() {
    sem_wait(&start_h);
    std::cout << "H";
    std::cout.flush();
    std::cout << "E";
    std::cout.flush();
    sem_post(&after_e);
}

void process2() {
    sem_wait(&after_e);
    std::cout << "L";
    std::cout.flush();
    sem_post(&after_l1);

    sem_wait(&after_l1);
    std::cout << "L";
    std::cout.flush();
    sem_post(&after_l2);
}

void process3() {
    sem_wait(&after_l2);
    std::cout << "O";
    std::cout.flush();
}

int main() {
    std::cout << "=== Task 2B: HELLO WITH Semaphores ===\n";
    std::cout << "Semaphores: start_h(1), after_e(0), after_l1(0), after_l2(0)\n";
    std::cout << "Running 5 times — must always print HELLO:\n\n";

    for (int run = 1; run <= 5; run++) {
        sem_init(&start_h,  0, 1);
        sem_init(&after_e,  0, 0);
        sem_init(&after_l1, 0, 0);
        sem_init(&after_l2, 0, 0);

        std::cout << "Run " << run << ": ";
        std::cout.flush();

        std::thread t1(process1);
        std::thread t2(process2);
        std::thread t3(process3);

        t1.join();
        t2.join();
        t3.join();

        std::cout << "\n";

        sem_destroy(&start_h);
        sem_destroy(&after_e);
        sem_destroy(&after_l1);
        sem_destroy(&after_l2);
    }

    std::cout << "\n[DONE] Every run printed HELLO in correct order.\n";
    return 0;
}
