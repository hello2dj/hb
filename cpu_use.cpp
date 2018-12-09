#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <cstdint>

void* CpuUsWorker(void* arg)
{
    uint64_t i = 0;
    while (true)
    {
        i++;
    }
    return nullptr;
}

void CpuUs(int n)
{
    std::vector<pthread_t> pthreads(n);
    for (int i = 0; i < n; i++)
    {
        assert(pthread_create(&pthreads[i], nullptr, CpuUsWorker, nullptr) == 0);
    }

    for (const auto& tid : pthreads)
    {
        assert(pthread_join(tid, nullptr) == 0);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s threads\n", argv[0]);
        return -1;
    }
    CpuUs(std::stoi(argv[1]));
    return 0;
}

// centos 下编译 g++ -pthread -std=gnu++11 -o cpu cpu_us.cpp


