//#ifndef EMAILADDRESS_H
//#define EMAILADDRESS_H
#pragma once
#include "pch.h"

static regex rgx_eml( "^([a-zA-Z0-9_\\-\\.]+)@(([a-zA-Z0-9\\-]+\\.)+([a-zA-Z]{2,4})?)$" );

class EmailAddress
{
protected:
	bool _valid;
	string _mailbox, _domain;
	smatch what;

public:

	string& mailbox()
	{
		return _mailbox;
	}
	string& domain()
	{
		return _domain;
	}

	string to_string()
	{
		return _mailbox+"@"+_domain;
	}
	bool& isvalid()
	{
		return _valid;
	}

	void parse( const string& address )
	{
		_valid = regex_match( address, what, rgx_eml ) ? true : false;

		if( _valid )
		{
			_mailbox = what[1];
			_domain = what[2];
		}
	}

	EmailAddress(){};
	EmailAddress( string address )
	{
		parse( address );
	}
	EmailAddress( string mailbox, string domain )
	{
		parse( mailbox + "@" + domain );
	}
	
	virtual ~EmailAddress(){};
};

//
//#endif