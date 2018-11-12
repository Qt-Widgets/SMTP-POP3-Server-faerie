//#ifndef IREQUESTHANDLER_H
//#define IREQUESTHANDLER_H
#pragma once

#include "pch.h" 

class IRequestHandler //: public IWork
{
public:
	virtual void perform() = 0;
	virtual void respond() = 0;

	IRequestHandler(){};
	virtual ~IRequestHandler(){};
};



//#endif