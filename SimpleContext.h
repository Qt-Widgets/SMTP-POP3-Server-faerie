#pragma once

#include "pch.h"

class SimpleContext
{
protected:
	string _rawdata;
	unsigned int _index;
public:
	void append( const string& data );
	string getline();

	SimpleContext();
	virtual ~SimpleContext();
};