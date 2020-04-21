#ifndef SCRIPT_THREADPOOL_H
#define SCRIPT_THREADPOOL_H


#include <filesystem>
#include <thread>
#include <atomic>
#include <vector>
#include <map>

#include "../QueueThreadSafe/QueueThreadSafe.h"
#include "../JoinThreads/JoinThreads.h"
#include "../AnalyzeFunctions/AnalyzeFunctions.h"

class ThreadPool
{
    private :

        std::atomic_bool done;
        std::atomic_int countThreads;
        QueueThreadSafe<std::pair<std::filesystem::path, std::string>> queue;
        std::vector<std::thread> threads;
        JoinThreads joiner;

        std::mutex mut;

        //  This data is shared between threads, that's why we need mutex.
        std::map<std::string, int> infoByLanguage = {};                  // Stores an information about how many lines each language have.

        void work();

    public :

        ThreadPool();

        bool isFinished();
        bool allFinished();

        void stop();

        [[nodiscard]]
        const std::map<std::string, int>& getInfoByLanguage() const;

        //  By reference, because is initialized in the main thread. And when you push, create a copy in the queue.
        void submitTask(const std::filesystem::path& pathToFile, const std::string& language);

};


#endif //SCRIPT_THREADPOOL_H
