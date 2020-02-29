#pragma once

#include <memory>


template<typename T>
struct LessPred;

template<typename T>
struct LessPred<std::unique_ptr<T>>
{
	using type = std::unique_ptr<T>;
public:
	bool operator()(const type& left, const type& right) const { return LessPred<T>()(*left, *right); }
	bool operator()(const type& left, const T* right) const { return LessPred<T>()(*left, *right); }
	bool operator()(const T* left, const type& right) const { return LessPred<T>()(*left, *right); }
};

template<typename T>
struct LessPred<std::shared_ptr<T>>
{
	using type = std::shared_ptr<T>;
public:
	bool operator()(const type& left, const type& right) const { return LessPred<T>()(*left, *right); }
	bool operator()(const type& left, const T* right) const { return LessPred<T>()(*left, *right); }
	bool operator()(const T* left, const type& right) const { return LessPred<T>()(*left, *right); }
};