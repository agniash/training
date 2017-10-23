#pragma once

#include <thread>

class TScopedThread {
public:
    explicit TScopedThread(std::thread&& thread)
        : Thread(std::move(thread))
    {
        if (!Thread.joinable()) {
            throw std::logic_error("thread is not joinable");
        }
    }
    TScopedThread(const TScopedThread&) = delete;
    TScopedThread& operator = (const TScopedThread&) = delete;

    ~TScopedThread() {
        Thread.join();
    }

private:
    std::thread Thread;
};
