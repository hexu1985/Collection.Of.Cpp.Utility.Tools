#pragma once

/******************************************************************************
*                 Template Class RCPtr (from pp. 203, 206)                    *
******************************************************************************/
template<class T>                      // template class for smart
class RCPtr {                          // pointers-to-T objects; T
public:                                // must support the RCObject interface
	RCPtr(T* realPtr = 0);
	RCPtr(const RCPtr& rhs);
	~RCPtr();
	RCPtr& operator=(const RCPtr& rhs);                     
	T* operator->() const;
	T& operator*() const;

private:
	T *pointee;
};

template<class T>
RCPtr<T>::RCPtr(T* realPtr)
: pointee(realPtr)
{
	if (pointee) pointee->AddReference();
}

template<class T>
RCPtr<T>::RCPtr(const RCPtr& rhs)
: pointee(rhs.pointee)
{ 
	if (pointee) pointee->AddReference();
}

template<class T>
RCPtr<T>::~RCPtr()
{
	if (pointee) pointee->RemoveReference();
}

template<class T>
RCPtr<T>& RCPtr<T>::operator=(const RCPtr& rhs)
{
	if (pointee != rhs.pointee) {                   // this code was modified
		T *oldPointee = pointee;                    // for the book's 10th
		                                            // printing
		pointee = rhs.pointee;
        if (pointee) pointee->AddReference();

		if (oldPointee) oldPointee->RemoveReference();                
	}

	return *this;
}

template<class T>
T* RCPtr<T>::operator->() const 
{
	return pointee;
}

template<class T>
T& RCPtr<T>::operator*() const
{
	return *pointee;
}

