#include "pch.h"

PopRequestHandler::PopRequestHandler( shared_ptr<SWInetSocket> incoming ):service_available(1)
{
	_incoming = incoming;
	datahandler = std::bind( &PopRequestHandler::authorization, this);
}
PopRequestHandler::~PopRequestHandler()
{
}

void PopRequestHandler::respond()
{
	send(  "+OK POP3 server ready.\r\n"  );

	while( service_available )
	{
		string line = context.getline();

		if( !line.size() )
			context.append( _incoming->recvmsg() );
		else
			datahandler( line );
	}
}
vector<string> PopRequestHandler::split( const string& line )
{
	//char_separator<char> sep( ": " );
	//tokenizer< boost::char_separator<char> > tk( line, sep );
	//tokenizer< boost::char_separator<char> >::iterator it;
	
	vector<string> pairs;

	//for(it = tk.begin(); it != tk.end(); ++it)
	//	pairs.push_back( *it );

	return pairs;
}


void PopRequestHandler::authorization( const string& line )
{
	if(line.find("USER ") == 0 )
		user( line );
	else if( line == "QUIT")
		quit();
}
void PopRequestHandler::user( const string& line )
{
	vector<string> pairs = split( line );

	if( pairs.size() < 2 )
	{
		send( "-ERR USER command requires at least a parameter\r\n" );
		return;
	}
	
	_username.parse( pairs[1] );

	if( !_username.isvalid() )
	{
		send( "-ERR The username should be valid e-mail address\r\n" );
		return;
	}

	datahandler = std::bind( &PopRequestHandler::pass, this);
}
void PopRequestHandler::pass( const string& line )
{
	if( line.find("PASS ") != 0 )
	{
		send( "-ERR The next command to the USER command should be valid PASS command\r\n" );
		datahandler = std::bind( &PopRequestHandler::authorization, this);
		return;
	}
	vector<string> pairs = split( line );
	if( pairs.size() < 2 )
	{
		send( "-ERR Pass command requires a parameter containing your password\r\n" );
		datahandler = std::bind( &PopRequestHandler::authorization, this);
		return;
	}

	string password = pairs[1];
	//
	//TODO: kullanýcý adý ve þifrenin doðruluðunu kontrol et
	//
	bool validuser = true;
	
	if( !validuser )
	{
		send( "-ERR Invalid credentials\r\n" );
		datahandler = std::bind( &PopRequestHandler::authorization, this);
		return;	
	}

	//
	//TODO: Postalarýn listesini oluþtur
	//



	datahandler = std::bind( &PopRequestHandler::transaction, this);
	send( "+OK YOUR maildrop has XXX messages" );
}
void PopRequestHandler::update()
{
	//save the state
	//release the resources
	//say goodbye
}

void PopRequestHandler::transaction( const string& line )
{
	if (line == "STAT")
		stat();
	else if (line.find("LIST") == 0)
		list(line);
	else if (line.find("DELE ") == 0)
		dele(line);
	else if (line.find("RETR ") == 0)
		retr(line);
	else if (line.find("UIDL") == 0)
		uidl(line);
	else if (line == "NOOP")
		send("+OK\r\n");
	else if (line == "RSET")
		rset();
	else if (line == "QUIT")
		quit();
	else
		send("-ERR Unknown Command\r\n");
}

void PopRequestHandler::stat()
{
	send( "+OK "+ to_string(messages.total_msg()) 
		+" "+ to_string(messages.total_size()) +"\r\n" );
}

void PopRequestHandler::rset()
{
	messages.remove_the_marks();

	send( "+OK "+ to_string(messages.total_msg()) 
		+" "+ to_string(messages.total_size()) +"\r\n" );
}
void PopRequestHandler::list( const string& line )
{
	vector<string> pairs = split( line );
	
	string scan_list;

	if( pairs.size() < 2 )
	{
		scan_list = messages.list();
	}
	else
	{
		unsigned int msg_nr;
		try
		{
			msg_nr = stoi(pairs[1]);
		}
		catch( ... )
		{
			send( "-ERR Invalid Parameter\r\n" );
			return;
		}
		scan_list = messages.list( msg_nr );
	}
	send( scan_list );
}
void PopRequestHandler::retr( const string& line )
{
	vector<string> pairs = split( line );
	if( pairs.size() < 2 )
	{
		send( "-ERR RETR command requires at least a parameter\r\n" );
		return;
	}
	
	unsigned int msg_nr;
	try
	{
		msg_nr = stoi(pairs[1]);
	}
	catch( ... )
	{
		send( "-ERR Invalid parameter\r\n" );
		return;
	}
	
	unsigned int id = messages.get_id( msg_nr );

	if( !id )
	{
		send( "-ERR No such message\r\n" );
		return;
	}
	
	//
	//TODO:Veritabanýndan postayý al ve bu deðiþkene ata
	//
	string mail;

	send( messages.list( msg_nr ) );
	send( mail );
	send( "\r\n.\r\n" );
}
void PopRequestHandler::dele( const string& line )
{
	vector<string> pairs = split( line );
	if( pairs.size() < 2 )
	{
		send( "-ERR DELE command requires at least a parameter\r\n" );
		return;
	}
	
	unsigned int msg_nr;
	try
	{
		msg_nr = stoi(pairs[1]);
	}
	catch( ... )
	{
		send( "-ERR Invalid parameter\r\n" );
		return;
	}
	send( messages.dele( msg_nr ) );
}

void PopRequestHandler::uidl( const string& line )
{
	vector<string> pairs = split( line );
	
	string scan_list;

	if( pairs.size() < 2 )
	{
		scan_list = messages.uidl();
	}
	else
	{
		unsigned int msg_nr;
		try
		{
			msg_nr = stoi(pairs[1]);
		}
		catch( ... )
		{
			send( "-ERR Invalid parameter\r\n" );
			return;
		}
		scan_list = messages.uidl( msg_nr );
	}
	send( scan_list );
}
void PopRequestHandler::quit()
{
	//save the state?


	send( "+OK POP3 server signing off" );

	service_available = false;
}
void PopRequestHandler::perform()
{
	respond();
}
void PopRequestHandler::send( const string& data )
{
	_incoming->sendmsg( data );
}