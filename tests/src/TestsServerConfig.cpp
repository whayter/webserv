#include "catch_amalgamated.hpp"

#include "ServerConfig.hpp"

#include <iostream>
#include <fstream>


TEST_CASE( "ServerConfig1 - ./config_files/testParser.conf", "[class][ServerConfig]" )
{
	ServerConfig& config = ServerConfig::getInstance("./config_files/testParser.conf");
	(void)config;


	// listen directive
	CHECK( &config == &ServerConfig::getInstance("testing singleton identity, must override first call"));
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

	CHECK( config.getServer(1).getLocations()[0].getFastCgiParams().size() == 0);
	CHECK( config.getServer(1).getLocations()[1].getFastCgiParams().size() == 2);



}
