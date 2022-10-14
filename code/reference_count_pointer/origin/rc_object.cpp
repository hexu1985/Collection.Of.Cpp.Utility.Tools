#include "rc_object.hpp"

RCObject::RCObject()
	: refCount(0), shareable(true) {}

RCObject::RCObject(const RCObject&)
	: refCount(0), shareable(true) {}

RCObject& RCObject::operator=(const RCObject&)
{
	return *this;
}  

RCObject::~RCObject() {}

void RCObject::addReference() 
{
	++refCount;
}

void RCObject::removeReference()
{
	if (--refCount == 0) delete this;
}

void RCObject::markUnshareable()
{
	shareable = false;
}

bool RCObject::isShareable() const
{
	return shareable;
}

bool RCObject::isShared() const
{
	return refCount > 1;
}
