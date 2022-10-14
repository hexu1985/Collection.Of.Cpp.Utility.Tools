#pragma once

/******************************************************************************
*                                                                             *
*        Code from Item 29 ("Reference Counting") of MORE EFFECTIVE C++       *
*                                                                             *
*                               Scott Meyers                                  *   
*                                                                             *
*            Copyright 1996 (c) Addison-Wesley Publishing Company             *
*       You are free to use this code for non-commercial purposes only.       *
*                                                                             *
* This page contains the code for the classes and class templates making up   *
* the Reference Counting Item of More Effective C++.  To use this code,       *
* either copy this page and paste it into a C++ source file or save the       *
* entire page as text into a C++ source file.  Don't save the HTML source     *
* and expect that to compile :-)                                              *
*                                                                             *
* Each class or template in this file follows a block comment that shows the  *
* corresponding pages in the book.  This page also contains a main function   *
* that performs a VERY limited test of the code in this file.  You can        *
* compile the code in this file as a stand-alone program, and you should get  *
* this output:                                                                *
*                                                                             *
*     String with no changes.                                                 *
*     String with    changes.                                                 *
*     10                                                                      *
*     -1                                                                      *
*                                                                             *
* The code here reflects all changes made to date in response to bug reports  *
* from readers of the book.  (To see a complete list of known bugs in More    *
* Effective C++, as well as whether they've been fixed yet, visit the         *
* More Effective C++ Errata List.)  If you find any additional bugs, please   *
* send them to me.                                                            *
******************************************************************************/
#include <iostream>      // The iostream facilities are not used in the classes
                         // in this file, but they are used in the code that 
                         // tests the classes.

#include <cstring>       // This includes the C string functions, e.g.,
                         // strlen, strcpy, etc.  They are used in the
                         // implementation of class String::StringValue.

// The following is for compilers that don't support bool.  Uncomment these
// lines if your compilers lack bool support.  For details on this emulation
// of bool, see More Effective C++, pp. 3-4.
// typedef int bool;
// const bool false = 0;
// const bool true = 1;

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

