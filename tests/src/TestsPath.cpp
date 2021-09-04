#include "catch_amalgamated.hpp"

#include "Path.hpp"

TEST_CASE( "Path - static functions", "[class][path][static]" ) {
	// std::string s = Path::current();
	// CHECK (!s.empty());
	// Path p(s);
	// CHECK ((p.isDirectory() && p.isAbsolute()));
	// CHECK (p.isDirectory());
	// CHECK (p.isAbsolute());

	// s = Path::home();
	// CHECK (!s.empty());
	// p = s;
	// REQUIRE ((p.isDirectory() && p.isAbsolute()));
	// CHECK (p.isDirectory());
	// CHECK (p.isAbsolute());
	// CHECK (Path::home() == "");

	// s = Path::temp();
	// CHECK (!s.empty());
	// p = s;
	// REQUIRE (p.isDirectory() && p.isAbsolute());
}

TEST_CASE( "Path - Construction", "[class][path]" ) {

	Path p;
	p.parse("");
	CHECK( p.isRelative() );
	CHECK( !p.isAbsolute() );
	// CHECK( p.depth() == 0);
	CHECK( p.isDirectory());
	CHECK( !p.isFile());
	CHECK( p.toString() == "");

	p.parse("/");
	CHECK ( !p.isRelative());
	CHECK ( p.isAbsolute());
	// CHECK ( p.depth() == 0);
	CHECK ( p.isDirectory());
	CHECK ( !p.isFile());
	CHECK ( p.toString() == "/");

	p.parse("/usr");
	CHECK ( !p.isRelative());
	CHECK ( p.isAbsolute());
	// CHECK ( p.depth() == 0);
	CHECK ( !p.isDirectory());
	CHECK ( p.isFile());
	CHECK ( p.toString() == "/usr");

	p.parse("/usr/");
	CHECK ( !p.isRelative());
	CHECK ( p.isAbsolute());
	// CHECK ( p.depth() == 1);
	// CHECK ( p[0] == "usr");
	CHECK ( p.isDirectory());
	CHECK ( !p.isFile());
	CHECK ( p.toString() == "/usr/");

// 	p.parse("usr/");
// 	CHECK ( p.isRelative());
// 	CHECK ( !p.isAbsolute());
// 	// CHECK ( p.depth() == 1);
// 	// CHECK ( p[0] == "usr");
// 	CHECK ( p.isDirectory());
// 	CHECK ( !p.isFile());
// 	CHECK ( p.toString() == "usr/");

// 	p.parse("usr");
// 	CHECK ( p.isRelative());
// 	CHECK ( !p.isAbsolute());
// 	// CHECK ( p.depth() == 0);
// 	// CHECK ( p[0] == "usr");
// 	CHECK ( !p.isDirectory());
// 	CHECK ( p.isFile());
// 	CHECK ( p.toString() == "usr");

// 	p.parse("/usr/local");
// 	CHECK ( !p.isRelative());
// 	CHECK ( p.isAbsolute());
// 	// CHECK ( p.depth() == 1);
// 	// CHECK ( p[0] == "usr");
// 	// CHECK ( p[1] == "local");
// 	CHECK ( !p.isDirectory());
// 	CHECK ( p.isFile());
// 	CHECK ( p.toString() == "/usr/local");
}

