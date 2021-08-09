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

	uri.setScheme("HTTP");
	REQUIRE (uri.getScheme() == "http");

    uri.setAuthority("www.42.com");
	REQUIRE( uri.getAuthority() == "www.42.com");
	REQUIRE( uri.getPort() == 80);
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

TEST_CASE( "Uri - Parsing", "[class][uri]" ) {
    Uri uri("http://www.appinf.com");
	REQUIRE( uri.getScheme() == "http");
	REQUIRE( uri.getAuthority() == "www.appinf.com");
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "http://www.appinf.com/";
	REQUIRE( uri.getScheme() == "http");
	REQUIRE( uri.getAuthority() == "www.appinf.com");
	REQUIRE( uri.getPath() == "/");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "ftp://anonymous@ftp.appinf.com/pub/";
	REQUIRE( uri.getScheme() == "ftp");
	REQUIRE( uri.getUserInfo() == "anonymous");
	REQUIRE( uri.getHost() == "ftp.appinf.com");
	REQUIRE( uri.getPort() == 21);
	REQUIRE( uri.getAuthority() == "anonymous@ftp.appinf.com");
	REQUIRE( uri.getPath() == "/pub/");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());
	REQUIRE( !uri.isRelative());

	uri = "https://www.appinf.com/index.html#top";
	REQUIRE( uri.getScheme() == "https");
	REQUIRE( uri.getHost() == "www.appinf.com");
	REQUIRE( uri.getPort() == 443);
	REQUIRE( uri.getPath() == "/index.html");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment() == "top");
	REQUIRE( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
	REQUIRE( uri.getScheme() == "http");
	REQUIRE( uri.getHost() == "www.appinf.com");
	REQUIRE( uri.getPort() == 80);
	REQUIRE( uri.getPath() == "/search.cgi");
	REQUIRE( uri.getQuery() == "keyword=test&scope=all");
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
	REQUIRE( uri.getScheme() == "http");
	REQUIRE( uri.getHost() == "www.appinf.com");
	REQUIRE( uri.getPort() == 80);
	REQUIRE( uri.getPath() == "/search.cgi");
	REQUIRE( uri.getQuery() == "keyword=test&scope=all");
	REQUIRE( uri.getFragment() == "result");
	REQUIRE( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
	REQUIRE( uri.getScheme() == "http");
	REQUIRE( uri.getHost() == "www.appinf.com");
	REQUIRE( uri.getPort() == 80);
	REQUIRE( uri.getPath() == "/search.cgi");
	REQUIRE( uri.getQuery() == "keyword=test encoded&scope=all");
	REQUIRE( uri.getFragment() == "result");
	REQUIRE( !uri.isRelative());

	uri = "ldap://[2001:db8::7]/c=GB?objectClass?one";
	REQUIRE( uri.getScheme() == "ldap");
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost() == "2001:db8::7");
	REQUIRE( uri.getPort() == 389);
	REQUIRE( uri.getAuthority() == "[2001:db8::7]");
	REQUIRE( uri.getPath() == "/c=GB");
	REQUIRE( uri.getQuery() == "objectClass?one");
	REQUIRE( uri.getFragment().empty());

	uri = "mailto:John.Doe@example.com";
	REQUIRE( uri.getScheme() == "mailto");
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getPath() == "John.Doe@example.com");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());

	uri = "tel:+1-816-555-1212";
	REQUIRE (uri.getScheme() == "tel");
	REQUIRE (uri.getUserInfo().empty());
	REQUIRE (uri.getHost().empty());
	REQUIRE (uri.getPort() == 0);
	REQUIRE (uri.getAuthority().empty());
	REQUIRE (uri.getPath() == "+1-816-555-1212");
	REQUIRE (uri.getQuery().empty());
	REQUIRE (uri.getFragment().empty());

	uri = "telnet://192.0.2.16:80";
	REQUIRE( uri.getScheme() == "telnet");
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost() == "192.0.2.16");
	REQUIRE( uri.getPort() == 80);
	REQUIRE( uri.getAuthority() == "192.0.2.16:80");
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());

	uri = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	REQUIRE( uri.getScheme() == "urn");
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());

	uri = "";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.empty());

	// relative references

	uri = "/foo/bar";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "/foo/bar");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "./foo/bar";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "./foo/bar");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "../foo/bar";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "../foo/bar");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "index.html";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "index.html");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "index.html#frag";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "index.html");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment() == "frag");
	REQUIRE( uri.isRelative());

	uri = "?query=test";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery() == "query=test");
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "?query=test#frag";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery() == "query=test");
	REQUIRE( uri.getFragment() == "frag");
	REQUIRE( uri.isRelative());

	uri = "#frag";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment() == "frag");
	REQUIRE( uri.isRelative());

	uri = "#";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath().empty());
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "file:///a/b/c";
	REQUIRE( uri.getScheme() == "file");
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "/a/b/c");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "file://localhost/a/b/c";
	REQUIRE( uri.getScheme() == "file");
	REQUIRE( uri.getAuthority() == "localhost");
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost() == "localhost");
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "/a/b/c");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "file:///c:/Windows/system32/";
	REQUIRE( uri.getScheme() == "file");
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "/c:/Windows/system32/");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( !uri.isRelative());

	uri = "./c:/Windows/system32/";
	REQUIRE( uri.getScheme().empty());
	REQUIRE( uri.getAuthority().empty());
	REQUIRE( uri.getUserInfo().empty());
	REQUIRE( uri.getHost().empty());
	REQUIRE( uri.getPort() == 0);
	REQUIRE( uri.getPath() == "./c:/Windows/system32/");
	REQUIRE( uri.getQuery().empty());
	REQUIRE( uri.getFragment().empty());
	REQUIRE( uri.isRelative());

	uri = "ws://www.appinf.com/ws";
	REQUIRE( uri.getScheme() == "ws");
	REQUIRE( uri.getPort() == 80);

	uri = "wss://www.appinf.com/ws";
	REQUIRE( uri.getScheme() == "wss");
	REQUIRE( uri.getPort() == 443);
}