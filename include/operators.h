#ifndef OPERATORS_HPP
#define OPERATORS_HPP

// less than.
// comparable same type
#define OPERATOR_LESS_THAN1(template_name) \
	less_than_comp1<template_name >

// less than.  
// comparable diff type
#define OPERATOR_LESS_THAN2(t1, t2) \
	less_than_comp2<t1, t2 >

// equality.
// comparable same type
#define OPERATOR_EQUALITY1(template_name) \
	equality_comp1<template_name >

// equality.
// comparable diff type
#define OPERATOR_EQUALITY2(t1, t2) \
	equality_comp2<t1, t2 >

template <class T>
struct less_than_comp1
{
	friend bool operator >(const T& x, const T& y)
	{
		return static_cast<bool>(y < x);
	}

	friend bool operator <=(const T& x, const T& y)
	{
		return !static_cast<bool>(y < x);
	}

	friend bool operator >=(const T& x, const T& y)
	{
		return !static_cast<bool>(x < y);
	}
};

template <class T, class U = T>
struct less_than_comp2
{
	friend bool operator >=(const T& x, const U& y)
	{
		return !static_cast<bool>(x < y);
	}

	friend bool operator <=(const T& x, const U& y)
	{
		return !static_cast<bool>(x > y);
	}

	friend bool operator >(const U& x, const T& y)
	{
		return static_cast<bool>(y < x);
	}

	friend bool operator <(const U& x, const T& y)
	{
		return static_cast<bool>(y > x);
	}

	friend bool operator >=(const U& x, const T& y)
	{
		return !static_cast<bool>(y > x);
	}

	friend bool operator <=(const U& x, const T& y)
	{
		return !static_cast<bool>(y < x);
	}
};

template <class T>
struct equality_comp1
{
	friend bool operator !=(const T& x, const T& y)
	{
		return !static_cast<bool>(x == y);
	}
};

template <class T, class U = T>
struct equality_comp2
{
	friend bool operator !=(const U& x, const T& y)
	{
		return !static_cast<bool>(y == x);
	}

	friend bool operator ==(const U& x, const T& y)
	{
		return static_cast<bool>(y == x);
	}

	friend bool operator !=(const T& x, const U& y)
	{
		return !static_cast<bool>(x == y);
	}
};

#endif // OPERATORS_HPP