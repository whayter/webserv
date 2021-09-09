#include "catch_amalgamated.hpp"

#include "system_error.hpp"

TEST_CASE( "ft::system_error", "[namespace][ft][system_error]" )
{
	CHECK(ft::errc::address_in_use == EADDRINUSE);
}
