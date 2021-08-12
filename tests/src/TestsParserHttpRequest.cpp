#include "catch_amalgamated.hpp"

#include "ParserHttpRequest.hpp"
#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>

TEST_CASE( "ParserHttpRequest - Create", "[class][ParserHttpRequest]" )
{
	std::ifstream file;

	file.open("file.txt", std::ifstream::in);
	HttpRequest result = parser::http::ParserHttpRequest::create(file);

}