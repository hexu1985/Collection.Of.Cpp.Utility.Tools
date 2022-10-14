#pragma once

/******************************************************************************
*                  Template Class RCIPtr (from pp. 209-210)                   *
*                                                                             *
* The code for RCIPtr has changed over the years in response to errors        *
* both in the original source code as well as in the subsequent fixes.  You   *
* can find a complete list of changes at the More Effective C++ errata page.  *
* The code here is accurate as of the 13th printing of the book.              *
******************************************************************************/
template<class T>
class RCIPtr {
public:
	RCIPtr(T* realPtr = 0);
	RCIPtr(const RCIPtr& rhs);
	~RCIPtr();
	RCIPtr& operator=(const RCIPtr& rhs);

	T* operator->() const;
	T& operator*() const;

	bool IsShared() const;

private:
	struct CountHolder: public RCObject {
		~CountHolder() { delete pointee; }
		T *pointee;
	};

	CountHolder *counter;
};

template<class T>
RCIPtr<T>::RCIPtr(T* realPtr)
	: counter(new CountHolder)
{ 
	counter->pointee = realPtr;
	counter->AddReference();
}

template<class T>
RCIPtr<T>::RCIPtr(const RCIPtr& rhs)
	: counter(rhs.counter)
{ counter->AddReference(); }

template<class T>
RCIPtr<T>::~RCIPtr()
{ counter->RemoveReference(); }

template<class T>
RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs)
{
	if (counter != rhs.counter) {         
		counter->RemoveReference();     
		counter = rhs.counter;
        counter->AddReference();
	}
	return *this;
}

template<class T>
T* RCIPtr<T>::operator->() const
{ return counter->pointee; }

template<class T>
T& RCIPtr<T>::operator*() const
{ return *(counter->pointee); }

template<class T>
bool RCIPtr<T>::IsShared() const
{ return counter->IsShared(); }

