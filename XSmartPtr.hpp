#ifndef XSMARTPTR_H
#define XSMARTPTR_H

#include "stddef.h"
#include "assert.h"
#include "typeinfo.h"
#include "winbase.h"

using namespace std;

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
		// ���°�һ����ָ�룬֮ǰ�йܵ���ָ��ᱻ�ͷ�
		void Reset(T* pObj)    
		{
			assert(pObj == NULL || pObj != m_pObj);
			ThisType(pObj).Swap(*this);
		}

		// ��������XScopePtr�����йܵ�ָ��
		void Swap(XScopePtr& that)    
		{
			T* pTemp = that.m_pObj;
			that.m_pObj = this->m_pObj;
			this->m_pObj = pTemp;
		}

		// ֧�ֽ�����
		T& operator *()    
		{
			assert(m_pObj != NULL);
			return *m_pObj;
		}

		// ʹ֮����������ָ����Ϊ��ࡣ
		T* operator ->()    
		{
			assert(m_pObj != NULL);
			return m_pObj;
		}

		// �����ָ�롣�û�����ͷ���ָ�룬�ᵼ��DoubleFree��
		T* Get() const                 
		{
			return m_pObj;
		}
		
		// ֧����bool������ʽת�������ԣ�XScopePtr a(new T); if (a){...} �ǺϷ��ġ�
		operator bool()    
		{
			return m_pObj != NULL;
		}

	private:
		typedef XScopePtr<T> ThisType;
		XScopePtr(const XScopePtr& that);
		XScopePtr& operator =(const XScopePtr& that);
		bool operator ==(const XScopePtr& that);
		bool operator !=(const XScopePtr& that);

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
		// �����ָ��
		T* Get() const    
		{
			return m_pArray;
		}

		// �����йܵ���ָ��
		void Swap(XScopeArray& that)    
		{
			T* pTemp = that.m_pArray;
			that.m_pArray = this->m_pArray;
			this->m_pArray = pTemp;
		}

		// ���°���ָ�룬֮ǰ�Ļᱻ����
		void Reset(T* pArray)    
		{
			assert(pArray == NULL || pArray != m_pArray);
			ThisType(pArray).Swap(*this);
		}

		// ֧���±������
		T& operator [](ptrdiff_t i) const    
		{
			assert(i >= 0 && m_pArray != NULL);
			return m_pArray[i];
		}

		// ֧����bool������ʽת��
		operator bool() const    
		{
			return m_pArray != NULL;
		}

	private:
		typedef XScopeArray<T> ThisType;
		XScopeArray(const XScopeArray& that);
		XScopeArray& operator =(const XScopeArray& that);
		bool operator ==(const XScopeArray& that);
		bool operator !=(const XScopeArray& that);

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
		// �޲ι���
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

		// ��ֹ��T* ��ʽת��ΪXSharedPtr<T>
		explicit XSharedPtr(T* pObj)
			:m_pObj(pObj)
		{

		}

		// ��ͬ���͵Ŀ�������
		template <class Y>
		XSharedPtr(const XSharedPtr<Y>& that)
			:m_counter(that.GetCounter())
		{
			T* pTemp = dynamic_cast<T*>(that.Get());
			assert(pTemp != NULL);
			m_pObj = pTemp;
		}

		// ��������
		XSharedPtr(const XSharedPtr& that)
			:m_counter(that.m_counter)
			,m_pObj(that.m_pObj)
		{
			
		}

		// ��ͬ���͵Ŀ�����ֵ
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

		// ������ֵ
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

		// ����
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

		// ʹ֮����������ָ����
		T& operator *()
		{
			return *m_pObj;
		}

        // operator* const�汾
		T& operator *() const
		{
			return *m_pObj;
		}

		// ʹ֮����������ָ����
		T* operator ->()
		{
			return m_pObj;
		}

		// operator-> const�汾
		T* operator ->() const
		{
			return m_pObj;
		}

	public:
		// �����ָ��
		T* Get() const
		{
			return m_pObj;
		}

		// �й�һ���µĲ�ͬ���͵���ָ��
		template <class Y>
		void Reset(Y* pObj)
		{
			T* pTemp = dynamic_cast<T*>(pObj);
			assert(pTemp != NULL);
			Reset(pTemp);
		}

		// �й�һ���µ���ָ��
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

		// ûʲô�ã�����Ϊ�˽����������
		const XSharedCount& GetCounter() const
		{
			return m_counter;
		}

		// ��bool��ʽת��
		operator bool()
		{
			return m_pObj != NULL;
		}

		// С��
		template <class Y>
		bool operator <(const XSharedPtr<Y>& that)
		{
			return m_pObj < that.m_pObj;
		}

		// ����
		bool operator ==(const XSharedPtr& that)
		{
			return m_pObj == that.m_pObj;
		}

		// ������
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



#endif // XSMARTPTR_H