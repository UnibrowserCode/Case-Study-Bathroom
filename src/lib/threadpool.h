#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false), tasksInProgress(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    { // acquire lock
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                        ++tasksInProgress;
                    }
                    task(); // execute task
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        --tasksInProgress;
                        finishedCv.notify_all();
                    }
                }
            });
        }
    }

    // Add a new task to the queue
    template <class F>
    void enqueue(F&& f) {
        { 
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::function<void()>(f));
        }
        cv.notify_one();
    }

    // Wait until all tasks have finished
    void wait() {
        std::unique_lock<std::mutex> lock(queueMutex);
        finishedCv.wait(lock, [this] { return tasks.empty() && tasksInProgress == 0; });
    }

    ~ThreadPool() {
        { 
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable cv;

    std::condition_variable finishedCv;
    std::atomic<int> tasksInProgress;
    bool stop;
};
