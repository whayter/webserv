#include "catch.hpp"

#include "ScannerMessage.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace parser::http;

static std::vector<unsigned char>	vectorFromStr(const std::string &s)
{
	std::vector<unsigned char> result;

	result.insert(result.begin(), s.begin(), s.end());
	return result;
}

void requireToken(ScannerMessage &scanner, TokenKind kind, const std::string &value, bool skipLWS = true)
{
	Token t;

	t = scanner.getToken(skipLWS);
	CHECK( t.value == value);
	CHECK( t.kind == kind);
}

void requireCRLF(ScannerMessage &scanner)
{
	requireToken(scanner, TokenKind::kCarriage, "\r");
	requireToken(scanner, TokenKind::kNewLine, "\n");
}

void requireHeaderCRLF(ScannerMessage &scanner,const std::string &name, const std::string &value)
{
	requireToken(scanner, TokenKind::kString, name);
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, value);
	requireCRLF(scanner);	
}

TEST_CASE( "ScannerMessage ", "[class][ScannerMessage]" )
{
	std::ifstream file;

	file.open("./http_requests/simple_get", std::ifstream::in);
	std::vector<unsigned char> vec((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	ScannerMessage scanner(vec);
	Token t;

	requireToken(scanner, TokenKind::kString, "GET");
	requireToken(scanner, TokenKind::kString, "/getip");
	requireToken(scanner, TokenKind::kString, "HTTP/1.1");
	requireCRLF(scanner);
	
	requireHeaderCRLF(scanner, "User-Agent", "PostmanRuntime/7.26.10");
	requireHeaderCRLF(scanner, "Accept", "*/*");
	requireHeaderCRLF(scanner, "Postman-Token", "ec250329-5eb0-4d4b-8150-39f294b6aea2");

	requireToken(scanner, TokenKind::kString, "Host");
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, "dynamicdns.park-your-domain.com");
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, "8080");
	requireCRLF(scanner);

	requireToken(scanner, TokenKind::kString, "Accept-Encoding");
	requireToken(scanner, TokenKind::kColon, ":");
	requireToken(scanner, TokenKind::kString, "gzip,");
	requireToken(scanner, TokenKind::kString, "deflate,");
	requireToken(scanner, TokenKind::kString, "br");
	requireCRLF(scanner);


	requireHeaderCRLF(scanner, "Connection", "keep-alive");
	requireHeaderCRLF(scanner, "Content-Length", "4");
	requireHeaderCRLF(scanner, "Cookie", "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");
	requireCRLF(scanner);
	
	std::vector<unsigned char> content;
	char c;
	while ((c = scanner.getChar()))
		content.push_back(c);
	
	REQUIRE( content == vectorFromStr("Test"));
}
