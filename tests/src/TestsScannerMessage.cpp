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
	std::cout << t << std::endl;
	CHECK( t.value == value);
	CHECK( t.kind == kind);
}

void requireCRLF(ScannerMessage2 &scanner)
{
	requireToken(scanner, TokenKind::kCarriage, "\r");
	requireToken(scanner, TokenKind::kNewLine, "\n");
}

void requireHeaderCRLF(ScannerMessage2 &scanner,const std::string &name, const std::string &value)
{
	requireToken(scanner, TokenKind::kString, name);
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, value);
	requireCRLF(scanner);	
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
	requireCRLF(scanner);
	
	requireHeaderCRLF(scanner, "User-Agent", "PostmanRuntime/7.26.10");
	requireHeaderCRLF(scanner, "Accept", "*/*");
	requireHeaderCRLF(scanner, "Postman-Token", "ec250329-5eb0-4d4b-8150-39f294b6aea2");
	requireHeaderCRLF(scanner, "Host", "dynamicdns.park-your-domain.com");

	requireToken(scanner, TokenKind::kString, "Host");
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, "dynamicdns.park-your-domain.com");
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, "8080");
	requireCRLF(scanner);


	requireHeaderCRLF(scanner, "Accept-Encoding", "gzip, deflate, br");
	requireHeaderCRLF(scanner, "Connection", "keep-alive");
	requireHeaderCRLF(scanner, "Content-Length", "4");
	requireHeaderCRLF(scanner, "Cookie", "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");
	requireCRLF(scanner);
	





}
