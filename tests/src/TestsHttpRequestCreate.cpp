#include "catch_amalgamated.hpp"

#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>

/// simple get
TEST_CASE( "HttpRequest::Create1", "[class][ParserHttpRequest][get]" )
{
	std::ifstream file;

	file.open("./http_requests/req1", std::ifstream::in);
	HttpRequest req = HttpRequest::create(file);

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

	CHECK( req.getContent() == "Test");

}

/// simple post
TEST_CASE( "HttpRequest::Create2", "[class][ParserHttpRequest][post]" )
{
	std::ifstream file;

	file.open("./http_requests/req2", std::ifstream::in);
	HttpRequest req = HttpRequest::create(file);

	CHECK( req.getMethod() == "POST" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 9);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.10");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "969ffb76-2d3a-42cd-9b1b-701aca68ecdf");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "9");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");

	CHECK( req.getContent() == "test test");

}

// two get in a row with payload
TEST_CASE( "HttpRequest::Create3", "[class][ParserHttpRequest][get][two]" )
{
	std::ifstream file;

	file.open("./http_requests/two_requests", std::ifstream::in);
	HttpRequest req = HttpRequest::create(file);

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 9);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "ea45c23e-da12-465a-808b-fa9de79bd675");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "14");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQACCRAQT=MOOMNKOCMFKECOHGBEDGOEDP");


	CHECK( req.getContent() == "hi\nhow\nare\nu ?");

	req = HttpRequest::create(file);

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/");

	CHECK( req.getHeaders().size() == 8);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "533c1ccd-2255-444a-88df-a2c82b126eee");
	CHECK( req.getHeader("Host")		== "google.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "23");


	CHECK( req.getContent() == "Well and u ?\nWtf man !?");


}

/// two get in a row without payload
TEST_CASE( "HttpRequest::Create4", "[class][ParserHttpRequest][get][two]" )
{
	std::ifstream file;

	file.open("./http_requests/two_requests_no_payload", std::ifstream::in);
	HttpRequest req = HttpRequest::create(file);

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 8);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "ea45c23e-da12-465a-808b-fa9de79bd675");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQACCRAQT=MOOMNKOCMFKECOHGBEDGOEDP");


	CHECK( req.getContent().empty());

	req = HttpRequest::create(file);

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/");

	CHECK( req.getHeaders().size() == 7);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "533c1ccd-2255-444a-88df-a2c82b126eee");
	CHECK( req.getHeader("Host")		== "google.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");

	CHECK( req.getContent().empty());
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

TEST_CASE( "HttpRequest::read1", "[class][ParserHttpRequest][get]" )
{
	std::ifstream file;

	file.open("./http_requests/req1", std::ifstream::in);
	HttpRequest req;
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	req.read(str.c_str());

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

	CHECK( req.getContent() == "Test");

}

// simple post
TEST_CASE( "HttpRequest::read2", "[class][ParserHttpRequest][post]" )
{
	std::ifstream file;

	file.open("./http_requests/req2", std::ifstream::in);
	HttpRequest req;
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	req.read(str.c_str());

	CHECK( req.getMethod() == "POST" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 9);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.10");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "969ffb76-2d3a-42cd-9b1b-701aca68ecdf");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "9");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");

	CHECK( req.getContent() == "test test");

}

// two get in a row with payload
TEST_CASE( "HttpRequest::read3", "[class][ParserHttpRequest][get][two]" )
{
	std::ifstream file;

	file.open("./http_requests/two_requests", std::ifstream::in);
	HttpRequest req;
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	req.read(str.c_str());
	
	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 9);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "ea45c23e-da12-465a-808b-fa9de79bd675");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "14");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQACCRAQT=MOOMNKOCMFKECOHGBEDGOEDP");


	CHECK( req.getContent() == "hi\nhow\nare\nu ?");
	CHECK( req.isComplete() );

	req.clear();
	CHECK( req.isComplete() == false);

	// req = HttpRequest::create(file);
	req.read("");

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/");

	CHECK( req.getHeaders().size() == 8);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "533c1ccd-2255-444a-88df-a2c82b126eee");
	CHECK( req.getHeader("Host")		== "google.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Content-Length")		== "23");


	CHECK( req.getContent() == "Well and u ?\nWtf man !?");


}

/// two get in a row without payload
TEST_CASE( "HttpRequest::read4", "[class][ParserHttpRequest][get][two]" )
{
	std::ifstream file;

	file.open("./http_requests/two_requests_no_payload", std::ifstream::in);
	HttpRequest req;
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	req.read(str.c_str());
	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/getip");

	CHECK( req.getHeaders().size() == 8);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "ea45c23e-da12-465a-808b-fa9de79bd675");
	CHECK( req.getHeader("Host")		== "dynamicdns.park-your-domain.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");
	CHECK( req.getHeader("Cookie")		== "ASPSESSIONIDQACCRAQT=MOOMNKOCMFKECOHGBEDGOEDP");


	CHECK( req.getContent().empty());
	CHECK( req.isComplete() );

	req.clear();
	CHECK( req.isComplete() == false);

	// req = HttpRequest::create(file);
	req.read(str.c_str());

	CHECK( req.getMethod() == "GET" );
	CHECK( req.getUri().getPathEtc() == "/");

	CHECK( req.getHeaders().size() == 7);
	
	CHECK( req.getHeader("Content-Type")		== "text/plain");
	CHECK( req.getHeader("User-Agent")		== "PostmanRuntime/7.26.8");
	CHECK( req.getHeader("Accept")		== "*/*");
	CHECK( req.getHeader("Postman-Token")		== "533c1ccd-2255-444a-88df-a2c82b126eee");
	CHECK( req.getHeader("Host")		== "google.com");
	CHECK( req.getHeader("Accept-Encoding")		== "gzip, deflate, br");
	CHECK( req.getHeader("Connection")		== "keep-alive");

	CHECK( req.getContent().empty());
}
