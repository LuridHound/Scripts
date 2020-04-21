#ifndef RENAMEPACK_RENAMEPACK_INL
#define RENAMEPACK_RENAMEPACK_INL


#include <cassert>


template<class T>
void RenamePack<T>::processFunction()
{
    assert(multithreadedVersion);
    while (!done)
    {
        T task;
        queue.wait_and_pop(task);
        task();
    }
}


template<class T>
RenamePack<T>::RenamePack(std::string_view packName, std::filesystem::path directory) :
    multithreadedVersion(std::thread::hardware_concurrency() != 0),
    done(false),
    packName(packName), directory(std::move(directory)),
    joiner(extraThread)
{
    if (multithreadedVersion)
    {
        extraThread.push_back(std::thread(&RenamePack::processFunction, this));
    }
}


template<class T>
RenamePack<T>::~RenamePack()
{
    if (multithreadedVersion)
    {
        queue.notify();
        while (!queue.empty())
        {
            std::this_thread::yield();
        }

        done = true;
    }

}


template<class T>
template<class FunctionType>
void RenamePack<T>::launch(FunctionType function)
{
    if (multithreadedVersion)
    {
        for (const auto& file : std::filesystem::recursive_directory_iterator{directory} )
        {
            if (is_regular_file(file.path()))
            {
                auto currentClosure = [function, packName = &(this->packName), file]()
                {
                    function(file.path(), *packName);
                };

                queue.push(currentClosure);
            }
        }
    }
    else
    {
        for (const auto& file : std::filesystem::recursive_directory_iterator{directory} )
        {
            if (is_regular_file(file.path()))
            {
                function(file.path(), packName);
            }
        }
    }

}


#endif //RENAMEPACK_RENAMEPACK_INL
