#include "rc_object.hpp"

RCObject::RCObject(): refCount(0) {}

RCObject::RCObject(const RCObject&) : refCount(0) {}

RCObject& RCObject::operator=(const RCObject&) { return *this; }

RCObject::~RCObject() {}

void RCObject::addReference() { ++refCount; }

void RCObject::removeReference() { if (--refCount == 0) delete this; }

