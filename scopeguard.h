#ifndef SCOPEGUARD_H
#define SCOPEGUARD_H

class ScopeGuardBase;
typedef const ScopeGuardBase& ScopeGuard;

// disallow operator =
#undef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(Type) \
	Type& operator =(const Type&);

class ScopeGuardBase
{
	DISALLOW_ASSIGN(ScopeGuardBase)
public:
	// default ctor
	ScopeGuardBase() throw()
		:m_bDismiss(false)
	{

	}

protected:
	// copy ctor
	// copy ctor will dismiss that`s guard
	ScopeGuardBase(const ScopeGuardBase& that) throw()
		:m_bDismiss(that.m_bDismiss)
	{
		that.Dismiss();
	}

	~ScopeGuardBase()
	{

	}

	template <class T>
	static void SafeExecute(T& t) throw()
	{
		if (!t.m_bDismiss)
		{
			try
			{
				t.Execute();
			}
			catch(...)
			{

			}
		}
	}

public:
	void Dismiss() const throw()
	{
		m_bDismiss = true;
	}

protected:
	mutable bool m_bDismiss;
};

// ScopeGuard0
// execute a function, or a functor
template <class Func>
class ScopeGuard0 : public ScopeGuardBase
{
	typedef ScopeGuard0<Func> ThisType;

public:
	static ThisType MakeGuard(Func func)
	{
		return ThisType(func);
	}

	~ScopeGuard0() throw()
	{
		SafeExecute(*this);
	}

	void Execute()
	{
		m_func();
	}

protected:
	ScopeGuard0(Func func)
		:m_func(func)
	{

	}

protected:
	Func m_func;

};

template <class Func>
inline ScopeGuard0<Func> MakeGuard(Func func)
{
	return ScopeGuard0<Func>::MakeGuard(func);
}

// ScopeGuard1
// execute a function or a functor, with 1 param
template <class Func, class P1>
class ScopeGuard1 : public ScopeGuardBase
{
	typedef ScopeGuard1<Func, P1> ThisType;
public:
	static ThisType MakeGuard(Func func, P1 p1)
	{
		return ThisType(func, p1);
	}

	~ScopeGuard1() throw()
	{
		SafeExecute(*this);
	}

	void Execute()
	{
		m_func(m_p1);
	}

protected:
	ScopeGuard1(Func func, P1 p1)
		:m_func(func)
		,m_p1(p1)
	{

	}

protected:
	Func m_func;
	const P1 m_p1;

};

template <class Func, class P1>
inline ScopeGuard1<Func, P1> MakeGuard(Func func, P1 p1)
{
	return ScopeGuard1<Func, P1>::MakeGuard(func, p1);
}

// ScopeGuard2
// execute a function or a functor, with 2 param
template <class Func, class P1, class P2>
class ScopeGuard2 : public ScopeGuardBase
{
	typedef ScopeGuard2<Func, P1, P2> ThisType;

public:
	static ThisType MakeGuard(Func func, P1 p1, P2 p2)
	{
		return ThisType(func, p1, p2);
	}

	~ScopeGuard2() throw()
	{
		SafeExecute(*this);
	}

	void Execute()
	{
		m_func(m_p1, m_p2);
	}

protected:
	ScopeGuard2(Func func, P1 p1, P2 p2)
		:m_func(func)
		,m_p1(p1)
		,m_p2(p2)
	{

	}

protected:
	Func m_func;
	const P1 m_p1;
	const P2 m_p2;

};

template <class Func, class P1, class P2>
inline ScopeGuard2<Func, P1, P2> MakeGuard(Func func, P1 p1, P2 p2)
{
	return ScopeGuard2<Func, P1, P2>::MakeGuard(func, p1, p2);
}


// ScopeGuard3
// execute a function or a functor, with 3 param
template <class Func, class P1, class P2, class P3>
class ScopeGuard3 : public ScopeGuardBase
{
	typedef ScopeGuard3<Func, P1, P2, P3> ThisType;

public:
	static ThisType MakeGuard(Func func, P1 p1, P2 p2, P3 p3)
	{
		return ThisType(func, p1, p2, p3);
	}

	~ScopeGuard3() throw()
	{
		SafeExecute(*this);
	}

	void Execute()
	{
		m_func(m_p1, m_p2, m_p3);
	}

protected:
	ScopeGuard3(Func func, P1 p1, P2 p2, P3 p3)
		:m_func(func)
		,m_p1(p1)
		,m_p2(p2)
		,m_p3(p3)
	{

	}

protected:
	Func m_func;
	const P1 m_p1;
	const P2 m_p2;
	const P3 m_p3;
};

template <class Func, class P1, class P2, class P3>
inline ScopeGuard3<Func, P1, P2, P3> MakeGuard(Func func, P1 p1, P2 p2, P3 p3)
{
	return ScopeGuard3<Func, P1, P2, P3>::MakeGuard(func, p1, p2, p3);
}

// RefHolder
// some function or functor need a reference param
// reference param wraper
template <class T>
class RefHolder
{
public:
	RefHolder(T& ref)
		:m_ref(ref)
	{

	}

	operator T& () const
	{
		return m_ref;
	}

private:
	T& m_ref;
};

template <class T>
inline RefHolder<T> ByRef(T& ref)
{
	return RefHolder<T>(ref);
}

// ScopeGuardObj0
// execute a obj`s member function with no param
template <class Obj, class MembFunc>
class ScopeGuardObj0 : public ScopeGuardBase
{
	typedef ScopeGuardObj0<Obj, MembFunc> ThisType;
public:
	static ThisType MakeObjGuard(Obj& obj, MembFunc membFunc)
	{
		return ThisType(obj, membFunc);
	}

	~ScopeGuardObj0() throw()
	{
		SafeExecute(*this);
	}

	void Execute() 
	{
		(m_obj.*m_membFunc)();
	}

protected:
	ScopeGuardObj0(Obj& obj, MembFunc func)
		:m_obj(obj)
		,m_membFunc(func)
	{

	}

protected:
	Obj& m_obj;
	MembFunc m_membFunc;
};

template <class Obj, class MembFunc>
inline ScopeGuardObj0<Obj, MembFunc> MakeObjGuard(Obj& obj, MembFunc membFunc)
{
	return ScopeGuardObj0<Obj, MembFunc>::MakeObjGuard(obj, membFunc);
}

#undef DISALLOW_ASSIGN

#endif // SCOPEGUARD_H