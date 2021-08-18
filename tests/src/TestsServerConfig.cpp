#include "catch_amalgamated.hpp"

#include "ServerConfig.hpp"

#include <iostream>
#include <fstream>


TEST_CASE( "ServerConfig1", "[class][ServerConfig]" )
{
	ServerConfig& config = ServerConfig::getInstance("./config_files/testParser.conf");
	(void)config;


	CHECK( &config == &ServerConfig::getInstance("testing singleton identity"));
	CHECK( config.getServers().size() == 2);

	CHECK( config.getServer(0).listens[0].port == 80 );
	CHECK( config.getServer(0).listens[1].port == 81 );
	CHECK( config.getServer(1).listens[0].port == 82 );
	CHECK( config.getServer(1).listens[1].port == 83 );

	CHECK( &config.findServer(80) == &config.getServer(0));
	CHECK( &config.findServer(81) == &config.getServer(0));
	CHECK( &config.findServer(82) == &config.getServer(1));
	CHECK( &config.findServer(83) == &config.getServer(1));
}
