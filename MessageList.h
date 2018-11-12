#pragma once

#include "pch.h"


class MessageList
{
protected:
	vector<PopMessage> _messages;

	unsigned int _tt_msg;
	unsigned int _tt_size;
public:
	
	void add( const PopMessage& message );
	unsigned int get_id( unsigned int msg_nr );
	
	string list();
	string list( unsigned int msg_nr );
	
	string uidl();
	string uidl( unsigned int msg_nr );
	
	string dele( unsigned int msg_nr );
	
	void remove_the_marks();
	
	unsigned int total_size();
	unsigned int total_msg();

	MessageList();
	virtual ~MessageList();
};
