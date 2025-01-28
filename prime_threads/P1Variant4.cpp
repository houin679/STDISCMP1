#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <mutex>

std::mutex mtx;

bool is_prime(int n, int start_divisor, int end_divisor) {
    if (n <= 1) return false;
    for (int i = start_divisor; i <= end_divisor; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

void find_primes(int max_number, int thread_id, int num_threads, std::vector<int>& primes) {
    for (int num = 2; num <= max_number; num++) {
        int start_divisor = 2 + thread_id;
        int end_divisor = std::sqrt(num);
        if (is_prime(num, start_divisor, end_divisor)) {
            std::lock_guard<std::mutex> lock(mtx);
            primes.push_back(num);
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
    std::vector<int> primes;
    std::vector<std::vector<int>> thread_primes(num_threads);

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(find_primes, max_number, i, num_threads, std::ref(thread_primes[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (int i = 0; i < num_threads; i++) {
        for (int prime : thread_primes[i]) {
            primes.push_back(prime);
        }
    }

    std::cout << "All primes found: ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << "\n";

    auto end_time = std::chrono::system_clock::now();
    std::time_t end_time_t = std::chrono::system_clock::to_time_t(end_time);
    std::cout << "End Time: " << std::ctime(&end_time_t);

    return 0;
}
