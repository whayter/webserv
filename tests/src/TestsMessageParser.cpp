#include "catch.hpp"

#include "messageParser.hpp"
#include "ScannerBuffer.hpp"
#include "http/Request.hpp"
#include "http/Status.hpp"
#include "ScannerMessage.hpp"
#include "SingletonFixture.hpp"


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



TEST_CASE( "http::hasTwoConsecutiveCRNL", "[namespace][http][hasTwoConsecutiveCRNL]" )
{
	bool containIAC;

	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("test"), containIAC));
	REQUIRE(!containIAC);
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("test\r\n\r "), containIAC));
	REQUIRE(!containIAC);
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("test\r\ntest\r\n"),containIAC));
	REQUIRE(!containIAC);
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("test\r\ntest\r\ntest\r\n"),containIAC));
	REQUIRE(!containIAC);

	CHECK(http::hasTwoConsecutiveCRNL(vectorFromStr("test\r\n\r\n"),containIAC));
	REQUIRE(!containIAC);
	CHECK(http::hasTwoConsecutiveCRNL(vectorFromStr("test\r\ntest\r\n\r\ntest\r\n"), containIAC));
	REQUIRE(!containIAC);

	// check for IAC
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("\xff"), containIAC));
	REQUIRE(containIAC);
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("\r\ne\r\n\xff"), containIAC));
	REQUIRE(containIAC);
	CHECK(!http::hasTwoConsecutiveCRNL(vectorFromStr("test\xff\xf4 \r\ntest\r\n\r\ntest\r\n"), containIAC));
	REQUIRE(containIAC);

	CHECK(http::hasTwoConsecutiveCRNL(vectorFromStr("\r\n\r\n\xff"), containIAC));
	REQUIRE(!containIAC);
	CHECK(http::hasTwoConsecutiveCRNL(vectorFromStr("hi\r\n\r\nhi\xff"), containIAC));
	REQUIRE(!containIAC);

	// std::ifstream file;
	// file.open("./http_requests/simple_get", std::ifstream::in);
	// std::vector<unsigned char> vec((std::istreambuf_iterator<char>(file)),
    //              std::istreambuf_iterator<char>());

	// CHECK(http::hasTwoConsecutiveCRNL(vec, containEndOfInput));
	// REQUIRE(!containEndOfInput);
}

TEST_CASE( "ScannerBuffer2 - test vite fait", "[ScannerBuffer2]" )
{
	std::vector<unsigned char> vec = vectorFromStr("test");
	ft::scanner::ScannerBuffer scan(vec);

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
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - simple get",
	"[namespace][http][parseRequest][get][simple]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/simple_get", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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
	
	std::string body = "Test";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );
}

// simple post
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - simple post",
	"[namespace][http][parseRequest][post]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/simple_post", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);


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

	std::string body = "test test";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );
}


// two get in a row with payload
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - two get in a row",
	"[namespace][http][parseRequest][two][get][payload]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/two_requests", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	REQUIRE (req.empty());
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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

	std::string body = "hi\nhow\nare\nu ?";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );

	req.clear();
	REQUIRE (req.empty());
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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

	body = "Well and u ?\nWtf man !?";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );
}

/// two get in a row without payload
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - two get in a row no payload",
	"[namespace][http][parseRequest][two][get][no_payload]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/two_requests_no_payload", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	REQUIRE (req.empty());
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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


	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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

// simple get cut in half
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - simple get cut in half",
	"[namespace][http][parseRequest][simple][get][cut_half]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/simple_get", std::ifstream::in);
	std::string data((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	
	size_t idx = 75;

	std::vector<unsigned char> one = vectorFromStr(data.substr(0,idx));
	std::vector<unsigned char> two = vectorFromStr(data.substr(idx));

	std::vector<unsigned char> buffer;
	buffer.insert(buffer.end(), one.begin(), one.end());

	REQUIRE( !http::parseRequest(req, error, one) );
	REQUIRE( error == http::Status::None);
	buffer.insert(buffer.end(), two.begin(), two.end());
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

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

	std::string body = "Test";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );


	req.clear();
	CHECK( req.getMethod().empty() );
	CHECK( req.getContentLength() == 0 );
	CHECK( req.getUri().empty());

	CHECK( req.getHeaders().size() == 0);
	CHECK( req.getContent().empty());

}

