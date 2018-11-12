#include "pch.h"

const char* enc64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char dec64[] = {
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 73,
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,255,255,255,
  255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
  255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255 
};


bool b64enc( const string& input, string& output )
{
	size_t ord = 0;
	const size_t in_len = input.size();
	
	output.reserve( 4*((in_len+2)/3) + 1 );
	
	while( ord < in_len )
	{
		register char a, b;

		a = input[ ord++ ];

		output += enc64[ (a >> 2) & 0x3f ];
		
		if( ord == in_len )
		{
			output += enc64[ (a << 4 ) & 0x30 ];
			output += "==";
			return true;
		}

		b = input[ ord++ ];

		output += enc64[(( a << 4 ) | ((b >> 4) /*& 0xF*/)) & 0x3f];

		
		if( ord == in_len )
		{
			output += enc64[ (b << 2) & 0x3f ];
			output += '=';
			return true;
		}

		a = input[ ord++ ];

		output += enc64[ ((( b << 2 ) & 0x3f ) | ((a >> 6)& 0x3)) & 0x3f ];

		output += enc64[ a & 0x3f ];
	}

	return true;
}

char* b64enc( char* input )
{
	unsigned int in_len = strlen( input );
	char* output = new char[4*((in_len+2)/3)+1];
	char* p = output;

	while( in_len-- > 0 )
	{
		register char a, b;

		a = *input++;

		*p++ = enc64[ (a >> 2) & 0x3f ];
		
		if( in_len-- <= 0 )
		{
			*p++ = enc64[ (a << 4 ) & 0x30 ];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		b = *input++;

		*p++ = enc64[(( a << 4 ) | ((b >> 4) )) & 0x3f];
		
		if( in_len-- <= 0 )
		{
			*p++ = enc64[ (b << 2) & 0x3f ];
			*p++ = '=';
			break;
		}

		a = *input++;

		*p++ = enc64[ ((( b << 2 ) & 0x3f ) | ((a >> 6)& 0x3)) & 0x3f ];

		*p++ = enc64[ a & 0x3f ];
	}
	*p = 0;

	return output;
}
bool b64dec( char* input, char** output)
{
	unsigned int in_len = (unsigned int)strlen(input);
	if( in_len < 4 || in_len % 4 != 0 )
	{
		//This is not a valid base64 string
		return false;
	}
	
	char* p = new char[ 3*((int)strlen(input)/4)+1 ];
	*output = p;


	char a, b;

	unsigned int cycle = in_len/4;
	while( cycle-- )
	{
		
		a = *input++;
		b = *input++;

		if( a > 127 || (a = dec64[a]) == 255 || (b = dec64[b]) == 255 )
			return false;

		*p++ = (a << 2) | (b >> 4);

		a = *input++;

		if( a == '=' )
		{		
			if( *input == '=' )
				break;
			else
				return false;
		}

		if( a > 127 || (a = dec64[a]) == 255 )
			return false;
		*p++ = (b << 4) | (a >> 2);

		b = *input++;
		if( b == '=' )
			break;
		
		if (b > 127 || (b = dec64[b]) == 255) 
			return false;
		
		*p++ = (a << 6) | b;
	}

	return true;
}