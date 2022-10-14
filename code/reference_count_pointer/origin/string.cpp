#include "string.hpp"

#include <cstring>
#include <iostream>

void String::StringValue::init(const char *initValue)
{
	using namespace std;

	data = new char[strlen(initValue) + 1];
	strcpy(data, initValue);
}

String::StringValue::StringValue(const char *initValue)
{ 
	init(initValue);
}

String::StringValue::StringValue(const StringValue& rhs)
{
	init(rhs.data);
}

String::StringValue::~StringValue()
{
	delete [] data;
}


String::String(const char *initValue)
	: value(new StringValue(initValue)) {}

char String::operator[](int index) const
{
	return value->data[index];
}

char& String::operator[](int index)
{ 
	if (value->isShared()) {      
		value = new StringValue(value->data);
	}

	value->markUnshareable();

	return value->data[index];
}

std::ostream& operator<<(std::ostream& stream, const String& string)
{
	return stream << string.value->data;
}

