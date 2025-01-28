#ifndef PRIME_OPERATIONS_H
#define PRIME_OPERATIONS_H

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <queue>
#include <ctime>

using namespace std;

bool is_prime(int n);
void search_primes(int start, int end, int thread_id, mutex& mtx);
void search_primes_deferred(int start, int end, int thread_id, mutex& mtx, queue<string>& results);
void test_divisibility(int num, int thread_id, mutex& mtx);
void test_divisibility_deferred(int num, int thread_id, mutex& mtx, queue<string>& results);

#endif // PRIME_OPERATIONS_H
