/*
	Copyright (C) 2000 Nicolas Brodu, nicolas.brodu@free.fr
	                   Stefan Westerfeld, stefan@space.twc.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
   
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef MCOP_COMPONENT_REFERENCE_H
#define MCOP_COMPONENT_REFERENCE_H

#include "common.h"

// Acts as a string or ObjectReference, but enables a different constructor
class Reference {
private:
	ObjectReference myref;
	std::string mys;
	bool strMode;
public:
	
	inline Reference(const ObjectReference& ref) {
		myref=ref;
		strMode=false;
	}

	inline Reference(const std::string& s) {
		mys=s;
		strMode=true;
	}
	
	inline Reference(const char* c) {
		mys=c;
		strMode=true;
	}
	inline Reference& operator=(const std::string& s) {mys=s; return *this;}
	inline Reference& operator=(const char*c) {mys=c; return *this;}
	inline bool isString() const {return strMode;}
//	inline operator const std::string() const {return mys;}
	inline const std::string& string() const {return mys;}
//	inline operator const ObjectReference() const {return myref;}
	inline const ObjectReference& reference() const {return myref;}
};

// Acts as a string, but enables a different constructor
class SubClass {
private:
	std::string mys;
public:

	inline SubClass(const std::string& s) : mys(s) {}
	inline SubClass(const char* c) : mys(c) {}
	inline SubClass& operator=(const std::string& s) {mys=s; return *this;}
	inline SubClass& operator=(const char*c) {mys=c; return *this;}
//	inline operator const std::string() const {return mys;}
	inline const std::string& string() const {return mys;}
};

class SmartWrapper {
protected:
	bool _autoCreate;
public:
	SmartWrapper() { _autoCreate = true; }
};

#endif
