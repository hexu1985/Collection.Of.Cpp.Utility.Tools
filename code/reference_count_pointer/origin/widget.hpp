#pragma once

/******************************************************************************
*                          Class Widget (from p. 210)                         *
*                                                                             *
* This class is the same as that in the book, but I've added an               *
* implementation so that RCIPtr can be tested.                                *
******************************************************************************/
class Widget {
public:
	Widget(int size): value(size) {}
	Widget(const Widget& rhs): value(rhs.value) {}
	~Widget() {}

	Widget& operator=(const Widget& rhs)
	{
		value = rhs.value;
		return *this; 
	}

	void doThis()
	{ value = -1; }

	int showThat() const
	{ return value; }

private:
	int value;
};
