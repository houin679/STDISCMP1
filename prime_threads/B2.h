#ifndef B2_H
#define B2_H

#include <mutex>
#include <queue>
#include <string>
using namespace std;

void taskB2(int X, int y, mutex& mtx, queue<string>& results);

#endif // B2_H
