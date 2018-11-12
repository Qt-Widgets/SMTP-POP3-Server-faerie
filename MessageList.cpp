#include "pch.h"

MessageList::MessageList() : _tt_size(0), _tt_msg(0)
{
}
MessageList::~MessageList()
{
}

void MessageList::add( const PopMessage& message )
{
	_tt_msg++;
	_tt_size += message.size;
	_messages.push_back( message );
}

unsigned int MessageList::get_id( unsigned int msg_nr )
{
	if( msg_nr < 1 || _messages.size() < msg_nr )
		return 0;
	if( _messages[ msg_nr - 1 ].deleted )
		return 0;

	return _messages[msg_nr - 1].id;
}
string MessageList::list()
{
	string scan_list = "+OK "+ to_string(_tt_msg) +" messages ("+ to_string(_tt_size) +") octets\r\n";

	for( unsigned int i = 0; i < _messages.size(); i++ )
	{
		if( _messages[i].deleted )
			continue;

		scan_list.append( to_string(i+1) + " " + to_string(_messages[i].size) +"\r\n");
	}
	scan_list.append( ".\r\n" );

	return scan_list;
}

string MessageList::list( unsigned int msg_nr )
{
	if( _messages.size() < msg_nr || msg_nr < 1 )
		return "-ERR no such message\r\n";

	return ("+OK "+ to_string(msg_nr) +" "+ to_string( _messages[ msg_nr - 1 ].size ) +"\r\n");
}
string MessageList::uidl()
{
	string scan_list = "+OK";

	for( unsigned int i = 0; i < _messages.size(); i++ )
	{
		if( _messages[i].deleted )
			continue;

		scan_list.append( to_string(i+1) + " " + to_string(_messages[i].id) +"\r\n");
	}
	scan_list.append( ".\r\n" );

	return scan_list;
}
string MessageList::uidl( unsigned int msg_nr )
{
	if( msg_nr < 1 || _messages.size() < msg_nr )
		return "-ERR No such message\r\n";
	
	if( _messages[ msg_nr-1 ].deleted )
		return "-ERR Message "+ to_string(msg_nr) +" marked as deleted\r\n";
	
	return "+OK "+ to_string(msg_nr) +" "+ to_string(_messages[msg_nr-1].id) +"\r\n";
}
string MessageList::dele( unsigned int msg_nr )
{
	if( msg_nr < 1 || _messages.size() < msg_nr )
		return "-ERR No such message\r\n";
	
	if( _messages[ msg_nr - 1 ].deleted )
		return "-ERR Message"+ to_string(msg_nr) +"already deleted\r\n";

	_messages[ msg_nr - 1 ].deleted = true;

	return "+OK Message "+ to_string(msg_nr) +" deleted\r\n";
}
void MessageList::remove_the_marks()
{
	for( unsigned int i = 0; i < _messages.size(); i++ )
		_messages[i].deleted = false;
}
unsigned int MessageList::total_size()
{
	return _tt_size;
}

unsigned int MessageList::total_msg()
{
	return _tt_msg;
}