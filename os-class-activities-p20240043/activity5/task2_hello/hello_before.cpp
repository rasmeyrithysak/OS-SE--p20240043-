// Task 2A: Print HELLO - BEFORE semaphores (wrong/unpredictable order)
#include <iostream>
#include <thread>
#include <chrono>

void process1() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    std::cout << "H";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "E";
    std::cout.flush();
}

void process2() {
    std::cout << "L";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "L";
    std::cout.flush();
}

void process3() {
    std::cout << "O";
    std::cout.flush();
}

int main() {
    std::cout << "=== Task 2A: HELLO WITHOUT Semaphores ===\n";
    std::cout << "Running 5 times to show unpredictable ordering:\n\n";

    for (int run = 1; run <= 5; run++) {
        std::cout << "Run " << run << ": ";
        std::cout.flush();

        std::thread t1(process1);
        std::thread t2(process2);
        std::thread t3(process3);

        t1.join();
        t2.join();
        t3.join();

        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\n[NOTE] Without semaphores, letters can appear in any order.\n";
    std::cout << "       P2 and P3 have no reason to wait for P1 to finish.\n";
    return 0;
}
