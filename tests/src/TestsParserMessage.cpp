#include "catch_amalgamated.hpp"

#include "parserMessage.hpp"

#include <iostream>
#include <fstream>
#include <vector>

std::vector<unsigned char>	vectorFromStr(const std::string &s)
{
	std::vector<unsigned char> result;

	result.insert(result.begin(), s.begin(), s.end());
	return result;
}

TEST_CASE( "http2::hasTwoConsecutiverCRNL", "[namespace][http2][hasTwoConsecutiverCRNL]" )
{
	CHECK(!http2::hasTwoConsecutiverCRNL(vectorFromStr("test")));
	CHECK(!http2::hasTwoConsecutiverCRNL(vectorFromStr("test\r\n\r ")));
	CHECK(!http2::hasTwoConsecutiverCRNL(vectorFromStr("test\r\ntest\r\n")));
	CHECK(!http2::hasTwoConsecutiverCRNL(vectorFromStr("test\r\ntest\r\ntest\r\n")));

	CHECK(http2::hasTwoConsecutiverCRNL(vectorFromStr("test\r\n\r\n")));
	CHECK(http2::hasTwoConsecutiverCRNL(vectorFromStr("test\r\ntest\r\n\r\ntest\r\n")));

	std::ifstream file;
	file.open("./http_requests/simple_get", std::ifstream::in);
	std::vector<unsigned char> vec((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	CHECK(http2::hasTwoConsecutiverCRNL(vec));
}

// /// simple get
// TEST_CASE( "HttpRequest::read simple get", "[class][HttpRequest][read]" )
// {
// 	std::ifstream file;

// 	file.open("./http_requests/simple_get", std::ifstream::in);
// 	HttpRequest req;
// 	std::string str((std::istreambuf_iterator<char>(file)),
//                  std::istreambuf_iterator<char>());

                 
// 	req.read(str.c_str(), str.length());

// 	CHECK( req.getMethod() == "GET" );
// 	CHECK( req.getUri().getPathEtc() == "/getip");
// 	CHECK( req.getUri().toString() == "http://dynamicdns.park-your-domain.com:8080/getip");

// 	CHECK( req.getHeaders().size() == 8);
	
// 	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.10");
// 	CHECK( req.getHeader("Accept")			== "*/*");
// 	CHECK( req.getHeader("Postman-Token")	== "ec250329-5eb0-4d4b-8150-39f294b6aea2");
// 	CHECK( req.getHeader("Host") 			== "dynamicdns.park-your-domain.com:8080");
// 	CHECK( req.getHeader("Accept-Encoding")	== "gzip, deflate, br");
// 	CHECK( req.getHeader("Connection")		== "keep-alive");
// 	CHECK( req.getHeader("Cookie")			== "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");
	
// 	std::string s("Test");
// 	std::vector<unsigned char> comp(s.begin(), s.end());
// 	CHECK( req.getContent() == comp );
// }