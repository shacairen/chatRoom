#pragma once

#include <thread>  
#include <vector>  
#include <queue>  
#include <mutex>  
#include <condition_variable>  
#include <functional>  
#include <atomic>  


class ThreadPool {
public:
	ThreadPool(size_t ThreadNum = std::thread::hardware_concurrency())
		:_thread_nums(ThreadNum), _stop(false) {//hardware_concurrencyȡһ��CPU֧�ֲ�����
		for (size_t i = 0; i < _thread_nums; ++i) {
			_vt.push_back(std::thread(&ThreadPool::LoopWork, this));//����ThreadPool�ĳ�Ա����LoopWork��thisָ�롣
		}
	}
	ThreadPool(const ThreadPool &) = delete;
	ThreadPool & operator =(const ThreadPool) = delete;

private:
	void LoopWork() {
		std::unique_lock<std::mutex> ul(_mtx);
		while (true) {
			while (_taskQueue.empty() && !_stop) {
				_cond.wait(ul);
			}
			if (_taskQueue.empty()) {
				ul.unlock();
				break;
			}
			else {
				auto task = std::move(_taskQueue.front());
				_taskQueue.pop();
				ul.unlock();
				task();
				ul.lock();
			}
		}
	}
public:
	template<class F, class... Args>
	void PushFront(F &&task, Args&&... args) {
		auto func = std::bind(std::forward<F>(task), std::forward<Args>(args)...);//std::forward����ת����������ֵ���ԡ�
		{
			std::lock_guard<std::mutex> lg(_mtx);
			_taskQueue.push(std::forward<decltype(func)>(func));
		}
		_cond.notify_one();
	}

	~ThreadPool() {
		_stop = true;
		_cond.notify_all();
		for (size_t i = 0; i < _thread_nums; i++) {
			if (_vt[i].joinable())
				_vt[i].join();
		}
	}

private:
	size_t _thread_nums;
	std::vector<std::thread> _vt;
	std::queue<std::function<void()>> _taskQueue;
	std::mutex _mtx;
	std::condition_variable _cond;
	std::atomic<bool> _stop;
};