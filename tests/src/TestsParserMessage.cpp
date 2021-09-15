#include "catch_amalgamated.hpp"

#include "parserMessage.hpp"
#include "ScannerBuffer2.hpp"
#include "http/Request.hpp"
#include "http/Status.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

static std::vector<unsigned char>	vectorFromStr(const std::string &s)
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

TEST_CASE( "ScannerBuffer2 - test vite fait", "[ScannerBuffer2]" )
{
	std::vector<unsigned char> vec = vectorFromStr("test");
	
	ft::scanner::ScannerBuffer2 scan(vec);


	REQUIRE(scan.get() == 't');
	scan.unget();
	REQUIRE(scan.get() == 't');
	REQUIRE(scan.get() == 'e');
	scan.unget();
	scan.unget();
	REQUIRE_THROWS_AS( scan.unget(), std::out_of_range);
	REQUIRE(scan.get() == 't');
	REQUIRE(scan.get() == 'e');
	REQUIRE(scan.get() == 's');
	REQUIRE(scan.get() == 't');
	REQUIRE(scan.get() == '\0');
	REQUIRE_NOTHROW(scan.unget());
	REQUIRE(scan.get() == '\0');
	REQUIRE_THROWS_AS( scan.get(), std::out_of_range);
}


/// simple get
TEST_CASE( "parse http request simple get", "[namespace][http2][parseRequest]" )
{
	std::ifstream file;

	file.open("./http_requests/simple_get", std::ifstream::in);
	
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());


	http::Request req;
	http::Status error;
	REQUIRE( http2::parseRequest(req, error, buffer) );

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");
	CHECK( req.getUri().toString() == "http://dynamicdns.park-your-domain.com:8080/getip");

	CHECK( req.getHeaders().size() == 8);
	
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.10");
	CHECK( req.getHeader("Accept")			== "*/*");
	CHECK( req.getHeader("Postman-Token")	== "ec250329-5eb0-4d4b-8150-39f294b6aea2");
	CHECK( req.getHeader("Host") 			== "dynamicdns.park-your-domain.com:8080");
	CHECK( req.getHeader("Accept-Encoding")	== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Cookie")			== "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");
	
	std::string s("Test");
	std::vector<unsigned char> comp(s.begin(), s.end());
	CHECK( req.getContent() == comp );
}