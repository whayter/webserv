#include "catch.hpp"

#include "Uri.hpp"

TEST_CASE( "Uri - Construction", "[class][uri]" ) {
    Uri uri;

	CHECK( uri.getScheme().empty()    );
	CHECK( uri.getAuthority().empty() );
	CHECK( uri.getUserInfo().empty()  );
	CHECK( uri.getHost().empty()      );
	CHECK( uri.getPort() == 0         );
	CHECK( uri.getPath().empty()      );
	CHECK( uri.getQuery().empty()     );
	CHECK( uri.getFragment().empty()  );

	uri.setScheme("ftp");
	CHECK( uri.getScheme() == "ftp");
	CHECK( uri.getPort() == 21);

	uri.setScheme("HTTP");
	CHECK (uri.getScheme() == "http");

    uri.setAuthority("www.42.com");
	CHECK( uri.getAuthority() == "www.42.com");
	CHECK( uri.getPort() == 80);
	CHECK( uri.getSpecifiedPort() == 0);

	uri.setAuthority("user@intrA.42.com:8000");
	CHECK( uri.getUserInfo() == "user");
	CHECK( uri.getHost() == "intra.42.com");
	CHECK( uri.getPort() == 8000);
	CHECK( uri.getSpecifiedPort() == 8000);

	uri.setPath("/index.html");
	CHECK( uri.getPath() == "/index.html");

	uri.setPath("/file%20with%20spaces.html");
	CHECK( uri.getPath() == "/file with spaces.html");

	uri.setPathEtc("/query.cgi?query=foo");
	CHECK( uri.getPath() == "/query.cgi");
	CHECK( uri.getQuery() == "query=foo");
	CHECK( uri.getFragment().empty());
	CHECK( uri.getPathEtc() == "/query.cgi?query=foo");
	CHECK( uri.getPathAndQuery() == "/query.cgi?query=foo");

	uri.setPathEtc("/query.cgi?query=bar#frag");
	CHECK( uri.getPath() == "/query.cgi");
	CHECK( uri.getQuery() == "query=bar");
	CHECK( uri.getFragment() == "frag");
	CHECK( uri.getPathEtc() == "/query.cgi?query=bar#frag");
	CHECK( uri.getPathAndQuery() == "/query.cgi?query=bar");

	uri.setQuery("query=test");
	CHECK( uri.getQuery() == "query=test");

	uri.setFragment("result");
	CHECK( uri.getFragment() == "result");

    SECTION( "Constructor (scheme, pathEtc)" ) {
        uri = Uri("file", "/home/juligonz/webserv.md");
	    
        CHECK( uri.getScheme() == "file");
	    CHECK( uri.getPath() == "/home/juligonz/webserv.md");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.42.com", "/index.html");
    	
        CHECK( uri.getScheme() == "http");
	    CHECK( uri.getAuthority() == "www.42.com");
	    CHECK( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.hwinston.com:8000", "/index.html");
        CHECK( uri.getScheme() == "http");
        CHECK( uri.getAuthority() == "www.hwinston.com:8000");
        CHECK( uri.getPath() == "/index.html");
    }
    
    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.hwinston.com:8000", "/index.html");
        CHECK(uri.getScheme() == "http");
        CHECK(uri.getUserInfo() == "user");
        CHECK(uri.getHost() == "www.hwinston.com");
        CHECK(uri.getPort() == 8000);
        CHECK(uri.getAuthority() == "user@www.hwinston.com:8000");
        CHECK(uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.appinf.com:80", "/index.html");

        CHECK( uri.getScheme() == "http");
        CHECK( uri.getUserInfo() == "user");
        CHECK( uri.getHost() == "www.appinf.com");
        CHECK( uri.getPort() == 80);
        CHECK( uri.getSpecifiedPort() == 80);
        CHECK( uri.getAuthority() == "user@www.appinf.com");
        CHECK( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "user@www.appinf.com:", "/index.html");
        CHECK( uri.getScheme() == "http");
        CHECK( uri.getUserInfo() == "user");
        CHECK( uri.getHost() == "www.appinf.com");
        CHECK( uri.getPort() == 80);
        CHECK( uri.getSpecifiedPort() == 0);
        CHECK( uri.getAuthority() == "user@www.appinf.com");
        CHECK( uri.getPath() == "/index.html");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.appinf.com", "/index.html", "query=test");
        CHECK( uri.getScheme() == "http");
        CHECK( uri.getAuthority() == "www.appinf.com");
        CHECK( uri.getPath() == "/index.html");
        CHECK( uri.getQuery() == "query=test");
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
        uri = Uri("http", "www.appinf.com", "/index.html", "query=test", "fragment");
        CHECK( uri.getScheme() == "http");
        CHECK( uri.getAuthority() == "www.appinf.com");
        CHECK( uri.getPath() == "/index.html");
        CHECK( uri.getPathEtc() == "/index.html?query=test#fragment");
        CHECK( uri.getQuery() == "query=test");
        CHECK( uri.getFragment() == "fragment");

        uri.clear();
        CHECK( uri.getScheme().empty());
        CHECK( uri.getAuthority().empty());
        CHECK( uri.getUserInfo().empty());
        CHECK( uri.getHost().empty());
        CHECK( uri.getPort() == 0);
        CHECK( uri.getPath().empty());
        CHECK( uri.getQuery().empty());
        CHECK( uri.getFragment().empty());
    }

    SECTION( "Constructor (scheme, authority, pathEtc)" ) {
    	uri = Uri("http", "www.appinf.com", "/index.html?query=test#fragment");
        CHECK( uri.getScheme() == "http");
        CHECK( uri.getAuthority() == "www.appinf.com");
        CHECK( uri.getPath() == "/index.html");
        CHECK( uri.getPathEtc() == "/index.html?query=test#fragment");
        CHECK( uri.getQuery() == "query=test");
        CHECK( uri.getFragment() == "fragment");
    }
} /* TEST_CASE Uri - Construction */

TEST_CASE( "Uri - Parser 2", "[class][uri]" ) {
    Uri uri("http://www.appinf.com");

	CHECK( uri.getScheme() == "http");
	CHECK( uri.getAuthority() == "www.appinf.com");
	CHECK( uri.getPath() == "");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "http://www.appinf.com/";
	CHECK( uri.getScheme() == "http");
	CHECK( uri.getAuthority() == "www.appinf.com");
	CHECK( uri.getPath() == "/");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "ftp://anonymous@ftp.appinf.com/pub/";
	CHECK( uri.getScheme() == "ftp");
	CHECK( uri.getUserInfo() == "anonymous");
	CHECK( uri.getHost() == "ftp.appinf.com");
	CHECK( uri.getPort() == 21);
	CHECK( uri.getAuthority() == "anonymous@ftp.appinf.com");
	CHECK( uri.getPath() == "/pub/");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());
	CHECK( !uri.isRelative());

	uri = "https://www.appinf.com/index.html#top";
	CHECK( uri.getScheme() == "https");
	CHECK( uri.getHost() == "www.appinf.com");
	CHECK( uri.getPort() == 443);
	CHECK( uri.getPath() == "/index.html");
	CHECK( uri.getQuery() == "");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment() == "top");
	CHECK( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
	CHECK( uri.getScheme() == "http");
	CHECK( uri.getHost() == "www.appinf.com");
	CHECK( uri.getPort() == 80);
	CHECK( uri.getPath() == "/search.cgi");
	CHECK( uri.getQuery() == "keyword=test&scope=all");
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
	CHECK( uri.getScheme() == "http");
	CHECK( uri.getHost() == "www.appinf.com");
	CHECK( uri.getPort() == 80);
	CHECK( uri.getPath() == "/search.cgi");
	CHECK( uri.getQuery() == "keyword=test&scope=all");
	CHECK( uri.getFragment() == "result");
	CHECK( !uri.isRelative());

	uri = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
	CHECK( uri.getScheme() == "http");
	CHECK( uri.getHost() == "www.appinf.com");
	CHECK( uri.getPort() == 80);
	CHECK( uri.getPath() == "/search.cgi");
	CHECK( uri.getQuery() == "keyword=test encoded&scope=all");
	CHECK( uri.getFragment() == "result");
	CHECK( !uri.isRelative());

	uri = "ldap://127.0.0.1/c=GB?objectClass?one";
	CHECK( uri.getScheme() == "ldap");
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost() == "127.0.0.1");
	CHECK( uri.getPort() == 389);
	CHECK( uri.getAuthority() == "127.0.0.1");
	CHECK( uri.getPath() == "/c=GB");
	CHECK( uri.getQuery() == "objectClass?one");
	CHECK( uri.getFragment().empty());


	uri = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	CHECK( uri.getScheme() == "urn");
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());

	uri = "mailto:John.Doe@example.com";
	CHECK( uri.getScheme() == "mailto");
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getPath() == "John.Doe@example.com");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());

	uri = "tel:+1-816-555-1212";
	CHECK (uri.getScheme() == "tel");
	CHECK (uri.getUserInfo().empty());
	CHECK (uri.getHost().empty());
	CHECK (uri.getPort() == 0);
	CHECK (uri.getAuthority().empty());
	CHECK (uri.getPath() == "+1-816-555-1212");
	CHECK (uri.getQuery().empty());
	CHECK (uri.getFragment().empty());

	uri = "telnet://192.0.2.16:80";
	CHECK( uri.getScheme() == "telnet");
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost() == "192.0.2.16");
	CHECK( uri.getPort() == 80);
	CHECK( uri.getAuthority() == "192.0.2.16:80");
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());


	uri = "";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.empty());

	// relative references

	uri = "/foo/bar";
	CHECK( uri.getScheme() == "");
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "/foo/bar");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "./foo/bar";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "./foo/bar");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "../foo/bar";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "../foo/bar");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "index.html";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "index.html");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "index.html#frag";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "index.html");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment() == "frag");
	CHECK( uri.isRelative());

	uri = "?query=test";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery() == "query=test");
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "?query=test#frag";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery() == "query=test");
	CHECK( uri.getFragment() == "frag");
	CHECK( uri.isRelative());

	uri = "#frag";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment() == "frag");
	CHECK( uri.isRelative());

	uri = "#";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath().empty());
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "file:///a/b/c";
	CHECK( uri.getScheme() == "file");
	CHECK( uri.getAuthority() == "" );
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "/a/b/c");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "file://localhost/a/b/c";
	CHECK( uri.getScheme() == "file");
	CHECK( uri.getAuthority() == "localhost");
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost() == "localhost");
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "/a/b/c");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "file:///c:/Windows/system32/";
	CHECK( uri.getScheme() == "file");
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "/c:/Windows/system32/");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( !uri.isRelative());

	uri = "./c:/Windows/system32/";
	CHECK( uri.getScheme().empty());
	CHECK( uri.getAuthority().empty());
	CHECK( uri.getUserInfo().empty());
	CHECK( uri.getHost().empty());
	CHECK( uri.getPort() == 0);
	CHECK( uri.getPath() == "./c:/Windows/system32/");
	CHECK( uri.getQuery().empty());
	CHECK( uri.getFragment().empty());
	CHECK( uri.isRelative());

	uri = "ws://www.appinf.com/ws";
	CHECK( uri.getScheme() == "ws");
	CHECK( uri.getPort() == 80);

	uri = "wss://www.appinf.com/ws";
	CHECK( uri.getScheme() == "wss");
	CHECK( uri.getPort() == 443);
}

