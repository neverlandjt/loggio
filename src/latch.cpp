//
// Created by ytymchenko on 07.11.2021.
//

#include "../inc/latch.h"


void latch::count_down() {
    {
        std::unique_lock<std::mutex> lck(lock);
        if (0 == count) return;
        if (--count == 0) cv.notify_all();
    }
}

void latch::wait() {
    {
        std::unique_lock<std::mutex> lck(lock);
        if (0 == count) return;
        else cv.wait(lck);
    }

}

