#include "catch_amalgamated.hpp"

#include "ServerConfig.hpp"

#include <iostream>
#include <fstream>


TEST_CASE( "ServerConfig1", "[class][ServerConfig]" )
{
	ServerConfig* config = ServerConfig::getInstance("./config_files/default.conf");
	(void)config;

	
}
