#include "catch_amalgamated.hpp"

#include "system_error.hpp"
#include "error_code.hpp"

TEST_CASE( "ft::system_error", "[namespace][ft][system_error]" )
{
	CHECK(ft::errc::address_in_use == EADDRINUSE);

	ft::error_code err = ft::make_error_code(ft::errc::address_in_use);
	CHECK(err.value() == EADDRINUSE);
	CHECK(err.message() == "Address already in use");
	
	err.assign(EPERM);
	CHECK(err.value() == EPERM);
	
	err.clear();
	CHECK(err.value() == 0);
}
