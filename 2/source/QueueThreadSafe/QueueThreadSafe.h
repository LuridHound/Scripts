#ifndef SCRIPT_QUEUETHREADSAFE_H
#define SCRIPT_QUEUETHREADSAFE_H


#include <mutex>
#include <queue>
#include <condition_variable>


template<typename T>
class QueueThreadSafe
{
    private :
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;

    public :

        void push(T new_value);
        bool try_pop(T& value);

        bool empty() const;
};


#include "QueueThreadSafe.inl"



#endif //SCRIPT_QUEUETHREADSAFE_H
