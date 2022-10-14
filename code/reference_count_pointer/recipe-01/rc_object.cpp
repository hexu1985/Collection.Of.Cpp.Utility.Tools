#include "rc_object.hpp"

RCObject::RCObject() 
    : refCount(0) {}

RCObject::RCObject(const RCObject&) 
    : refCount(0) {}

RCObject& RCObject::operator=(const RCObject&)
{
	return *this;
}  

RCObject::~RCObject() {}

void RCObject::AddReference() 
{
	++refCount;
}

void RCObject::RemoveReference()
{
	if (--refCount == 0) delete this;
}

bool RCObject::IsShared() const
{
	return refCount > 1;
}
