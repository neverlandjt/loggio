//
// Created by ytymchenko on 28.11.2021.
//

#ifndef LOGGIO_CONCURRENT_QUEUE_H
#define LOGGIO_CONCURRENT_QUEUE_H


#include <deque>
#include <mutex>
#include <condition_variable>


template<typename T>
class concurrent_queue {
private:
    std::deque<T> q;
    std::mutex m;
    std::condition_variable cv;
public:

    concurrent_queue() = default;

    void push_back(T item) {
        {
            std::lock_guard<std::mutex> lck(m);
            q.push_back(std::move(item));
        }
        cv.notify_one();
    }

    T pop_front() {
        std::unique_lock<std::mutex> lck(m);
        cv.wait(lck, [this] { return !q.empty(); });
        T rv = std::move(q.front());
        q.pop_front();
        return std::move(rv);
    }

    size_t size() {
        std::lock_guard<std::mutex> lck(m);
        return q.size();
    }


};


#endif //LOGGIO_CONCURRENT_QUEUE_H
