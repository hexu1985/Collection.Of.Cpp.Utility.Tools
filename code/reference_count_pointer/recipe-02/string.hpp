#pragma once

#include "rc_object.hpp"
#include "rc_ptr.hpp"

#include <iosfwd>

/******************************************************************************
*           Classes String and StringValue (from pp. 204, 206-207)            *
******************************************************************************/
class String {                          // class to be used by
public:                                 // application developers
	String(const char *value = "");

	char operator[](int index) const;       
	char& operator[](int index);

private:
	// class representing string values
	struct StringValue: public RCObject {
		char *data;

		StringValue(const char *initValue);
		StringValue(const StringValue& rhs);
		void Init(const char *initValue);
		~StringValue();
	};

	RCPtr<StringValue> value;                       

	// This function is not in the book, but it's convenient for testing the
	// class -- see below.
	friend std::ostream& operator<<(std::ostream& stream, const String& string);
};

