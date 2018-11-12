#ifndef SERVER_H
#define SERVER_H

#include "pch.h" 

class Server
{
protected:
	unsigned short port;

public:
	virtual void start() = 0;
	virtual void stop() = 0;

	Server(){};
	virtual ~Server(){};
};



#endif