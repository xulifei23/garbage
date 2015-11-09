#ifndef XSMARTPTR_H
#define XSMARTPTR_H

#include "stddef.h"
#include "assert.h"
#include "typeinfo.h"
#include "winbase.h"

#undef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
		ClassName(const ClassName&); \
		ClassName& operator =(const ClassName&)

#undef FREE_POINTER
#define FREE_POINTER(p) \
		if (p) { delete p; p = NULL; }

#undef FREE_ARRAY
#define FREE_ARRAY(p) \
		if (p) { delete [] p; p = NULL; }

namespace XSMARTPTR
{
	// XScopePtr
	template <class T>
	class XScopePtr    //noncopyable
	{
	public:
		explicit XScopePtr(T* pObj = NULL)   
			:m_pObj(pObj)
		{

		}

		~XScopePtr()
		{
			if (m_pObj != NULL)
			{
				XGLOBALFUNC::check_delete(m_pObj);
			}
		}

	public:
		// 重新绑定一个裸指针，之前托管的裸指针会被释放
		void Reset(T* pObj)    
		{
			assert(pObj == NULL || pObj != m_pObj);
			ThisType(pObj).Swap(*this);
		}

		// 交换两个XScopePtr对象托管的指针
		void Swap(XScopePtr& that)    
		{
			T* pTemp = that.m_pObj;
			that.m_pObj = this->m_pObj;
			this->m_pObj = pTemp;
		}

		// 支持解引用
		T& operator *()    
		{
			assert(m_pObj != NULL);
			return *m_pObj;
		}

		// 使之看起来和裸指针行为差不多。
		T* operator ->()    
		{
			assert(m_pObj != NULL);
			return m_pObj;
		}

		// 获得裸指针。用户如果释放裸指针，会导致DoubleFree。
		T* Get() const                 
		{
			return m_pObj;
		}
		
		// 支持向bool类型隐式转换。所以：XScopePtr a(new T); if (a){...} 是合法的。
		operator bool()    
		{
			return m_pObj != NULL;
		}

	private:
		typedef XScopePtr<T> ThisType;
		DISALLOW_COPY_AND_ASSIGN(ThisType);

	private:
		T* m_pObj;
	};
	// end XScopePtr

	// XScopeArray
	template <class T>
	class XScopeArray      //noncopyable
	{
	public:
		explicit XScopeArray(T* pObj = NULL)
			:m_pArray(pObj)
		{

		}

		~XScopeArray()
		{
			if (m_pArray != NULL)
			{
				XGLOBALFUNC::check_array_delete(m_pArray);
			}
		}

	public:
		// 获得裸指针
		T* Get() const    
		{
			return m_pArray;
		}

		// 交换托管的裸指针
		void Swap(XScopeArray& that)    
		{
			T* pTemp = that.m_pArray;
			that.m_pArray = this->m_pArray;
			this->m_pArray = pTemp;
		}

		// 重新绑定裸指针，之前的会被销毁
		void Reset(T* pArray)    
		{
			assert(pArray == NULL || pArray != m_pArray);
			ThisType(pArray).Swap(*this);
		}

		// 支持下标操作符
		T& operator [](ptrdiff_t i) const    
		{
			assert(i >= 0 && m_pArray != NULL);
			return m_pArray[i];
		}

		// 支持向bool类型隐式转换
		operator bool() const    
		{
			return m_pArray != NULL;
		}

	private:
		typedef XScopeArray<T> ThisType;
		DISALLOW_COPY_AND_ASSIGN(ThisType);

	private:
		T* m_pArray;
	};
	// end XScopeArray

	// XSharedCount
	class XSharedCount
	{
	public:
		// Ctor
		XSharedCount()
			:m_pUseCount(new size_t(1))
		{
			assert(m_pUseCount != NULL);
		}

		// Destor
		~XSharedCount()
		{
			if (--(*m_pUseCount) == 0)
			{
				delete m_pUseCount;
			}
		}

		// Copy Ctor
		XSharedCount(const XSharedCount& that)
		{
			assert(that.m_pUseCount != NULL);
			m_pUseCount = that.m_pUseCount;
			++(*m_pUseCount);
		}

		// Copy Assign
		XSharedCount& operator =(const XSharedCount& that)
		{
			if (this == &that)
			{
				return *this;
			}
			
			this->Reattach(that);
			return *this;
		}

	public:
		// reattach to anohter pointer
		bool Reattach(const XSharedCount& that)
		{
			if (this == &that)
			{
				return false;
			}

			if (--(*m_pUseCount) == 0)
			{
				delete m_pUseCount;
				m_pUseCount = that.m_pUseCount;
				this->AddRef();
				return true;
			}
			m_pUseCount = that.m_pUseCount;
			this->AddRef();
			return false;
		}

		// Unique
		bool IsOnly()
		{
			return *m_pUseCount == 1;
		}

	protected:
		void AddRef()
		{
			++(*m_pUseCount);
		}

