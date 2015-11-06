#ifndef XSMARTPTR_HPP
#define XSMARTPTR_HPP

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
	ClassName(const ClassName&); \
	ClassName& operator =(const ClassName&)
#endif // DISALLOW_COPY_AND_ASSIGN

#ifndef FREE_POINTER
#define FREE_POINTER(p) \
	if (p) { delete p; p = NULL; }
#endif // FREE_POINTER

#ifndef FREE_ARRAY
#define FREE_ARRAY(pArray) \
	if (pArray) { delete [] pArray; pArray = NULL; }
#endif // FREE_ARRAY
	

namespace XSMARTPTR
{
	// XScopePtr
	template<class T>
	class XScopePtr
	{
		typedef XScopePtr<T> ThisType;
		
	public:
	
		// ctor
		explicit XScopePtr(T* pObj = NULL)
			:m_pObj(pObj)
		{
			
		}
		// dector
		~XScopePtr()
		{
			FREE_POINTER(m_pObj);
		}
		// reset
		void Reset(T* pOtherObj)
		{
			if (pOtherObj == NULL)
			{
				throw exception("null ptr");
			}
			
			
		}
		
	private:
	
		DISALLOW_COPY_AND_ASSIGN;
		T* m_pObj;
	};
	
}

#endif // XSMARTPTR_HPP
