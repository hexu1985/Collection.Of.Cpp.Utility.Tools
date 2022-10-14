#pragma once

#include "widget.hpp"
#include "rci_ptr.hpp"

/******************************************************************************
*                         Class RCWidget (from p. 210)                        *
*                                                                             *
* I modified this class for the 13th printing of the book.  For details on    *
* why, consult the erratum affecting pages 209, 210, and 316 that was fixed   *
* on 7/15/02 in the book's errata list.                                       *
******************************************************************************/
class RCWidget {
public:
	RCWidget(int size)
		: value(new Widget(size)) {}

	void DoThis()
	{
		if (value.GetRCObject().IsShared()) {         // do COW if 
			value = new Widget(*value);               // Widget is shared
		}
		value->DoThis();
	}

	int ShowThat() const
	{ return value->ShowThat(); }

private:
	RCIPtr<Widget> value;
};
