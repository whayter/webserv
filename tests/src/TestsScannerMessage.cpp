#include "catch_amalgamated.hpp"

#include "ScannerMessage2.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace parser::http;



void requireToken(ScannerMessage2 &scanner, TokenKind kind, const std::string &value, bool skipLWS = true)
{
	Token t;

	t = scanner.getToken(skipLWS);
	REQUIRE( t.value == value);
	REQUIRE( t.kind == kind);
}

void requireCRNL(ScannerMessage2 &scanner)
{
	requireToken(scanner, TokenKind::kCarriage, "\r");
	requireToken(scanner, TokenKind::kNewLine, "\n");
}

void requireHeaderCRNL(ScannerMessage2 &scanner,const std::string &name, const std::string &value)
{
	requireToken(scanner, TokenKind::kString, name);
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, value);
	requireCRNL(scanner);	
}


TEST_CASE( "ScannerMessage ", "[class][ScannerMessage2]" )
{
	std::ifstream file;

	file.open("./http_requests/simple_get", std::ifstream::in);
	std::vector<unsigned char> vec((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	ScannerMessage2 scanner(vec);
	Token t;

	requireToken(scanner, TokenKind::kString, "GET");
	requireToken(scanner, TokenKind::kString, "/getip");
	requireToken(scanner, TokenKind::kString, "HTTP/1.1");
	requireCRNL(scanner);
	
	requireHeaderCRNL(scanner, "User-Agent", "PostmanRuntime/7.26.10");
	requireHeaderCRNL(scanner, "Accept", "*/*");
	requireHeaderCRNL(scanner, "Postman-Token", "ec250329-5eb0-4d4b-8150-39f294b6aea2");
	requireHeaderCRNL(scanner, "Host", "dynamicdns.park-your-domain.com:8080");
	requireHeaderCRNL(scanner, "Accept-Encoding", "gzip, deflate, br");
	requireHeaderCRNL(scanner, "Connection", "keep-alive");
	requireHeaderCRNL(scanner, "Content-Length", "4");
	requireHeaderCRNL(scanner, "Cookie", "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");

	
	requireCRNL(scanner);
	



	// t = scanner.getToken();
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "GET");

	// t = scanner.getToken();
	// CHECK( t.kind == TokenKind::kLWS);
	// CHECK( t.value == " ");

	// t = scanner.getToken();
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "/getip");
	
	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "HTTP/1.1");
	
	// requireCRNL(scanner);

	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "PostmanRuntime/7.26.10");
	
	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "Accept");
	
	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "*/*");
	
	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "");
	
	// t = scanner.getToken(true);
	// CHECK( t.kind == TokenKind::kString);
	// CHECK( t.value == "");
	




}
