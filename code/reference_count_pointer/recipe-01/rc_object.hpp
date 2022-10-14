#pragma once

/******************************************************************************
*                       Class RCObject (from pp. 204-205)                     *
******************************************************************************/
class RCObject {                       // base class for reference-
public:                                // counted objects
	void AddReference();
	void RemoveReference();
	bool IsShared() const;

protected:
	RCObject();
	RCObject(const RCObject& rhs);
	RCObject& operator=(const RCObject& rhs);
	virtual ~RCObject() = 0;

private:
	int refCount=0;
};

