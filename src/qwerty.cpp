#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mt;

void kek(std::vector<int> &lol) {
    for (int i = 0; i < lol.size(); i++) {
        std::cout << std::this_thread::get_id() << "\n";
        mt.lock();
        lol[i]++;
        mt.unlock();
    }
}

int main() {
    std::vector<int> arr(10000);
    auto start = std::chrono::high_resolution_clock::now();
    std::thread tr(kek, std::ref(arr));
    std::thread tr1(kek, std::ref(arr));
    tr.join();
    tr1.join();
    // kek(arr);
    // kek(arr);
    auto duration = std::chrono::high_resolution_clock::now() - start;
    std::cout << duration.count() << "\n";
}