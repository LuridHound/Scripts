#ifndef RENAMEPACK_QUEUETHREADSAFE_INL
#define RENAMEPACK_QUEUETHREADSAFE_INL


template<class T, int Size>
void QueueThreadSafe<T, Size>::push(T newValue)
{
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(newValue);
    if (data_queue.size() > Size)
    {
        data_cond.notify_one();
    }
}


template<class T, int Size>
void QueueThreadSafe<T, Size>::notify()
{
    data_cond.notify_one();
}


template<class T, int Size>
void QueueThreadSafe<T, Size>::wait_and_pop(T& value)
{
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this]{return !data_queue.empty();});
    value = data_queue.front();
    data_queue.pop();
}


template<class T, int Size>
bool QueueThreadSafe<T, Size>::empty() const
{
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
}


#endif //RENAMEPACK_QUEUETHREADSAFE_INL
