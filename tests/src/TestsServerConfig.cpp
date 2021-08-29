#include "catch_amalgamated.hpp"

#include "ServerConfig.hpp"

#include <iostream>
#include <fstream>


TEST_CASE( "ServerConfig1 - ./config_files/testParser.conf", "[class][ServerConfig]" )
{
	ServerConfig& config = ServerConfig::getInstance("./config_files/testParser.conf");


	// listen directive
	CHECK( &config == &ServerConfig::getInstance("testing singleton identity, must not override first call"));
	CHECK( config.getServers().size() == 2);

	CHECK( config.getServer(0).getListen(0).getHostname().empty() );
	CHECK( config.getServer(0).getListen(0).getPort() == 80 );

	CHECK( config.getServer(0).getListen(1).getHostname() == "127.0.0.1" );
	CHECK( config.getServer(0).getListen(1).getPort() == 81 );

	CHECK( config.getServer(1).getListen(0).getHostname().empty() );
	CHECK( config.getServer(1).getListen(0).getPort() == 82 );
	
	CHECK( config.getServer(1).getListen(1).getHostname().empty() );
	CHECK( config.getServer(1).getListen(1).getPort() == 83 );


	CHECK( &config.findServer(80) == &config.getServer(0) );
	CHECK( &config.findServer(81) == &config.getServer(0) );
	CHECK( &config.findServer(82) == &config.getServer(1) );
	CHECK( &config.findServer(83) == &config.getServer(1) );

	// location directive
	CHECK( config.getServer(0).getLocations()[0].getUri() == "/" );
	CHECK( config.getServer(0).getLocations()[1].getUri() == "/media" );
	CHECK( config.getServer(0).getLocations()[2].getUri() == ".php$" );

	CHECK( config.getServer(1).getLocations()[0].getUri() == "/" );
	CHECK( config.getServer(1).getLocations()[1].getUri() == "\\.php$" );

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

	// fastcgi_pass directive
	CHECK( config.getServer(0).getLocations()[2].getFastCgiPass().getHostname() == "wordpress");
	CHECK( config.getServer(0).getLocations()[2].getFastCgiPass().getPort() == 9000);

	CHECK( config.getServer(1).getLocations()[1].getFastCgiPass().getHostname() == "rails");
	CHECK( config.getServer(1).getLocations()[1].getFastCgiPass().getPort() == 3000);

	// fastcgi_param directive
	CHECK( config.getServer(0).getLocations()[0].getFastCgiParams().size() == 0);
	CHECK( config.getServer(0).getLocations()[1].getFastCgiParams().size() == 0);
	CHECK( config.getServer(0).getLocations()[2].getFastCgiParams().size() == 1);
	CHECK( config.getServer(0).getLocations()[2].getFastCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");

	CHECK( config.getServer(1).getLocations()[0].getFastCgiParams().size() == 0);
	CHECK( config.getServer(1).getLocations()[1].getFastCgiParams().size() == 2);
	CHECK( config.getServer(1).getLocations()[1].getFastCgiParam("SCRIPT_FILENAME") == "$document_root$fastcgi_script_name");
	CHECK( config.getServer(1).getLocations()[1].getFastCgiParam("SERVER_NAME") == "$server_name");

	// client_max_body_size directive
	CHECK( config.getServer(0).getClientMaxBodySize() == (1000 * 1000) );
	CHECK( config.getServer(0).getLocations()[0].getClientMaxBodySize() == 1000);
	CHECK( config.getServer(0).getLocations()[1].getClientMaxBodySize() == 1024);
	CHECK( config.getServer(0).getLocations()[2].getClientMaxBodySize() == 1000 * 1000);


	CHECK( config.getServer(1).getClientMaxBodySize() == 2 * (1000 * 1000) );
	CHECK( config.getServer(1).getLocations()[0].getClientMaxBodySize() == 2 * (1000 * 1000));
	CHECK( config.getServer(1).getLocations()[1].getClientMaxBodySize() == 1024 * 1024);


}

TEST_CASE( "ServerConfig2 - ./config_files/testParser_directive_return.conf", "[class][ServerConfig][directive][return]" )
{
	ServerConfig& config = ServerConfig::getInstance("./config_files/testParser_directive_return.conf");
	(void)config;

	CHECK( config.getServers().size() == 2);
	//server 0

	CHECK( config.getServer(0).hasReturnDirective() == true);
	CHECK( config.getServer(0).getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getReturnDirective().getCode() == 301);
	CHECK( config.getServer(0).getReturnDirective().getText().empty() );
	CHECK( config.getServer(0).getReturnDirective().getUri() == "stack");

	CHECK( config.getServer(0).getLocations().size() == 6);

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
	CHECK( config.getServer(0).getLocations()[1].getReturnDirective().getUri() == "https://stackoverflow.com");

	CHECK( config.getServer(0).getLocations()[2].getUri() == "/youtube" ) ;
	CHECK( config.getServer(0).getLocations()[2].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getCode() == 303);
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[2].getReturnDirective().getUri() == "https://youtube.fr");

	CHECK( config.getServer(0).getLocations()[3].getUri() == "/intra" ) ;
	CHECK( config.getServer(0).getLocations()[3].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getCode() == 307);
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[3].getReturnDirective().getUri() == "https://intra.42.fr");

	CHECK( config.getServer(0).getLocations()[4].getUri() == "/nginx" ) ;
	CHECK( config.getServer(0).getLocations()[4].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasText() == false);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getCode() == 308);
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getText().empty());
	CHECK( config.getServer(0).getLocations()[4].getReturnDirective().getUri() == "http://nginx.org");

	CHECK( config.getServer(0).getLocations()[5].getUri() == "/bird" ) ;
	CHECK( config.getServer(0).getLocations()[5].hasReturnDirective() == true );
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasCode() == true);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasText() == true);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().hasUri() == false);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getCode() == 204);
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getText() == "data_returned_quotes?");
	CHECK( config.getServer(0).getLocations()[5].getReturnDirective().getUri().empty());

	
	
	// server 1

	CHECK( config.getServer(1).getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).hasReturnDirective() == false);
	CHECK( config.getServer(1).getReturnDirective().hasCode() == false);
	CHECK( config.getServer(1).getReturnDirective().hasUri() == false);
	CHECK( config.getServer(1).getReturnDirective().getCode() == 0);
	CHECK( config.getServer(1).getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getReturnDirective().getUri().empty());

	CHECK( config.getServer(1).getLocations()[0].getUri() == "/" ) ;
	CHECK( config.getServer(1).getLocations()[0].hasReturnDirective() == true );
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasCode() == false);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getCode() == 0);
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getLocations()[0].getReturnDirective().getUri() == "stack");

	CHECK( config.getServer(1).getLocations()[1].getUri() == "/stack" ) ;
	CHECK( config.getServer(1).getLocations()[1].hasReturnDirective() == true );
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasCode() == false);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasText() == false);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().hasUri() == true);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getCode() == 0);
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getText().empty());
	CHECK( config.getServer(1).getLocations()[1].getReturnDirective().getUri() == "https://stackoverflow.com");

}
