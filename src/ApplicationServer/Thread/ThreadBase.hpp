//
// Created by gaojian on 2019/11/30.
//

#ifndef QUICK_REPAIR_THREADBASE_HPP
#define QUICK_REPAIR_THREADBASE_HPP

#include <mutex>
#include <memory>
#include <functional>
#include <queue>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <future>

class RunnableWrapper {
public:
	template<typename Runnable, typename ... Args>
	explicit RunnableWrapper(Runnable &&runnable, Args ... args)
			: m_runnable(std::make_unique<ImplType<Runnable, Args...>>(std::forward<Runnable>(runnable), args...))
	{}
	RunnableWrapper(RunnableWrapper &&other) : m_runnable{std::move(other.m_runnable)} {}
	RunnableWrapper(RunnableWrapper &) = delete;
	RunnableWrapper() = default;

	void operator()()
	{
		m_runnable->call();
	}

	RunnableWrapper &operator=(RunnableWrapper &&runnableWrapper) noexcept
	{
		m_runnable = std::move(runnableWrapper.m_runnable);
		return *this;
	}
	RunnableWrapper &operator=(RunnableWrapper &) = delete;

public:
	struct ImplBase {
		virtual void call() = 0;
		virtual ~ImplBase() = default;
	};

	template<typename Runnable, typename ... Args>
	struct ImplType : ImplBase {
//		typedef typename std::result_of<Runnable(Args...)>::type resType;
		std::function<void()> r;
		Runnable runnableInstance;

		ImplType(Runnable &&runnable, Args ... args)
				: runnableInstance{std::move(runnable)}, r{std::bind(&ImplType<Runnable, Args...>::runRunnable, this, args...)}
		{
			;
//			call();
//			runnableInstance(args...);
//			auto l = std::bind(&ImplType<Runnable &&, Args...>::runRunnable, this, args...);
//			l();
		}

		inline void call() override { r(); }

		void runRunnable(Args ... args)
		{
			runnableInstance(args...);
		}
	};

	std::unique_ptr<ImplBase> m_runnable;
};

template<typename T>
class ThreadSafeQueue {
public:
	ThreadSafeQueue() = default;

	void push(T &&newValue)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		queue.push(std::move(newValue));
		conditionVariable.notify_one();
	}

	void waitPop(T &value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		conditionVariable.wait(lock, [this] { return !queue.empty(); });
		value = queue.front();
		queue.pop();
	}

	bool tryPop(T &value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (queue.empty())
			return false;
		value = std::move(queue.front());
		queue.pop();
		return true;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return queue.empty();
	}

private:
	mutable std::mutex m_mutex;
	std::condition_variable conditionVariable;
	std::queue<T> queue;
};

class WorkStealingQueue {
	typedef RunnableWrapper Data;
public:
	WorkStealingQueue() = default;

	void push(Data data)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_deque.push_back(std::move(data));
		m_conditionVariable.notify_one();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_deque.empty();
	}

	void waitPop(Data &data)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_conditionVariable.wait(lock, [this] { return !m_deque.empty(); });
		data = std::move(m_deque.front());
		m_deque.pop_front();
	}

	void waitSteal(Data &data)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_conditionVariable.wait(lock, [this] { return !m_deque.empty(); });
		data = std::move(m_deque.back());
		m_deque.pop_back();
	}

	bool tryPop(Data &data)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_deque.empty())
			return false;
		data = std::move(m_deque.front());
		m_deque.pop_front();
		return true;
	}

	bool trySteal(Data &data)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_deque.empty())
			return false;
		data = std::move(m_deque.back());
		m_deque.pop_back();
		return true;
	}

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_conditionVariable;
	std::deque<Data> m_deque;
};

class ThreadJoiner {
public:
	ThreadJoiner(std::vector<std::thread> &threads)
		: _threads{threads}
	{}

	~ThreadJoiner() {
		for (auto &t : _threads)
		{
			if (t.joinable())
				t.join();
		}
	}

private:
	std::vector<std::thread> &_threads;
};
#endif //QUICK_REPAIR_THREADBASE_HPP
