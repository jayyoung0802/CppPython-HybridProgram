#include <iostream>
#include <string>
#include "Timer.h"
#include <thread>

long long sum_range()
{
    long long i;
    long long s = 0LL;
    long long high = 1e7;

    for (i = 0LL; i < high; i++)
        s += (long long)i;
    return s;
}

void func()
{
    while (true)
    {
        /* code */
    }
}

// 创建线程
int main(int argc, char *argv[])
{

    Timer timer;
    timer.start();

    std::thread threads[6];
    std::cout << "Spawning 5 threads...\n";
    for (int i = 0; i < 6; i++)
    {
        threads[i] = std::thread(func);
    }
    std::cout << "Done spawning threads! Now wait for them to join\n";
    for (auto &t : threads)
    {
        t.join();
    }
    std::cout << "All threads joined.\n";
    timer.stop();
    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    return 0;
}