TEST_CASE( "Uri - ToString() ", "[class][uri]" ) {
	Uri uri("http://www.appinf.com");
	CHECK( uri.toString() == "http://www.appinf.com");

	uri = "http://www.appinf.com/";
	CHECK( uri.toString() == "http://www.appinf.com/");

	uri = "ftp://anonymous@ftp.appinf.com/pub/";
	CHECK( uri.toString() == "ftp://anonymous@ftp.appinf.com/pub/");

	uri = "https://www.appinf.com/index.html#top";
	CHECK( uri.toString() == "https://www.appinf.com/index.html#top");

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
	CHECK( uri.toString() == "http://www.appinf.com/search.cgi?keyword=test&scope=all");

	uri = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
	CHECK( uri.toString() == "http://www.appinf.com/search.cgi?keyword=test&scope=all#result");

	uri = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
	CHECK( uri.toString() == "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");

	uri = "mailto:John.Doe@example.com";
	CHECK( uri.toString() == "mailto:John.Doe@example.com");

	uri = "tel:+1-816-555-1212";
	CHECK( uri.toString() == "tel:+1-816-555-1212");

	uri = "telnet://192.0.2.16:80";
	CHECK( uri.toString() == "telnet://192.0.2.16:80");

	uri = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
	CHECK( uri.toString() == "urn:oasis:names:specification:docbook:dtd:xml:4.1.2");

	uri = "";
	CHECK( uri.toString() == "");

	// relative references

	uri = "/foo/bar";
	CHECK( uri.toString() == "/foo/bar");

	uri = "./foo/bar";
	CHECK( uri.toString() == "./foo/bar");

	uri = "../foo/bar";
	CHECK( uri.toString() == "../foo/bar");

	uri = "//foo/bar";
	CHECK( uri.toString() == "//foo/bar");

	uri = "index.html";
	CHECK( uri.toString() == "index.html");

	uri = "index.html#frag";
	CHECK( uri.toString() == "index.html#frag");

	uri = "?query=test";
	CHECK( uri.toString() == "?query=test");

	uri = "?query=test#frag";
	CHECK( uri.toString() == "?query=test#frag");

	uri = "#frag";
	CHECK( uri.toString() == "#frag");

	uri = "#";
	CHECK( uri.toString() == "");

	uri = "file:///a/b/c";
	CHECK( uri.toString() == "file:///a/b/c");

	uri = "file://localhost/a/b/c";
	CHECK( uri.toString() == "file://localhost/a/b/c");

	uri = "file:///c:/Windows/system32/";
	CHECK( uri.toString() == "file:///c:/Windows/system32/");

	uri = "./c:/Windows/system32/";
	CHECK( uri.toString() == "./c:/Windows/system32/");

	uri = "http://www.appinf.com";
	uri.setRawQuery("query=test");
	CHECK( uri.toString() == "http://www.appinf.com/?query=test");
}

