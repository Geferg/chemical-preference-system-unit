#ifndef TCPDEBUGCLEAN_CLIENTQUEUE_H
#define TCPDEBUGCLEAN_CLIENTQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

namespace Server {

    template<typename T>

    class ClientQueue {
    private:
        std::queue<T> queue;
        mutable std::mutex mtx;
        std::condition_variable cv;

    public:
        void push(T value) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(value));
            cv.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ return !queue.empty(); });
            T value = std::move(queue.front());
            queue.pop();
            return value;
        }
    };
}

#endif
