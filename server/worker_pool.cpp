#include "worker_pool.h"

WorkerPool::WorkerPool(int nThreads) {
    for (int i = 0; i < nThreads; i++)
        workers.emplace_back(&WorkerPool::run, this);
}

WorkerPool::~WorkerPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto &w : workers) w.join();
}

void WorkerPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(task);
    }
    cv.notify_one();
}

void WorkerPool::run() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) return;

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}
