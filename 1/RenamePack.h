#ifndef RENAMEPACK_RENAMEPACK_H
#define RENAMEPACK_RENAMEPACK_H


#include <functional>
#include <thread>
#include <filesystem>
#include <atomic>

#include "JoinThreads.h"
#include "QueueThreadSafe.h"


template<class T = std::function<void()>>
class RenamePack
{
    private :
        const bool multithreadedVersion;
        std::atomic_bool done;
        std::string packName;
        std::filesystem::path directory;
        std::vector<std::thread> extraThread;
        QueueThreadSafe<T> queue;
        JoinThreads joiner;

        void processFunction();

    public :

        explicit RenamePack(std::string_view packName, std::filesystem::path directory = std::filesystem::current_path());
        ~RenamePack();

        template<class FunctionType>
        void launch(FunctionType function);
};


#include "RenamePack.inl"


#endif //RENAMEPACK_RENAMEPACK_H
