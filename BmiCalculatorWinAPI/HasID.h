#pragma once

#include "predicates.h"

#include <cstddef>
#include <type_traits>


template<class T>
class HasID
{
public:
	HasID() : id_{ counter_ } { ++counter_; }
	std::size_t getID() const noexcept { return id_; }

private:
	static std::size_t counter_;
	const std::size_t id_;
};

template<class T>
std::size_t HasID<T>::counter_ = 0;


template<class T>
struct LessByIDPred;


template<class T>
struct LessByIDPred<HasID<T>>
{
	bool operator()(const HasID<T>& left, const HasID<T>& right) const { return left.getID() < right.getID(); }
	bool operator()(const std::size_t left, const HasID<T>& right) const { return left < right.getID(); }
	bool operator()(const HasID<T>&  left, const std::size_t right) const { return left.getID() < right; }
};

template<typename T>
struct LessByIDPred<T*>
{
	using type = T*;
public:
	bool operator()(const T* left, const T* right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
	bool operator()(const T& left, const T* right) const { return LessByIDPred<HasID<T>>()(left, *right); }
	bool operator()(const T* left, const T& right) const { return LessByIDPred<HasID<T>>()(*left, right); }
};


template<typename T>
struct LessByIDPred<std::unique_ptr<T>>
{
	using type = std::unique_ptr<T>;
public:
	bool operator()(const type& left, const type& right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
	bool operator()(const type& left, const T* right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
	bool operator()(const T* left, const type& right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
};

template<typename T>
struct LessByIDPred<std::shared_ptr<T>>
{
	using type = std::shared_ptr<T>;
public:
	bool operator()(const type& left, const type& right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
	bool operator()(const type& left, const T* right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
	bool operator()(const T* left, const type& right) const { return LessByIDPred<HasID<T>>()(*left, *right); }
};