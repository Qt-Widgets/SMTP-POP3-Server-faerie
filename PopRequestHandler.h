//#ifndef POPREQUESTHANDLER_H
//#define POPREQUESTHANDLER_H
#pragma once

#include "pch.h" 

class SWInetSocket;

class PopRequestHandler : public IRequestHandler
{
private:
	bool service_available;
	EmailAddress _username;
	

	SimpleContext context;
	MessageList messages;
	shared_ptr<SWInetSocket> _incoming;

	function<void( const string& )> datahandler;
	void authorization( const string& line );
	void transaction( const string& line );

	vector<string> split( const string& line );
	void send( const string& data );
	
	void respond();
	void update();

	void stat();
	void rset();
	void user( const string& line );
	void pass( const string& line );
	void list( const string& line );
	void retr( const string& line );
	void dele( const string& line );
	void uidl( const string& line );

	void quit();


public:
	void perform();

	PopRequestHandler( shared_ptr<SWInetSocket> incoming );
	virtual ~PopRequestHandler();
};


//#endif