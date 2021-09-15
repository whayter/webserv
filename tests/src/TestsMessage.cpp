#include "catch_amalgamated.hpp"

#include "Request.hpp"

/// simple get
TEST_CASE( "Http::Request", "[namespace][http][class][Request]" )
{
	http::Request req;

	REQUIRE( req.empty());
	req.setUri(Uri("http", "/google.com"));
	REQUIRE( !req.empty());
	req.clear();
	REQUIRE( req.empty());
}
