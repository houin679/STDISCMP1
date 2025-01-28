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
    std::vector<int> primes;
    for (int num = start; num <= end; num++) {
        if (is_prime(num)) {
            primes.push_back(num);
        }
    }
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << thread_id << " found primes: ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << "(Timestamp: " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << ")\n";
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








#include "B1.h"
#include <vector>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>

using namespace std;
/*
* This function searches for prime numbers using divisibility testing and prints them immediately.
* @param X the number of threads to be created
* @param y the range of numbers to be searched
* @param mtx the mutex to be used for synchronization
*/
void taskB1(int X, int y, mutex& mtx) {
    vector<thread> threads;
    queue<int> numbers;
    vector<int> primes;

    // Initialize the queue with numbers from 1 to y
    for (int num = 1; num <= y; ++num) {
        numbers.push(num);
    }
	// Worker function
    auto worker = [&numbers, &mtx, &primes]() {
        while (true) {
            int num;
            {  
                lock_guard<mutex> lock(mtx);
				// If the queue is empty, return
                if (numbers.empty()) {
                    return;
                }

				// Get the number from the queue and remove it from the queue
                num = numbers.front();
                numbers.pop();
            }

			// Check if the number is prime
            vector<future<bool>> futures;
			// Set limit as half of n + 1 since no number is divisible by a number greater than half of itself
            int limit = num / 2 + 1;

			// Check divisibility of num by numbers from 2 to limit
            for (int i = 2; i < limit; ++i) {
				// Check if num is divisible by i
                futures.push_back(async(launch::async, [i, num]() {
                    return num % i == 0;
                    }));
            }

            bool is_prime = true;
			// Check if any of the futures return true
            for (auto& f : futures) {
                if (f.get()) {
                    is_prime = false;
                    break;
                }
            }
            
			// If num is prime and greater than 1, add it to the list of primes
            if (is_prime && num > 1) {
                lock_guard<mutex> lock(mtx);
                primes.push_back(num);
                cout << num << " ";
            }
        }
        };

    // Print the initial statement
    cout << "Prime Numbers between 1 and " << y << ": ";

    // Create and start threads
    for (int i = 0; i < X; ++i) {
        threads.emplace_back(worker);
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    cout << endl;
}
