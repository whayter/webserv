#include "catch_amalgamated.hpp"

#include "ParserHttpRequest.hpp"
#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>

TEST_CASE( "ParserHttpRequest - Create() - Get ", "[class][ParserHttpRequest][get]" )
{
	std::ifstream file;

	file.open("./http_requests/req1", std::ifstream::in);
	HttpRequest req = parser::http::ParserHttpRequest::create(file);

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 7);
	
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.10");
	CHECK( req.getHeader("Accept")			== "*/*");
	CHECK( req.getHeader("Postman-Token")	== "ec250329-5eb0-4d4b-8150-39f294b6aea2");
	CHECK( req.getHeader("Host") 			== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")	== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Cookie")			== "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");

	CHECK( req.getContent() == "Test");

}