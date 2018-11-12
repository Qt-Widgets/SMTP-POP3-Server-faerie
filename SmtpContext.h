#pragma once

#include "pch.h"

class SmtpContext : public SimpleContext
{
private: 
	unsigned int idx_data;
	EmailAddress _from;
	map< string, set<string> > _recipients;

public:
	EmailAddress& from()
	{
		return _from;
	}
	void add_rcpt( EmailAddress& recipient )
	{
		_recipients[ recipient.domain() ].insert( recipient.mailbox() );
	}
	void begin_data()
	{
		idx_data = _index;
	}
	void removedot()
	{
		_rawdata.erase( _rawdata.size() - 3, 1 );
	}
	void reset()
	{
		_rawdata.erase( 0, _index ); 
		_index = 0;
		_from.domain() = "";
		_from.mailbox() = "";
		_recipients.clear();
	}
	SmtpContext(){};
	virtual ~SmtpContext(){};
};