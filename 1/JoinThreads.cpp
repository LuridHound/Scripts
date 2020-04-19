#include "JoinThreads.h"


JoinThreads::JoinThreads(std::vector<std::thread>& threads) :
    threads(threads)
{

}


JoinThreads::~JoinThreads()
{
    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}