#ifndef RENAMEPACK_JOINTHREADS_H
#define RENAMEPACK_JOINTHREADS_H


#include <vector>
#include <thread>


class JoinThreads
{
    public :
        explicit JoinThreads(std::vector<std::thread>& threads);
        ~JoinThreads();

    private :
        std::vector<std::thread>& threads;
};


#endif //RENAMEPACK_JOINTHREADS_H