TEST_CASE( "Uri - Others ", "[class][uri]" )
{
	// The search string is "hello%world"; google happens to ignore the '%'
	// character, so it finds lots of hits for "hello world" programs. This is
	// a convenient reproduction case, and a URL that actually works.
	Uri uri("http://google.com/search?q=hello%25world#frag%20ment");

	CHECK( uri.getHost() == "google.com");
	CHECK( uri.getPath() == "/search");
	CHECK( uri.getQuery() == "q=hello%world");
	CHECK( uri.getRawQuery() == "q=hello%25world");
	CHECK( uri.getFragment() == "frag ment");
	CHECK( uri.toString() == "http://google.com/search?q=hello%25world#frag%20ment");
	CHECK( uri.getPathEtc() == "/search?q=hello%25world#frag%20ment");

	uri.setQuery("q=foo&bar");
	CHECK( uri.getQuery() == "q=foo&bar");
	CHECK( uri.getRawQuery() == "q=foo&bar");
	CHECK( uri.toString() == "http://google.com/search?q=foo&bar#frag%20ment");
	CHECK( uri.getPathEtc() == "/search?q=foo&bar#frag%20ment");

	// uri.setQuery("q=foo/bar");
	// CHECK( uri.getQuery() == "q=foo/bar");
	// CHECK( uri.getRawQuery() == "q=foo%2Fbar");
	// CHECK( uri.toString() == "http://google.com/search?q=foo%2Fbar#frag%20ment");
	// CHECK( uri.getPathEtc() == "/search?q=foo%2Fbar#frag%20ment");

	// uri.setQuery("q=goodbye cruel world");
	// CHECK( uri.getQuery() == "q=goodbye cruel world");
	// CHECK( uri.getRawQuery() == "q=goodbye%20cruel%20world");
	// CHECK( uri.toString() == "http://google.com/search?q=goodbye%20cruel%20world#frag%20ment");
	// CHECK( uri.getPathEtc() == "/search?q=goodbye%20cruel%20world#frag%20ment");

	uri.setRawQuery("q=pony%7eride");
	CHECK( uri.getQuery() == "q=pony~ride");
	CHECK( uri.getRawQuery() == "q=pony%7eride");
	CHECK( uri.toString() == "http://google.com/search?q=pony%7eride#frag%20ment");
	CHECK( uri.getPathEtc() == "/search?q=pony%7eride#frag%20ment");

	// uri.addQueryParameter("pa=ra&m1");
	// CHECK( uri.getRawQuery() == "q=pony%7eride&pa%3Dra%26m1=");
	// CHECK( uri.getQuery() == "q=pony~ride&pa=ra&m1=");
	// uri.addQueryParameter("pa=ra&m2", "val&ue");
	// CHECK( uri.getRawQuery() == "q=pony%7eride&pa%3Dra%26m1=&pa%3Dra%26m2=val%26ue");
	// CHECK( uri.getQuery() == "q=pony~ride&pa=ra&m1=&pa=ra&m2=val&ue");

	uri = "http://google.com/search?q=hello+world#frag%20ment";
	CHECK( uri.getHost() == "google.com");
	CHECK( uri.getPath() == "/search");
	CHECK( uri.getQuery() == "q=hello+world");
	CHECK( uri.getRawQuery() == "q=hello+world");
	CHECK( uri.getFragment() == "frag ment");
	CHECK( uri.toString() == "http://google.com/search?q=hello+world#frag%20ment");
	CHECK( uri.getPathEtc() == "/search?q=hello+world#frag%20ment");
}

 // Compare tests ?
