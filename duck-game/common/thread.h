#ifndef THREAD_H
#define THREAD_H

#include <exception>
#include <iostream>
#include <thread>
#include <vector>

// Implementacion de thread extraido de hands on thread
class Thread {
private:
    std::thread thread;
    void main();

public:
    Thread();

    void start();

    void join();

    virtual void run() = 0;

    virtual ~Thread() {}

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) = delete;
    Thread& operator=(Thread&& other) = delete;
};

#endif  // THREAD_H
