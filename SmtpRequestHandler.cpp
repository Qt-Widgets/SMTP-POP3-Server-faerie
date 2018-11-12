#include "pch.h"

SmtpRequestHandler::SmtpRequestHandler( shared_ptr<SWInetSocket> incoming ):service_available(1), have_tr(0), state(0)
{
	_incoming = incoming;
	datahandler = std::bind( &SmtpRequestHandler::processor, this);
}
SmtpRequestHandler::~SmtpRequestHandler()
{
}

void SmtpRequestHandler::respond()
{
	send( "220 Service Ready.\r\n" );

	while( service_available )
	{
		string line = context.getline();

		if( !line.size() )
			context.append( _incoming->recvmsg() );
		else
			datahandler( line );
	}
	//request.Close();
}
void SmtpRequestHandler::processor( const string& line )
{
	if ( line.find("HELO ") == 0)
		helo(line);
	else if (line.find("EHLO ") == 0)
		ehlo(line);
	else if (line.find("MAIL FROM:") == 0)
		from(line);
	else if (line.find("RCPT TO:") == 0)
		rcpt(line);
	else if (line.find("VRFY ") == 0)
		send("252 Verifiyng isn't possible, but accepting.\r\n");
	else if (line == "NOOP")
		send("250 Ok\r\n");
	else if (line == "RSET")
		rset();
	else if (line == "QUIT")
		quit();
	else if (line == "DATA")
	{
		context.begin_data();
		send("354 End with <CRLF>.<CRLF>\r\n");
		datahandler = std::bind(&SmtpRequestHandler::data, this);
	}
	else
		send("500 Unknown Command.\r\n");
 	
}
vector<string> SmtpRequestHandler::split( const string& line )
{
	//char_separator<char> sep( ": " );
	//tokenizer< boost::char_separator<char> > tk( line, sep );
	//tokenizer< boost::char_separator<char> >::iterator it;
	
	vector< string > pairs;

	//for(it = tk.begin(); it != tk.end(); ++it)
	//	pairs.push_back( *it );

	return pairs;
}

void SmtpRequestHandler::helo( const string& line )
{
	if( have_tr )
		context.reset();

	state = HELO;
	send( "250 Hello\r\n" );
}
void SmtpRequestHandler::ehlo( const string& line )
{
	if( have_tr )
		context.reset();	
	
	state = HELO;

	send( "250-Hello\r\n" );
	send( "250-AUTH LOGIN\r\n" );
	send( "250-AUTH=LOGIN\r\n" );
	send( "250 PIPELINING\r\n" );
}
void SmtpRequestHandler::from( const string& line )
{
	if( state < HELO )
	{
		send( "503 Bad Sequence\r\n" ); 
		return;
	}	
	
	vector<string> pairs = split( line );
	if( pairs.size() < 3 )
	{
		send( "501 MAIL FROM: command requires at least one parameter\r\n" );
		return;
	}

	EmailAddress from( pairs[2] );
	if( !from.isvalid() )
	{
		send( "501 Invalid parameter\r\n" );
		return;
	}
	state = MAIL_FROM;
	
	
	if( have_tr )
		context.reset();
	else
		have_tr = true;
	
    context.from() = from;
	send( "250 "+ from.to_string() +" Sender ok\r\n" );
	
}
void SmtpRequestHandler::rcpt( const string& line )
{
	if( state < MAIL_FROM )
	{
		send( "503 Bad Sequence\r\n" ); 
		return;
	}

	vector<string> pairs = split( line );
	if( pairs.size() < 3 )
	{
		send( "501 MAIL FROM command requires at least one parameter\r\n" );
		return;
	}

	EmailAddress to( pairs[2] );
	if( !to.isvalid() )
	{
		send( "501 Invalid parameter\r\n" );
		return;
	}
	state = RCPT_TO;

    context.add_rcpt( to );
	send( "250 "+ to.to_string() +" Recipient ok\r\n" );
}
void SmtpRequestHandler::data( const string& line )
{
	if( line == "." )
	{
		//transmission end save immediately and reset;
		bool queued = true;
		
		//queuing...


		//resetting...
		state = HELO;
		have_tr = false;
		context.reset();
		
		datahandler = std::bind( &SmtpRequestHandler::processor, this);
		
		if( !queued )
		{		
			send( "500 Unexpected Failure.\r\n" );
			return;
		}
		send( "250 Queued mail for delivery.\r\n" );
		
	}
	
	if( line == ".." )
		context.removedot();
}
void SmtpRequestHandler::quit()
{
	service_available = false;
	send( "221 Service closing transmission channel\r\n" );
}
void SmtpRequestHandler::rset()
{
	state = HELO;
	have_tr = false;
	context.reset();
}


void SmtpRequestHandler::perform()
{
	respond();
}
void SmtpRequestHandler::send( const string& data )
{
	_incoming->sendmsg( data );
}
//
//NONE					= 0,
//SYNTAX_ERROR			= 500,
//SYNTAX_ERROR_IN_PARAMS	= 501,
//NOT_IMPLEMENTED			= 502,
//BAD_SEQUENCE			= 503,
//UNIMPLEMENTED_CMD_PARAM	= 504,
//
//SYSTEM_STATUS			= 211,
//HELP_MESSAGE			= 214,
//
//SERVICE_READY			= 220,
//CLOSING_CHANNEL			= 221,
//SERVICE_UNAVAILABLE		= 421,
//ACTION_OKAY				= 250,
//FORWARDING_TO			= 251,
//CNNT_VRFY_BUT_ACCEPTING	= 252,
//MAILBOX_UNAVAILABLE		= 450,
//MAILBOX_REJECTED		= 550,
//ACTION_ABORTED			= 451,
//FORWARD_TO				= 551,
//INSUFFICIENT_STORAGE	= 452,
//EXCEEDED_STORAGE		= 552,
//MAILBOX_NOT_FOUND		= 553,
//START_MAIL				= 354,
//TRANSACTION_FAILED		= 554,
//
//AUTHENTICATION_SUCCESS	= 235,
//AUTHENTICATION_UNSUCCESS= 535,
//AUTHENTICATION_REQUIRED = 530
