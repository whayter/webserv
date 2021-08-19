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

	CHECK( config.getServer(0).listens[0].host.empty() );
	CHECK( config.getServer(0).listens[0].port == 80 );

	CHECK( config.getServer(0).listens[1].host == "127.0.0.1" );
	CHECK( config.getServer(0).listens[1].port == 81 );

	CHECK( config.getServer(1).listens[0].host.empty() );
	CHECK( config.getServer(1).listens[0].port == 82 );
	
	CHECK( config.getServer(1).listens[1].host.empty() );
	CHECK( config.getServer(1).listens[1].port == 83 );


	CHECK( &config.findServer(80) == &config.getServer(0) );
	CHECK( &config.findServer(81) == &config.getServer(0) );
	CHECK( &config.findServer(82) == &config.getServer(1) );
	CHECK( &config.findServer(83) == &config.getServer(1) );

	//root directive
	CHECK( config.getServer(0).root == "/var/www/app" );
	CHECK( config.getServer(0).locations[0].root.empty() );
	CHECK( config.getServer(0).locations[1].root  == "/var/www/media" );
	CHECK( config.getServer(0).locations[2].root.empty() );
	
	CHECK( config.getServer(1).root == "/var/www/app" );
	CHECK( config.getServer(1).locations[0].root.empty() );
	CHECK( config.getServer(1).locations[1].root.empty() );

	//index directive
	CHECK( config.getServer(0).index == "server1.php" );
	CHECK( config.getServer(0).locations[0].index.empty());
	CHECK( config.getServer(1).index == "server2.php" );
	CHECK( config.getServer(1).locations[0].index == "server2Location.php");

	//error directive
	CHECK( config.getServer(0).errors.size() == 4);

	CHECK( config.getServer(0).errors[497] == "https://$host:443$request_uri");
	CHECK( config.getServer(0).errors[500] == "50x.html");
	CHECK( config.getServer(0).errors[501] == "50x.html");
	CHECK( config.getServer(0).errors[502] == "50x.html");
	
	CHECK( config.getServer(1).errors.size() == 1);
	CHECK( config.getServer(1).errors[497] == "https://$host:443$request_uri");

	// autoindex directive
	CHECK( config.getServer(0).autoindex == false );
	CHECK( config.getServer(0).locations[0].autoindex == false);
	CHECK( config.getServer(0).locations[1].autoindex == false);
	CHECK( config.getServer(0).locations[2].autoindex == false);

}