	private:
		size_t* m_pUseCount;

	};
	// end XSharedCount

	// XSharedPtr
	template <class T>
	class XSharedPtr
	{
	public:
		// 无参构造
		XSharedPtr()
			:m_pObj(NULL)
		{

		}

		template <class Y>
			explicit XSharedPtr(Y* pObj)
		{
			T* pTemp = dynamic_cast<T*>(pObj);
			assert(pTemp != NULL);
			m_pObj = pTemp;
		}

		// 禁止由T* 隐式转换为XSharedPtr<T>
		explicit XSharedPtr(T* pObj)
			:m_pObj(pObj)
		{

		}

		// 不同类型的拷贝构造
		template <class Y>
		XSharedPtr(const XSharedPtr<Y>& that)
			:m_counter(that.GetCounter())
		{
			T* pTemp = dynamic_cast<T*>(that.Get());
			assert(pTemp != NULL);
			m_pObj = pTemp;
		}

		// 拷贝构造
		XSharedPtr(const XSharedPtr& that)
			:m_counter(that.m_counter)
			,m_pObj(that.m_pObj)
		{
			
		}

		// 不同类型的拷贝赋值
		template <class Y>
		XSharedPtr<T>& operator =(const XSharedPtr<Y>& that)
		{
			T* pTemp = dynamic_cast<T*>(that.m_pObj);
			assert(pTemp != NULL);

			if (m_counter.Reattach(that.m_counter))
			{
				if (m_pObj != NULL)
				{
					XGLOBALFUNC::check_delete(m_pObj);
				}
			}
			m_pObj = pTemp;

			return *this;
		}

		// 拷贝赋值
		XSharedPtr& operator =(const XSharedPtr& that)
		{
			if (this == &that)
			{
				return *this;
			}
			
			if (m_counter.Reattach(that.m_counter))
			{
				if (m_pObj != NULL)
				{
					XGLOBALFUNC::check_delete(m_pObj);
				}
			}
			m_pObj = that.m_pObj;
			
			return *this;
		}

		// 析构
		~XSharedPtr()
		{
			if (m_counter.IsOnly())
			{
				if (m_pObj != NULL)
				{
					XGLOBALFUNC::check_delete(m_pObj);
				}
			}
		}

		// 使之用起来和裸指针差不多
		T& operator *()
		{
			return *m_pObj;
		}

        // operator* const版本
		T& operator *() const
		{
			return *m_pObj;
		}

		// 使之用起来和裸指针差不多
		T* operator ->()
		{
			return m_pObj;
		}

		// operator-> const版本
		T* operator ->() const
		{
			return m_pObj;
		}

	public:
		// 获得裸指针
		T* Get() const
		{
			return m_pObj;
		}

		// 托管一个新的不同类型的裸指针
		template <class Y>
		void Reset(Y* pObj)
		{
			T* pTemp = dynamic_cast<T*>(pObj);
			assert(pTemp != NULL);
			Reset(pTemp);
		}

		// 托管一个新的裸指针
		void Reset(T* pObj)
		{
			XSharedCount sharedCount;
			if (m_counter.Reattach(sharedCount))
			{
				if (m_pObj != NULL)
				{
					delete m_pObj;
				}
			}
			m_pObj = pObj;
		}

		// 没什么用，仅仅为了解决编译问题
		const XSharedCount& GetCounter() const
		{
			return m_counter;
		}

		// 向bool隐式转换
		operator bool()
		{
			return m_pObj != NULL;
		}

		// 小于
		template <class Y>
		bool operator <(const XSharedPtr<Y>& that)
		{
			return m_pObj < that.m_pObj;
		}

		// 等于
		bool operator ==(const XSharedPtr& that)
		{
			return m_pObj == that.m_pObj;
		}

		// 不等于
		bool operator !=(const XSharedPtr& that)
		{
			return m_pObj != that.m_pObj;
		}

	private:
		XSharedCount m_counter;
		T* m_pObj;
	};

	//just for STL::set
	template <class T>
	class Xless
	{
	public:
		bool operator ()(const T& lh, const T& rh) const
		{
			return lh < rh;
		}
	};
	// end for STL::set
}

namespace XGLOBALFUNC
{
	template <class T>
	inline void check_delete(T* pObj)
	{
		typedef char TypeCheck[sizeof(T) ? 1 : -1];
		(void) sizeof(TypeCheck);
		delete pObj;
	}

	template <class T>
	inline void check_array_delete(T* pObjArray)
	{
		typedef char TypeCheck[sizeof(T) ? 1 : -1];
		(void) sizeof(TypeCheck);
		delete [] pObjArray;
	}
}

template <class T, class Y>
bool operator <(const XSMARTPTR::XSharedPtr<T>& lh, const XSMARTPTR::XSharedPtr<Y>& rh)
{
	return lh.Get() < rh.Get();
}

#undef DISALLOW_COPY_AND_ASSIGN
#undef FREE_POINTER
#undef FREE_ARRAY

#endif // XSMARTPTR_H