// simple get cut in half loop
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - simple get cut in half loop",
	"[namespace][http][parseRequest][simple][get][cut_half][loop]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/simple_get", std::ifstream::in);
	std::string data((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;

	for (size_t idx = 0; idx < data.size(); idx++) {
        DYNAMIC_SECTION( "Looped section nb: " << idx)
		{
			std::vector<unsigned char> one = vectorFromStr(data.substr(0,idx));
			std::vector<unsigned char> two = vectorFromStr(data.substr(idx));
			std::vector<unsigned char> buffer;
			buffer.insert(buffer.end(), one.begin(), one.end());

			REQUIRE( !http::parseRequest(req, error, one) );
			REQUIRE( error == http::Status::None);
			buffer.insert(buffer.end(), two.begin(), two.end());
			REQUIRE( http::parseRequest(req, error, buffer) );
			REQUIRE( error == http::Status::None);

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
    }

}

// test for cgi response
TEST_CASE( "http::parseHeaders cgi", "[namespace][http][parseHeaders][cgi]" )
{
	std::ifstream file;
	file.open("./http_requests/cgi_response", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	parser::http::ScannerMessage scan(buffer);
	http::Message responseCgi;
	http::Status error; // useless
	http::parseHeaders(scan, responseCgi, error);

	CHECK(responseCgi.getHeaders().size() == 2);
	CHECK(responseCgi.getHeader("Status") == "503 Database Unavailable");
	CHECK(responseCgi.getHeader("Content-type") == "text/html");
	CHECK(responseCgi.getContent().size() == 0);
}

TEST_CASE( "http::parseCgiResponse", "[namespace][http][parseCgiResponse]" )
{
	std::ifstream file;
	file.open("./http_requests/cgi_response", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	std::vector<unsigned char> content(buffer.begin() + 120, buffer.end());

	http::Message responseCgi = http::parseCgiResponse(buffer);
	CHECK(responseCgi.getHeaders().size() == 2);
	CHECK(responseCgi.getHeader("Status") == "503 Database Unavailable");
	CHECK(responseCgi.getHeader("Content-type") == "text/html");
	CHECK(responseCgi.getContent().size() == 96);
	CHECK(responseCgi.getContent() == content);
}
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - chunked ",
	"[namespace][http][parseRequest][chunked]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/chunked_post", std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;
	REQUIRE( http::parseRequest(req, error, buffer) );
	REQUIRE( error == http::Status::None);

	CHECK( req.getMethod() == "POST" );
	CHECK( req.getUri().toString() == "http://localhost:83/test.php");

	CHECK( req.getHeaders().size() == 4);
	
	CHECK( req.getHeader("Date")				== "Mon, 22 Mar 2004 11:15:03 GMT");
	CHECK( req.getHeader("Content-Type")		== "text/html");
	CHECK( req.getHeader("Content-Length")		== "129");
	CHECK( req.getHeader("Transfer-Encoding").empty());

	std::string body = "<html><body><p>The file you requested is 3,400bytes long and was last modified:  Sat, 20 Mar 2004 21:12:00 GMT.</p></body></html>";
	CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );
}

// chunked cut in half loop
TEST_CASE_METHOD(SingletonFixture, "http::parseRequest - chunked request cut in half loop",
	"[namespace][http][parseRequest][cut_half][chunked]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testMessageParser.conf");
	(void)config;

	std::ifstream file;
	file.open("./http_requests/chunked_post", std::ifstream::in);
	std::string data((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	http::Request req;
	http::Status error;

	for (size_t idx = 0; idx < data.size(); idx++) {
        DYNAMIC_SECTION( "Looped section nb: " << idx)
		{
			std::vector<unsigned char> one = vectorFromStr(data.substr(0,idx));
			std::vector<unsigned char> two = vectorFromStr(data.substr(idx));
			std::vector<unsigned char> buffer;
			buffer.insert(buffer.end(), one.begin(), one.end());

			CHECK( !http::parseRequest(req, error, one) );
			CHECK( error == http::Status::None);
			buffer.insert(buffer.end(), two.begin(), two.end());
			CHECK( http::parseRequest(req, error, buffer) );
			CHECK( error == http::Status::None);

			CHECK( req.getMethod() == "POST" );
			CHECK( req.getUri().toString() == "http://localhost:83/test.php");

			CHECK( req.getHeaders().size() == 4);
			
			CHECK( req.getHeader("Date")				== "Mon, 22 Mar 2004 11:15:03 GMT");
			CHECK( req.getHeader("Content-Type")		== "text/html");
			CHECK( req.getHeader("Content-Length")		== "129");
			CHECK( req.getHeader("Transfer-Encoding").empty());
			
	
			std::string body = "<html><body><p>The file you requested is 3,400bytes long and was last modified:  Sat, 20 Mar 2004 21:12:00 GMT.</p></body></html>";

			CHECK( req.getContent() == std::vector<unsigned char>(body.begin(), body.end()) );
		}
    }

}