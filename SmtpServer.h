#ifndef SMTPSERVER_H
#define SMTPSERVER_H

#include "pch.h"

class SmtpServer : public Server
{
public:

	void start();
	void stop();

	SmtpServer(void);
	virtual ~SmtpServer(void);
};

#endif
