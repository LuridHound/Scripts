#ifndef SCRIPT_QUEUETHREADSAFE_INL
#define SCRIPT_QUEUETHREADSAFE_INL


template<typename T>
void QueueThreadSafe<T>::push(T new_value)
{
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
}


template<typename T>
bool QueueThreadSafe<T>::try_pop(T& value)
{
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty())
    {
        return false;
    }
    value = data_queue.front();
    data_queue.pop();
    return true;
}


template<typename T>
bool QueueThreadSafe<T>::empty() const
{
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
}


#endif //SCRIPT_QUEUETHREADSAFE_INL
