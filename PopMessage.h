#pragma once 

#include "pch.h"

struct PopMessage
{
	int id;
	unsigned int size;
	bool deleted;

	PopMessage( int _id, unsigned int _size )
	{
		id = _id;
		size = _size;
		deleted = false;
	}
};
