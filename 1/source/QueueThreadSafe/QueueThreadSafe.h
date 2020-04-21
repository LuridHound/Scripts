#ifndef RENAMEPACK_QUEUETHREADSAFE_H
#define RENAMEPACK_QUEUETHREADSAFE_H


#include <queue>
#include <condition_variable>
#include <mutex>


template<typename T, int Size = 10>
class QueueThreadSafe
{
    private :
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;

    public :
        QueueThreadSafe() = default;
        void push(T newValue);

        void notify();

        void wait_and_pop(T& value);
        bool empty() const;
};


#include "QueueThreadSafe.inl"


#endif //RENAMEPACK_QUEUETHREADSAFE_H
