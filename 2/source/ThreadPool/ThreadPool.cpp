#include "ThreadPool.h"


void ThreadPool::work()
{
    // Reserve memory for the string inside this function.
    (void)AnalyzeFunctions::getLinesInFile("");

    while (!done)
    {
        std::pair<std::filesystem::path, std::string> info;

        if (queue.try_pop(info))
        {
            int currentLines = AnalyzeFunctions::getLinesInFile( info.first.c_str());

            std::lock_guard<std::mutex> lock(mut);

            infoByLanguage[info.second] += currentLines;
        }
        else
        {
            std::this_thread::yield();
        }
    }
    countThreads.fetch_sub(1);
}

ThreadPool::ThreadPool(): done(false), countThreads(std::thread::hardware_concurrency() - 1), joiner(threads)
{
    try
    {
        threads.reserve(countThreads);
        for (unsigned int i = 0; i < countThreads; ++i)
        {
            threads.push_back(std::thread(&ThreadPool::work, this));
        }
    }
    catch (...)
    {
        done = true;
        throw;
    }

}


bool ThreadPool::isFinished()
{
    return queue.empty();
}


bool ThreadPool::allFinished()
{
    return countThreads.load() == 0;
}


void ThreadPool::stop()
{
    done = true;
}


[[nodiscard]]
const std::map<std::string, int>& ThreadPool::getInfoByLanguage() const
{
    return infoByLanguage;
}


//  By reference, because is initialiazed in the main thread. And when try to push,
void ThreadPool::submitTask(const std::filesystem::path& pathToFile, const std::string& language)
{
    queue.push({pathToFile, language});
}
