#include "catch_amalgamated.hpp"

#include "system_error.hpp"
#include "error_code.hpp"

TEST_CASE( "ft::error_code", "[namespace][ft][error_code]" )
{
	REQUIRE(ft::errc::address_in_use == EADDRINUSE);

	ft::error_code err(ft::errc::address_in_use);

	// member functions test
	CHECK(err.value() == EADDRINUSE);
	CHECK(err.message() == "Address already in use");
	err.assign(EPERM);
	CHECK(err.value() == EPERM);
	err.clear();
	CHECK(err.value() == 0);

	// assign operator test
	err = ft::make_error_code(ft::errc::bad_message);
	CHECK(err.value() == EBADMSG);
	CHECK(err.message() == "Bad message");
}

// TEST_CASE( "ft::system_error", "[namespace][ft][system_error]" )