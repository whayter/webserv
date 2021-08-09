#include "catch_amalgamated.hpp"

#include "Uri.hpp"

TEST_CASE( "Uri - Construction", "[class][uri]" ) {
    Uri uri;

	REQUIRE( uri.getScheme().empty()    );
	REQUIRE( uri.getAuthority().empty() );
	REQUIRE( uri.getUserInfo().empty()  );
	REQUIRE( uri.getHost().empty()      );
	REQUIRE( uri.getPort() == 0         );
	REQUIRE( uri.getPath().empty()      );
	REQUIRE( uri.getQuery().empty()     );
	REQUIRE( uri.getFragment().empty()  );

	uri.setScheme("ftp");
	REQUIRE( uri.getScheme() == "ftp");
	REQUIRE( uri.getPort() == 21);

    uri.setAuthority("www.42.com");
	REQUIRE( uri.getAuthority() == "www.42.com");
	// REQUIRE( uri.getPort() == 80);
	REQUIRE( uri.getSpecifiedPort() == 0);

	uri.setAuthority("user@intrA.42.com:8000");
	REQUIRE( uri.getUserInfo() == "user");
	REQUIRE( uri.getHost() == "intra.42.com");
	REQUIRE( uri.getPort() == 8000);
	REQUIRE( uri.getSpecifiedPort() == 8000);

	uri.setPath("/index.html");
	REQUIRE( uri.getPath() == "/index.html");

	uri.setPath("/file%20with%20spaces.html");
	REQUIRE( uri.getPath() == "/file with spaces.html");

	uri.setPathEtc("/query.cgi?query=foo");
	REQUIRE( uri.getPath() == "/query.cgi");
	REQUIRE( uri.getQuery() == "query=foo");
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.getPathEtc() == "/query.cgi?query=foo");
	REQUIRE( uri.getPathAndQuery() == "/query.cgi?query=foo");

	uri.setPathEtc("/query.cgi?query=bar#frag");
	REQUIRE( uri.getPath() == "/query.cgi");
	REQUIRE( uri.getQuery() == "query=bar");
	REQUIRE( uri.getFragment() == "frag");
	REQUIRE( uri.getPathEtc() == "/query.cgi?query=bar#frag");
	REQUIRE( uri.getPathAndQuery() == "/query.cgi?query=bar");

	uri.setQuery("query=test");
	REQUIRE( uri.getQuery() == "query=test");

	uri.setFragment("result");
	REQUIRE( uri.getFragment() == "result");

    SECTION( "Constructor (scheme, pathEtc)" ) {
        uri = Uri("file", "/home/juligonz/webserv.md");
	    
        REQUIRE( uri.getScheme() == "file");
	    REQUIRE( uri.getPath() == "/home/juligonz/webserv.md");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.42.com", "/index.html");
    	
        REQUIRE( uri.getScheme() == "http");
	    REQUIRE( uri.getAuthority() == "www.42.com");
	    REQUIRE( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.hwinston.com:8000", "/index.html");
        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getAuthority() == "www.hwinston.com:8000");
        REQUIRE( uri.getPath() == "/index.html");
    }
    
    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.hwinston.com:8000", "/index.html");
        REQUIRE(uri.getScheme() == "http");
        REQUIRE(uri.getUserInfo() == "user");
        REQUIRE(uri.getHost() == "www.hwinston.com");
        REQUIRE(uri.getPort() == 8000);
        REQUIRE(uri.getAuthority() == "user@www.hwinston.com:8000");
        REQUIRE(uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.appinf.com:80", "/index.html");

        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getUserInfo() == "user");
        REQUIRE( uri.getHost() == "www.appinf.com");
        REQUIRE( uri.getPort() == 80);
        REQUIRE( uri.getSpecifiedPort() == 80);
        REQUIRE( uri.getAuthority() == "user@www.appinf.com");
        REQUIRE( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.appinf.com:", "/index.html");
        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getUserInfo() == "user");
        REQUIRE( uri.getHost() == "www.appinf.com");
        REQUIRE( uri.getPort() == 80);
        REQUIRE( uri.getSpecifiedPort() == 0);
        REQUIRE( uri.getAuthority() == "user@www.appinf.com");
        REQUIRE( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.appinf.com", "/index.html", "query=test");
        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getAuthority() == "www.appinf.com");
        REQUIRE( uri.getPath() == "/index.html");
        REQUIRE( uri.getQuery() == "query=test");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.appinf.com", "/index.html", "query=test", "fragment");
        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getAuthority() == "www.appinf.com");
        REQUIRE( uri.getPath() == "/index.html");
        REQUIRE( uri.getPathEtc() == "/index.html?query=test#fragment");
        REQUIRE( uri.getQuery() == "query=test");
        REQUIRE( uri.getFragment() == "fragment");

        uri.clear();
        REQUIRE( uri.getScheme().empty());
        REQUIRE( uri.getAuthority().empty());
        REQUIRE( uri.getUserInfo().empty());
        REQUIRE( uri.getHost().empty());
        REQUIRE( uri.getPort() == 0);
        REQUIRE( uri.getPath().empty());
        REQUIRE( uri.getQuery().empty());
        REQUIRE( uri.getFragment().empty());
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
    	uri = Uri("http", "www.appinf.com", "/index.html?query=test#fragment");
        REQUIRE( uri.getScheme() == "http");
        REQUIRE( uri.getAuthority() == "www.appinf.com");
        REQUIRE( uri.getPath() == "/index.html");
        REQUIRE( uri.getPathEtc() == "/index.html?query=test#fragment");
        REQUIRE( uri.getQuery() == "query=test");
        REQUIRE( uri.getFragment() == "fragment");
    }
} /* TEST_CASE Uri - Construction */


