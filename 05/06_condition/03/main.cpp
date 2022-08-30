#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

int main() {
    std::condition_variable cv;
    std::mutex mtx;

    std::thread t1([&] {
        std::unique_lock lck(mtx);
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "t1 start to wait, time: " << now << ", own lock: " << lck.owns_lock() << std::endl;
        cv.wait(lck);
        std::cout << "t1 is awake" << ", own lock: " << lck.owns_lock() << std::endl;
    });

    std::thread t2([&] {
        std::unique_lock lck(mtx);
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "t2 start to wait, time: " << now << ", own lock: " << lck.owns_lock() << std::endl;
        cv.wait(lck);
        std::cout << "t2 is awake" << ", own lock: " << lck.owns_lock() << std::endl;
    });

    std::thread t3([&] {
        std::unique_lock lck(mtx);
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "t3 start to wait, time: " << now << ", own lock: " << lck.owns_lock() << std::endl;
        cv.wait(lck);
        std::cout << "t3 is awake" << ", own lock: " << lck.owns_lock() << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "notifying one, now: " << now << std::endl;
    std::cout << mtx.try_lock() << std::endl;
    cv.notify_one();  // awakening t1 only

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    std::cout << "notifying all" << std::endl;
    cv.notify_all();  // awakening t1 and t2
    mtx.unlock();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
