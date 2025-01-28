#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <mutex>

std::mutex mtx;

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i <= std::sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}

void find_primes(int start, int end, int thread_id) {
    for (int num = start; num <= end; num++) {
        if (is_prime(num)) {
            std::lock_guard<std::mutex> lock(mtx);
            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::system_clock::to_time_t(now);
            std::cout << "Thread " << thread_id << ": " << num << " is prime (Timestamp: " << std::ctime(&timestamp) << ")";
        }
    }
}

int main() {
    std::ifstream config_file("config.txt");
    int num_threads, max_number;
    std::string line;

    while (std::getline(config_file, line)) {
        if (line.find("num_threads=") != std::string::npos) {
            num_threads = std::stoi(line.substr(12));
        }
        else if (line.find("max_number=") != std::string::npos) {
            max_number = std::stoi(line.substr(11));
        }
    }

    auto start_time = std::chrono::system_clock::now();
    std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
    std::cout << "Start Time: " << std::ctime(&start_time_t);

    std::vector<std::thread> threads;
    int chunk_size = max_number / num_threads;

    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size + 1;
        int end = (i == num_threads - 1) ? max_number : (i + 1) * chunk_size;
        threads.emplace_back(find_primes, start, end, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = std::chrono::system_clock::now();
    std::time_t end_time_t = std::chrono::system_clock::to_time_t(end_time);
    std::cout << "End Time: " << std::ctime(&end_time_t);

    return 0;
}
