#include "rc_object.hpp"

namespace mini_util {

RCObject::RCObject(): refCount_(0) {}

RCObject::RCObject(const RCObject&) : refCount_(0) {}

RCObject& RCObject::operator=(const RCObject&) { return *this; }

RCObject::~RCObject() {}

void RCObject::addReference() { ++refCount_; }

void RCObject::removeReference() { if (--refCount_ == 0) delete this; }

}	// namespace mini_util
