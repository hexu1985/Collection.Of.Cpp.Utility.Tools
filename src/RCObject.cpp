#include "RCObject.hpp"

namespace mini_utils {

RCObject::RCObject()
: refCount_(0), shareable_(true) {}

RCObject::RCObject(const RCObject&)
: refCount_(0), shareable_(true) {}

RCObject& RCObject::operator=(const RCObject&)
{ return *this; }

RCObject::~RCObject() {}

void RCObject::addReference() { ++refCount_; }

void RCObject::removeReference()
{ if (--refCount_ == 0) delete this; }

void RCObject::markUnshareable()
{ shareable_ = false; }

bool RCObject::isShareable() const
{ return shareable_; }

bool RCObject::isShared() const
{ return refCount_ > 1; }

}	// namespace mini_utils
