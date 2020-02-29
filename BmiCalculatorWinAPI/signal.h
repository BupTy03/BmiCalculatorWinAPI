#pragma once

#include "slot.h"

#include <utility>
#include <algorithm>
#include <vector>
#include <mutex>
#include <shared_mutex>


template<class... Args>
class signal
{
public:
	void emit(Args&&... args) const
	{
		std::shared_lock<std::shared_mutex> lock_(mx_);
		for (std::size_t i = 0; i < slots_.size(); ++i) 
			slots_[i](std::forward<Args>(args)...);
	}

	void connect(void(*func)(Args...))
	{
		if (func == nullptr) return;
		connect(slot<Args...>(func));
	}

	template<class Obj, class BaseObj>
	void connect(Obj* obj, void (BaseObj::* func)(Args...))
	{
		if (obj == nullptr || func == nullptr) return;
		connect(slot<Args...>(obj, func));
	}

	void connect(const slot<Args...>& sl)
	{
		if (std::find(std::cbegin(slots_), std::cend(slots_), sl) != std::cend(slots_))
			return;

		std::unique_lock<std::shared_mutex> lock_(mx_);
		slots_.push_back(sl);
	}

	void connect(slot<Args...>&& sl)
	{
		if (std::find(std::cbegin(slots_), std::cend(slots_), sl) != std::cend(slots_))
			return;

		std::unique_lock<std::shared_mutex> lock_(mx_);
		slots_.push_back(std::move(sl));
	}

	void disconnect(void(*func)(Args...))
	{
		if (func == nullptr) return;
		disconnect(slot<Args...>(func));
	}

	template<class Obj, class... FArgs>
	void disconnect(Obj* obj, void (Obj::* func)(FArgs...))
	{
		disconnect(slot<FArgs...>(obj, func));
	}

	void disconnect(const slot<Args...>& sl)
	{
		std::shared_lock<std::shared_mutex> lock_(mx_);
		slots_.erase(std::find(std::begin(slots_), std::end(slots_), sl));
	}

private:
	mutable std::shared_mutex mx_;
	std::vector<slot<Args...>> slots_;
};

