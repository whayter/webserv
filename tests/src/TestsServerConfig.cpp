#include "catch.hpp"

#include "SingletonFixture.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

TEST_CASE_METHOD(SingletonFixture, "ServerConfig1 - ./config_files/testParser.conf", "[class][ServerConfig][testParser]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testParser.conf");


	// listen directive
	CHECK( &config == &ServerConfig::getInstance("testing singleton identity, must not override first call"));
	CHECK( config.getServers().size() == 2);

	CHECK( config.getServer(0).getListen(0).getHostname().empty() );
	CHECK( config.getServer(0).getListen(0).getPort() == 80 );

	// CHECK( config.getServer(0).getListen(1).getHostname() == "127.0.0.1" ); // not parsing address anymore
	CHECK( config.getServer(0).getListen(1).getPort() == 81 );

	CHECK( config.getServer(1).getListen(0).getHostname().empty() );
	CHECK( config.getServer(1).getListen(0).getPort() == 82 );
	
	CHECK( config.getServer(1).getListen(1).getHostname().empty() );
	CHECK( config.getServer(1).getListen(1).getPort() == 83 );

	// location directive
	CHECK( config.getServer(0).getLocations()[0].getUri() == "/" );
	CHECK( config.getServer(0).getLocations()[1].getUri() == "/media" );
	CHECK( config.getServer(0).getLocations()[2].getUri() == ".php$" );

	CHECK( config.getServer(1).getLocations()[0].getUri() == "/" );
	CHECK( config.getServer(1).getLocations()[1].getUri() == "\\.rb$" );

	//root directive
	CHECK( config.getServer(0).getRoot() == "/var/www/app" );
	CHECK( config.getServer(0).getLocations()[0].getRoot().empty() );
	CHECK( config.getServer(0).getLocations()[1].getRoot()  == "/var/www/media" );
	CHECK( config.getServer(0).getLocations()[2].getRoot().empty() );
	
	CHECK( config.getServer(1).getRoot() == "/var/www/app" );
	CHECK( config.getServer(1).getLocations()[0].getRoot().empty() );
	CHECK( config.getServer(1).getLocations()[1].getRoot().empty() );

	//index directive
	CHECK( config.getServer(0).getIndex() == "server1.php" );
	CHECK( config.getServer(0).getLocations()[0].getIndex().empty());
	CHECK( config.getServer(1).getIndex() == "server2.php" );
	CHECK( config.getServer(1).getLocations()[0].getIndex() == "server2Location.php");

	//error directive
	CHECK( config.getServer(0).getErrors().size() == 4);

	CHECK( config.getServer(0).getErrors()[497] == "https://$host:443$request_uri");
	CHECK( config.getServer(0).getErrors()[500] == "50x.html");
	CHECK( config.getServer(0).getErrors()[501] == "50x.html");
	CHECK( config.getServer(0).getErrors()[502] == "50x.html");
	
	CHECK( config.getServer(1).getErrors().size() == 1);
	CHECK( config.getServer(1).getErrors()[497] == "https://$host:443$request_uri");

	// autoindex directive
	CHECK( config.getServer(0).getAutoindex() == false );
	CHECK( config.getServer(0).getLocations()[0].getAutoindex() == true);
	CHECK( config.getServer(0).getLocations()[1].getAutoindex() == false);
	CHECK( config.getServer(0).getLocations()[2].getAutoindex() == true);

	CHECK( config.getServer(1).getAutoindex() == true );
	CHECK( config.getServer(1).getLocations()[0].getAutoindex() == true);
	CHECK( config.getServer(1).getLocations()[1].getAutoindex() == false);

	// // fastcgi_pass directive
	// CHECK( config.getServer(0).getLocations()[2].getFastCgiPass().getHostname() == "wordpress");
	// CHECK( config.getServer(0).getLocations()[2].getFastCgiPass().getPort() == 9000);

	// CHECK( config.getServer(1).getLocations()[1].getFastCgiPass().getHostname() == "rails");
	// CHECK( config.getServer(1).getLocations()[1].getFastCgiPass().getPort() == 3000);

	// // fastcgi_param directive
	// CHECK( config.getServer(0).getLocations()[0].getFastCgiParams().size() == 0);
	// CHECK( config.getServer(0).getLocations()[1].getFastCgiParams().size() == 0);
	// CHECK( config.getServer(0).getLocations()[2].getFastCgiParams().size() == 1);
	// CHECK( config.getServer(0).getLocations()[2].getFastCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");

	// CHECK( config.getServer(1).getLocations()[0].getFastCgiParams().size() == 0);
	// CHECK( config.getServer(1).getLocations()[1].getFastCgiParams().size() == 2);
	// CHECK( config.getServer(1).getLocations()[1].getFastCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");
	// CHECK( config.getServer(1).getLocations()[1].getFastCgiParam("SERVER_NAME") == "$server_name");

	// cgi_exec directive
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getUri() == "php-cgi");

	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getUri() == "ruby-cgi");

	// cgi_param directive
	// CHECK( config.getServer(0).getLocations()[0].getCgiParams().size() == 0);
	// CHECK( config.getServer(0).getLocations()[1].getCgiParams().size() == 0);
	// CHECK( config.getServer(0).getLocations()[2].getCgiParams().size() == 1);
	// CHECK( config.getServer(0).getLocations()[2].getCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");

	// CHECK( config.getServer(1).getLocations()[0].getCgiParams().size() == 0);
	// CHECK( config.getServer(1).getLocations()[1].getCgiParams().size() == 2);
	// CHECK( config.getServer(1).getLocations()[1].getCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");
	// CHECK( config.getServer(1).getLocations()[1].getCgiParam("SERVER_NAME") == "$server_name");

	// client_max_body_size directive
	CHECK( config.getServer(0).getClientMaxBodySize() == (1000 * 1000) );
	CHECK( config.getServer(0).getLocations()[0].getClientMaxBodySize() == 1000);
	CHECK( config.getServer(0).getLocations()[1].getClientMaxBodySize() == 1024);
	CHECK( config.getServer(0).getLocations()[2].getClientMaxBodySize() == 1000 * 1000);


	CHECK( config.getServer(1).getClientMaxBodySize() == 2 * (1000 * 1000) );
	CHECK( config.getServer(1).getLocations()[0].getClientMaxBodySize() == 2 * (1000 * 1000));
	CHECK( config.getServer(1).getLocations()[1].getClientMaxBodySize() == 1024 * 1024);

	//  upload_store directive
	CHECK( config.getServer(0).getUploadStore() == "./www/upload");
	CHECK( config.getServer(1).getUploadStore().empty());

}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig2 - ./config_files/testParser_directive_return.conf",
	"[class][ServerConfig][directive][return]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testParser_directive_return.conf");

	CHECK( config.getServers().size() == 2);

	//server 0
	REQUIRE( config.getServer(0).getLocations().size() == 7);


	CHECK( config.getServer(0).getLocations()[0].getUri() == "/" ) ;
	CHECK( config.getServer(0).getLocations()[0].hasReturnDirective() == false );
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().hasCode() == false);
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().hasUri() == false);
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().getCode() == 0);
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[0].getReturnDirective().getUri().empty());
	
	CHECK( config.getServer(0).getLocations()[1].getUri() == "/stack" ) ;
	CHECK( config.getServer(0).getLocations()[1].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().getCode() == 302);
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().getUri().toString() == "https://stackoverflow.com");

	CHECK( config.getServer(0).getLocations()[2].getUri() == "/youtube" ) ;
	CHECK( config.getServer(0).getLocations()[2].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getCode() == 303);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getUri().toString() == "https://youtube.fr");

	CHECK( config.getServer(0).getLocations()[3].getUri() == "/intra" ) ;
	CHECK( config.getServer(0).getLocations()[3].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getCode() == 307);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getUri().toString() == "https://intra.42.fr");

	CHECK( config.getServer(0).getLocations()[4].getUri() == "/nginx" ) ;
	CHECK( config.getServer(0).getLocations()[4].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getCode() == 308);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getUri().toString() == "http://nginx.org");

	CHECK( config.getServer(0).getLocations()[5].getUri() == "/bird" ) ;
	CHECK( config.getServer(0).getLocations()[5].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasUri() == false);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getCode() == 204);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getText() == "");
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getUri().empty());

	CHECK( config.getServer(0).getLocations()[6].getUri() == "/bird2" ) ;
	CHECK( config.getServer(0).getLocations()[6].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().hasText() == true);
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().hasUri() == false);
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().getCode() == 500);
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().getText() == "data_returned_quotes?");
	CHECK( config.getServer(0).getLocations()[6].getReturnDirective().getUri().empty());
	
	
	// server 1

	REQUIRE( config.getServer(1).getLocations().size() == 3);

	CHECK( config.getServer(1).getLocations()[0].getUri() == "/" ) ;
	CHECK( config.getServer(1).getLocations()[0].hasReturnDirective() == true );
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getCode() == 302);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getUri().toString() == "stack");

	CHECK( config.getServer(1).getLocations()[1].getUri() == "/stack" ) ;
	CHECK( config.getServer(1).getLocations()[1].hasReturnDirective() == true );
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getCode() == 302);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getUri().toString() == "https://stackoverflow.com");

	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).getLocations()[2].hasReturnDirective() == false);
	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().hasCode() == false);
	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().hasUri() == false);
	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().getCode() == 0);
	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getLocations()[2].getReturnDirective().getUri().empty());
}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig3 - ./config_files/testFindServer.conf",
	"[class][ServerConfig][findServer]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testFindServer.conf");

	CHECK(config.getServer(0).getServerName() == "srv_one");
	CHECK(config.getServer(1).getServerName() == "srv_two");
	CHECK(config.getServer(2).getServerName() == "srv_three");
	CHECK(config.getServer(3).getServerName() == "");
	CHECK(config.getServer(4).getServerName() == "srv_four");
	REQUIRE(config.getServer(5).getServerName() == "srv_five");

	REQUIRE(config.getServers().size() == 6);

	CHECK( config.findServer(Uri("http://localhost")).getServerName() == "srv_one");
	CHECK( config.findServer(Uri("http://srv_one:80")).getServerName() ==   "srv_one" );
	CHECK( config.findServer(Uri("http://srv_two:80")).getServerName() ==   "srv_two" );
	CHECK( config.findServer(Uri("http://srv_three:81")).getServerName() == "srv_three" );
	CHECK( config.findServer(Uri("http://localhost:81")).getServerName() == "srv_three" );
	CHECK( config.findServer(Uri("http://srv_four:81")).getServerName() ==  "srv_four" );

	Uri uri;
	uri.setPort(80);
	CHECK( config.findServer(uri).getIndex() == config.getServer(3).getIndex() );

	Uri uriEmptyHost;
	uriEmptyHost.setPort(80);
	CHECK( &config.findServer(uriEmptyHost) == &config.getServer(3) );
	uriEmptyHost.setPort(81);
	CHECK( &config.findServer(uriEmptyHost) == &config.getServer(2) );
	uriEmptyHost.setPort(82);
	CHECK( &config.findServer(uriEmptyHost) == &config.getServer(5) );

	uriEmptyHost.setPort(80);
	CHECK( config.findServer(uriEmptyHost).getIndex() == config.getServer(3).getIndex() );
	uriEmptyHost.setPort(81);
	CHECK( config.findServer(uriEmptyHost).getIndex() == config.getServer(2).getIndex() );
	uriEmptyHost.setPort(82);
	CHECK( config.findServer(uriEmptyHost).getIndex() == config.getServer(5).getIndex() );

}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig4 - ./config_files/testParser_directive_limit_except.conf",
	"[class][ServerConfig][limit_except]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testParser_directive_limit_except.conf");

	// Location /
	CHECK( config.getServer(0).getLocations()[0].getLimitExceptMethods().size() == 1);
	CHECK( config.getServer(0).getLocations()[0].hasLimitExceptMethod("GET") == true);

	// Location /youtube
	CHECK( config.getServer(0).getLocations()[1].getLimitExceptMethods().size() == 2);
	CHECK( config.getServer(0).getLocations()[1].hasLimitExceptMethod("GET") == true);
	CHECK( config.getServer(0).getLocations()[1].hasLimitExceptMethod("POST") == true);
	CHECK( config.getServer(0).getLocations()[1].hasLimitExceptMethod("DELETE") == false);
	CHECK( config.getServer(0).getLocations()[1].hasLimitExceptMethod("RANDOM") == false);
	
	// Location /intra
	CHECK( config.getServer(0).getLocations()[2].getLimitExceptMethods().size() == 2);
	CHECK( config.getServer(0).getLocations()[2].hasLimitExceptMethod("GET") == false);
	CHECK( config.getServer(0).getLocations()[2].hasLimitExceptMethod("POST") == true);
	CHECK( config.getServer(0).getLocations()[2].hasLimitExceptMethod("DELETE") == true);
	CHECK( config.getServer(0).getLocations()[2].hasLimitExceptMethod("Nop") == false);

	// Location /nginx
	CHECK( config.getServer(0).getLocations()[3].getLimitExceptMethods().size() == 1);
	CHECK( config.getServer(0).getLocations()[3].hasLimitExceptMethod("GET") == true);
}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig3 - ./config_files/testFindLocation.conf",
	"[class][ServerConfig][findLocation]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testFindLocation.conf");
	(void)config;

	ServerBlock srv = config.getServer(0);
	(void)srv;

	REQUIRE( srv.getLocations()[1].getReturnDirective().getUri() == "/");
	CHECK( srv.findLocation("/youtube/test/").getReturnDirective().getUri()		== srv.getLocations()[4].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/youtube/test/nothing").getReturnDirective().getUri()== srv.getLocations()[4].getReturnDirective().getUri() );
	

	CHECK( srv.findLocation("/youtube/random").getReturnDirective().getUri() 	== srv.getLocations()[3].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/noMatch").getReturnDirective().getUri() == srv.getLocations()[1].getReturnDirective().getUri() );

	CHECK( srv.findLocation("/youtube").getReturnDirective().getUri() 			== srv.getLocations()[2].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/youtube/").getReturnDirective().getUri() 			== srv.getLocations()[3].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/youtube/test").getReturnDirective().getUri() 		== srv.getLocations()[4].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/youtube/test.php").getReturnDirective().getUri() 	== srv.getLocations()[6].getReturnDirective().getUri() );
	CHECK( srv.findLocation("/youtube/test.rb").getReturnDirective().getUri() 	== srv.getLocations()[7].getReturnDirective().getUri() );


}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig4 - ./config_files/testFindLocation.conf",
	"[class][ServerConfig][getPathFromUri]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testFindLocation.conf");
	(void)config;

	ServerBlock srv = config.getServer(0);

	REQUIRE( srv.getLocations()[1].getReturnDirective().getUri() == "/");

	CHECK( srv.getPathFromUri("/").second	== "./www/index.html");
	CHECK( srv.getPathFromUri("/youtube").second	== "../tests/www/youtube/youtube.html");
	CHECK( srv.getPathFromUri("/youtube/").second	== "./www/youtube/youtube2.html");
	CHECK( srv.getPathFromUri("/youtube/demo.php").second	== "./www/youtube/demo.php");
	CHECK( srv.getPathFromUri("/youtube/nothing").second	== "./www/youtube/nothing");

	CHECK( srv.getPathFromUri("/youtube/test").second	== "./www/youtube/test/youtubeTest.html");
	CHECK( srv.getPathFromUri("/youtube/test/").second	== "./www/youtube/test/youtubeTest.html");


	// same as previous, but from config
	CHECK( config.getPathFromUri("/")	== "./www/index.html");
	CHECK( config.getPathFromUri("/youtube")	== "../tests/www/youtube/youtube.html");
	CHECK( config.getPathFromUri("/youtube/")	== "./www/youtube/youtube2.html");
	CHECK( config.getPathFromUri("/youtube/demo.php")	== "./www/youtube/demo.php");
	CHECK( config.getPathFromUri("/youtube/nothing")	== "./www/youtube/nothing");

	CHECK( config.getPathFromUri("/youtube/test")	== "./www/youtube/test/youtubeTest.html");
	CHECK( config.getPathFromUri("/youtube/test/")	== "./www/youtube/test/youtubeTest.html");

}

TEST_CASE_METHOD(SingletonFixture, "ServerConfig::getMime", "[class][ServerConfig][getMime]" )
{
	ServerConfig& config = SingletonFixture::SetUpFile("./config_files/testFindLocation.conf", "./../mime.types");
	(void)config;

	CHECK( config.getMime("png") == "image/png");
	CHECK( config.getMime("gif") == "image/gif");
	CHECK( config.getMime("ico") == "image/vnd.microsoft.icon");

	CHECK( config.getMime("jpeg") == "image/jpeg");
	CHECK( config.getMime("jpg") == "image/jpeg");
	CHECK( config.getMime("jpe") == "image/jpeg");
	CHECK( config.getMime("jfif") == "image/jpeg");

	CHECK( config.getMime("json") == "application/json");

	CHECK( config.getMime("html") == "text/html");
	CHECK( config.getMime("htm") == "text/html");
	CHECK( config.getMime("shtml") == "text/html");

	CHECK( config.getMime("css") == "text/css");
	


}