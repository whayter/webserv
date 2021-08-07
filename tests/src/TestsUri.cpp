#include "catch_amalgamated.hpp"

#include "Uri.hpp"

TEST_CASE( "Uri - ", "[class][uri]" ) {
    Uri uri;

    SECTION( "Simple /index.html" ) {
        uri = Uri(" /index.html");

        REQUIRE(uri.getPath() == "index.html");
    }

    SECTION( "Simple /index.html" ) {
        uri = Uri(" /index.html?format=json&sort=asc");

        REQUIRE(uri.getPath() == "index.html");
        REQUIRE(uri.getQueryString() == "format=json&sort=asc");
    }

}