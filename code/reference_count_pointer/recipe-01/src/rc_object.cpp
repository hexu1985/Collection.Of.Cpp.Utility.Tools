#include "rc_object.hpp"

RCObject::RCObject(): refCount_(0) {}

RCObject::RCObject(const RCObject&) : refCount_(0) {}

RCObject& RCObject::operator=(const RCObject&) { return *this; }

RCObject::~RCObject() {}

void RCObject::addReference() { ++refCount_; }

void RCObject::removeReference() { if (--refCount_ == 0) delete this; }

