//#ifndef SMTPREQUESTHANDLER_H
//#define SMTPREQUESTHANDLER_H
#pragma once
#include "pch.h" 

#define HELO		1
#define MAIL_FROM	2
#define RCPT_TO		3
#define DATA		4


class SmtpRequestHandler : public IRequestHandler
{
protected:
	bool service_available, have_tr;
	int state;

	SmtpContext context;
	
	function<void( const string& )> datahandler;
	shared_ptr<SWInetSocket> _incoming;
	
	void respond();
	void processor( const string& line );
	vector<string> split( const string& data );
	void send( const string& line );

	void helo( const string& line );
	void ehlo( const string& line );
	void from( const string& line );
	void rcpt( const string& line );
	void data( const string& line );
	void noop();
	void rset();
	void quit();

public:
	void perform();

	SmtpRequestHandler( shared_ptr<SWInetSocket> incoming );
	virtual ~SmtpRequestHandler();
};



//#endif