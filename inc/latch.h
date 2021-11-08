//
// Created by ytymchenko on 07.11.2021.
//

#ifndef LOGGIO_LATCH_H
#define LOGGIO_LATCH_H

#include <inttypes.h>
#include <stdint.h>
#include <mutex>
#include <condition_variable>


class latch {
public:

    latch(uint32_t count) : count(count) {};

    void wait();

    void count_down();

    uint32_t get_count() {
        std::unique_lock<std::mutex> lck(lock);
        return count;
    }

private:
    std::condition_variable cv;
    std::mutex lock ;
    uint32_t count;


    latch() = delete;

    latch(const latch &other) = delete;

    latch &operator=(const latch &other) = delete;

};


#endif //LOGGIO_LATCH_H
