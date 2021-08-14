#include "catch_amalgamated.hpp"

#include "ScannerConfig.hpp"

#include <iostream>
#include <fstream>

using namespace parser;

TEST_CASE( "ScannerConfig1", "[class][ScannerConfig]" )
{
	std::ifstream file;

	file.open("./config_files/default.conf", std::ifstream::in);

	ScannerConfig scanner(file);
	Token t;

	// server {\n
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "server");
	CHECK( t.column == 1);
	CHECK( t.line == 1);
	
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kLeftBrace);
	CHECK( t.value == "{");
	CHECK( t.column == 8);
	CHECK( t.line == 1);

	// \n
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kNewLine);
	CHECK( t.value == "\n");
	CHECK( t.column == 9);
	CHECK( t.line == 1);

	// listen 0.0.0.0:80;\n
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "listen");
	CHECK( t.column == 5);
	CHECK( t.line == 2);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "0.0.0.0:80");
	CHECK( t.column == 12);
	CHECK( t.line == 2);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kComma);
	CHECK( t.value == ";");
	CHECK( t.column == 22);
	CHECK( t.line == 2);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kNewLine);
	CHECK( t.column == 23);
	CHECK( t.line == 2);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kNewLine);
	CHECK( t.column == 1);
	CHECK( t.line == 3);

	// root /var/www/app;\n
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "root");
	CHECK( t.column == 5);
	CHECK( t.line == 4);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "/var/www/app");
	CHECK( t.column == 10);
	CHECK( t.line == 4);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kComma);
	CHECK( t.value == ";");
	CHECK( t.column == 22);
	CHECK( t.line == 4);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kNewLine);
	CHECK( t.column == 23);
	CHECK( t.line == 4);

	// index index.php index.html;\n
	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "index");
	CHECK( t.column == 5);
	CHECK( t.line == 5);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "index.php");
	CHECK( t.column == 11);
	CHECK( t.line == 5);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kString);
	CHECK( t.value == "index.html");
	CHECK( t.column == 21);
	CHECK( t.line == 5);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kComma);
	CHECK( t.value == ";");
	CHECK( t.column == 31);
	CHECK( t.line == 5);

	t = scanner.getToken();
	CHECK( t.kind == TokenKind::kNewLine);
	CHECK( t.column == 32);
	CHECK( t.line == 5);


	// server_name test.org test2.org;\n
	t = scanner.getToken();
	t = scanner.getToken();
	t = scanner.getToken(); // test2.org
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n
	
	t = scanner.getToken(); // \n

	//  error_page 404 /error404.html;\n
	t = scanner.getToken(); // error_page
	t = scanner.getToken(); // 404
	t = scanner.getToken(); // /error404.html
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n

	t = scanner.getToken(); // error_page
	t = scanner.getToken(); //500

	t = scanner.getToken(); //502
	CHECK( t.value == "502");

	t = scanner.getToken(); // /custom_50x.html
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // \n

	// location / {\n
	t = scanner.getToken(); // location
	CHECK( t.value == "location");

	t = scanner.getToken(); // /
	CHECK( t.value == "/");
	
	t = scanner.getToken(); // {
	CHECK( t.value == "{");
	CHECK( t.kind == TokenKind::kLeftBrace);

	t = scanner.getToken(); // \n

	// autoindex on;\n
	t = scanner.getToken();
	CHECK( t.value == "autoindex");

	t = scanner.getToken();
	CHECK( t.value == "on");

	t = scanner.getToken();
	CHECK( t.value == ";");

	t = scanner.getToken(); // \n

	t = scanner.getToken();
	CHECK( t.value == "}");
	CHECK( t.kind == TokenKind::kRightBrace);
	CHECK( t.column == 5);
	CHECK( t.line == 13);


	t = scanner.getToken(); // \n
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // location
	t = scanner.getToken(); // ~
	CHECK( t.value == "~");
	t = scanner.getToken(); // \.php$
	CHECK( t.value == "\\.php$");
	t = scanner.getToken(); // {
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // include
	CHECK( t.value == "include");
	t = scanner.getToken(); // fastcgi_param
	CHECK( t.value == "fastcgi_params");
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // fastcgi_pass
	CHECK( t.value == "fastcgi_pass");
	t = scanner.getToken(); // wordpress:9000
	CHECK( t.value == "wordpress:9000");
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // fastcgi_param
	t = scanner.getToken(); // SCRIPT_FILENAME
	t = scanner.getToken(); // $document_root$fastcgi_script_name
	CHECK( t.value == "$document_root$fastcgi_script_name");
	t = scanner.getToken(); // ;
	t = scanner.getToken(); // \n
	t = scanner.getToken(); // }
	CHECK( t.value == "}");
	t = scanner.getToken(); // \n
	CHECK( t.value == "\n");
	t = scanner.getToken(); // \n
	CHECK( t.value == "\n");
	t = scanner.getToken(); // }
	CHECK( t.value == "}");
	t = scanner.getToken(); // End Of Input
	CHECK( t.kind == TokenKind::kEndOfInput);

}
