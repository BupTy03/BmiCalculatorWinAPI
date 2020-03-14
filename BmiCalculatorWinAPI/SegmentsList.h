#pragma once

#include <vector>
#include <utility>
#include <stdexcept>


namespace SegmentsListImpl 
{

	template<typename Comp = std::less<>>
	struct CompareByFirst
	{
		template<typename T, typename K, typename V>
		bool operator()(const std::pair<K, V>& lhs, const T& rhs) const
		{
			return Comp()(lhs.first, rhs);
		}

		template<typename T, typename K, typename V>
		bool operator()(const T& lhs, const std::pair<K, V>& rhs) const
		{
			return Comp()(lhs, rhs.first);
		}

		template<typename K1, typename V1, typename K2, typename V2>
		bool operator()(const std::pair<K1, V1>& lhs, const std::pair<K2, V2>& rhs) const
		{
			return Comp()(lhs.first, rhs.first);
		}

		template<typename T>
		bool operator()(const T& lhs, const T& rhs) const
		{
			return Comp()(lhs, rhs);
		}
	};

}


template<typename Key, typename Value, typename Comp = std::less<>>
class SegmentsList
{
public:
	SegmentsList() = default;

	template<typename K, typename V>
	bool add(K&& key, V&& value)
	{
		SegmentsListImpl::CompareByFirst<Comp> comp;
		const auto it = std::lower_bound(std::cbegin(elems_), std::cend(elems_), key, comp);
		if (it != std::cend(elems_) && !comp(key, *it))
			return false;

		elems_.emplace(it, std::forward<K>(key), std::forward<V>(value));
		return true;
	}

	const Value& operator[](const Key& key) const
	{
		SegmentsListImpl::CompareByFirst<Comp> comp;
		if (std::empty(elems_) || comp(key, elems_.front()))
			throw std::out_of_range("Index is out of range!");

		const auto it = std::lower_bound(std::begin(elems_), std::end(elems_), 
			key, comp);

		if (it != std::end(elems_) && !comp(key, *it))
			return it->second;
		
		return std::prev(it)->second;
	}

	Value& operator[](const Key& key)
	{
		return const_cast<Value&>(const_cast<const SegmentsList*>(this)->operator[](key));
	}

private:
	std::vector<std::pair<Key, Value>> elems_;
};