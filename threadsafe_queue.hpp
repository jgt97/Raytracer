#ifndef THREADSAFE_QUEUE
#define THREADSAFE_QUEUE

#include <queue>
#include <condition_variable>
#include <mutex>

template<typename T>
class ThreadSafeQueue
{
public:
	void push(const T& value);
	bool empty() const;
	bool try_pop(T& popped_value);
	void wait_and_pop(T& popped_value);

private:
	std::queue<T> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

#include "threadsafe_queue.txx"

#endif
