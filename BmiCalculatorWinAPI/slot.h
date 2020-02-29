#pragma once

#include <utility>
#include <stdexcept>
#include <memory>
#include <cassert>
#include <type_traits>


template<class... Args>
class slot
{
	using TrivialFunc = void(*) (Args...);
	
	template<class Obj>
	using MemberFunc = void (Obj::*)(Args...);

public:
	template<class Obj, class BaseObj, typename = std::enable_if_t<std::is_base_of_v<BaseObj, Obj>>>
	explicit slot(Obj* owner, MemberFunc<BaseObj> func)
		: functor_{std::make_unique<member_function<BaseObj>>(static_cast<BaseObj*>(owner), func)}
	{
		if (owner == nullptr || func == nullptr)
			throw std::invalid_argument{ "pointer to an object and pointer to member of this object expected, nullptr was given" };
	}

	explicit slot(void(*func)(Args...))
		: functor_{ std::make_unique<free_function>(func) }
	{
		if (func == nullptr)
			throw std::invalid_argument{ "pointer to a trivial function expected, nullptr was given" };
	}

	inline bool operator == (const slot& other) const noexcept
	{
		return functor_->equals(*other.functor_);
	}
	inline bool operator != (const slot& other) const noexcept { return !(*this == other); }

	void operator () (Args&&... args) const
	{
		if (!functor_)
			throw std::runtime_error{ "uninitialized functor" };

		functor_->call(std::forward<Args>(args)...);
	}

private:

	struct callable
	{
		enum class Type { FreeFunction, MemberFunction };

		virtual void call(Args&&... args) const = 0;
		virtual bool equals(const callable& other) const = 0;
		virtual Type type() const = 0;
	};

	struct free_function : callable
	{
		explicit free_function(TrivialFunc func)
			: func_{ func } 
		{
			assert(func_ != nullptr);
		}

		void call(Args&&... args) const override
		{
			func_(std::forward<Args>(args)...);
		}

		callable::Type type() const override
		{
			return callable::Type::FreeFunction;
		}

		bool equals(const callable& other) const override
		{
			if (other.type() != type())
				return false;

			return dynamic_cast<const free_function&>(other).func_ == func_;
		}

	private:
		TrivialFunc func_ = nullptr;
	};

	template<class Obj>
	struct member_function : callable
	{
		explicit member_function(Obj* owner, void (Obj::* func)(Args...))
			: pObj_{ owner }, mfunc_{ func } 
		{
			assert(pObj_ != nullptr);
			assert(mfunc_ != nullptr);
		}

		void call(Args&&... args) const override
		{
			(pObj_->*mfunc_)(std::forward<Args>(args)...);
		}

		callable::Type type() const override
		{
			return callable::Type::MemberFunction;
		}

		bool equals(const callable& other) const override
		{
			if (other.type() != type())
				return false;

			const member_function& otherRef = dynamic_cast<const member_function&>(other);
			return otherRef.pObj_ == pObj_ &&
				otherRef.mfunc_ == mfunc_;
		}

	private:
		Obj* pObj_ = nullptr;
		void (Obj::* mfunc_)(Args...) = nullptr;
	};


	std::unique_ptr<const callable> functor_;
};
