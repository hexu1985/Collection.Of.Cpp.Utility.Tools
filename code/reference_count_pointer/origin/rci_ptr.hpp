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

	RCObject& getRCObject()               // give clients access to
	{ return *counter; }                  // isShared, etc.

private:
	struct CountHolder: public RCObject {
		~CountHolder() { delete pointee; }
		T *pointee;
	};

	CountHolder *counter;
	void init();
};

template<class T>
void RCIPtr<T>::init()
{
	if (counter->isShareable() == false) {
		T *oldValue = counter->pointee;
		counter = new CountHolder;
		counter->pointee = oldValue ? new T(*oldValue) : 0;
	} 

	counter->addReference();
}

template<class T>
RCIPtr<T>::RCIPtr(T* realPtr)
	: counter(new CountHolder)
{ 
	counter->pointee = realPtr;
	init();
}

template<class T>
RCIPtr<T>::RCIPtr(const RCIPtr& rhs)
	: counter(rhs.counter)
{ init(); }

template<class T>
RCIPtr<T>::~RCIPtr()
{ counter->removeReference(); }

template<class T>
RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs)
{
	if (counter != rhs.counter) {         
		counter->removeReference();     
		counter = rhs.counter;
		init();
	}
	return *this;
}

template<class T>
T* RCIPtr<T>::operator->() const
{ return counter->pointee; }

template<class T>
T& RCIPtr<T>::operator*() const
{ return *(counter->pointee); }
