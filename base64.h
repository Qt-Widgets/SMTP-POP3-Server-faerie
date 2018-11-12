//#ifndef BASE64_H
//#define BASE64_H
#pragma once
#include "pch.h" 

bool b64enc( const string& input, string& output );
char* b64enc( char* input );

bool b64dec( char* input, char** output);


//#endif