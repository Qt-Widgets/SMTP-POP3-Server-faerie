#include "pch.h"

SimpleContext::SimpleContext() : _index(0)
{
}
SimpleContext::~SimpleContext()
{
}
void SimpleContext::append( const string& data )
{
	_rawdata.append( data );
}

string SimpleContext::getline()
{
	string line;

	unsigned int pos = _rawdata.find( "\r\n", _index );
	
	if( pos != string::npos )
	{
		line = _rawdata.substr( _index, pos - _index );
	}
	_index = pos != string::npos ? pos + 2 : _rawdata.length() < 4 ? 0 : _rawdata.length() - 3;

	return line;
}
