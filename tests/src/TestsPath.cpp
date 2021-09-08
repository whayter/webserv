#include "catch_amalgamated.hpp"

#include "Filesystem.hpp"


namespace fs = ft::filesystem;

TEST_CASE( "ft::filesystem::path - Construction", "[class][path][construct]" )
{

	CHECK( fs::path().empty());
	CHECK( fs::path().root_directory().empty() );
	CHECK( fs::path().relative_path().empty() );


	CHECK( fs::path().has_root_directory() == false );
	CHECK( fs::path().has_root_path() == false );
	CHECK( fs::path().has_relative_path() == false );
	CHECK( fs::path().has_parent_path() == false );
	CHECK( fs::path().has_filename() == false );
	CHECK( fs::path().has_stem() == false );
	CHECK( fs::path().has_extension() == false );
	CHECK( fs::path().is_absolute() == false );
	CHECK( fs::path().is_relative() == true );


    CHECK(fs::path("///foo/bar").empty() == false);

    CHECK(fs::path("///foo/bar") == fs::path("/foo/bar"));
    CHECK(fs::path("///foo/bar") == "/foo/bar");
    CHECK(fs::path("//foo//bar") == "//foo/bar");
    CHECK("/usr/local/bin" == fs::path("/usr/local/bin"));


}

TEST_CASE( "path - Assign", "[class][path][assign]" )
{
    fs::path p1("/foo/bar");
    fs::path p2("/usr/local");
    fs::path p3;
    p3 = p1;
    REQUIRE(p1 == p3);
    p3 = fs::path("/usr/local");
    REQUIRE(p2 == p3);
    p3 = fs::path("/usr/local");
    REQUIRE(p2 == p3);
	p3 = fs::path::string_type("/foo/bar");
    REQUIRE(p1 == p3);
}

// TEST_CASE( "path - Append", "[class][path][append]" ) {
//     CHECK(fs::path("foo") / "" == "foo/");
//     CHECK(fs::path("foo") / "/bar" == "/bar");
//     CHECK(fs::path("/foo") / "/" == "/");
//     // if (has_host_root_name_support()) {
//     //     CHECK(fs::path("//host/foo") / "/bar" == "/bar");
//     //     CHECK(fs::path("//host") / "/" == "//host/");
//     //     CHECK(fs::path("//host/foo") / "/" == "/");
//     // }

// 	CHECK(fs::path("/foo/bar") / "some///other" == "/foo/bar/some/other");
//     fs::path p1("/tmp/test");
//     fs::path p2("foobar.txt");
//     ft::path p3 = p1 / p2;
//     CHECK("/tmp/test/foobar.txt" == p3);
// }

// TEST_CASE( "fs::path - Concat", "[class][path][concatenation]" )
// {
//     CHECK((path("foo") += path("bar")) == "foobar");
//     CHECK((path("foo") += path("/bar")) == "foo/bar");

//     CHECK((path("foo") += std::string("bar")) == "foobar");
//     CHECK((path("foo") += std::string("/bar")) == "foo/bar");

//     CHECK((path("foo") += "bar") == "foobar");
//     CHECK((path("foo") += "/bar") == "foo/bar");

// 	CHECK((path("foo") += 'b') == "foob");
//     CHECK((path("foo") += '/') == "foo/");

// 	CHECK((path("foo") += std::string("bar")) == "foobar");
//     CHECK((path("foo") += std::string("/bar")) == "foo/bar");


//     CHECK(path("foo").concat("bar") == "foobar");
//     CHECK(path("foo").concat("/bar") == "foo/bar");

//     std::string bar = "bar";
//     CHECK(path("foo").concat(bar.begin(), bar.end()) == "foobar");

//     CHECK((path("/foo/bar") += "/some///other") == "/foo/bar/some/other");


// }

// TEST_CASE( "path - Modifiers", "[class][path][modifiers]" ) {

//     CHECK(path("foo/bar").remove_filename() == "foo/");
//     CHECK(path("foo/").remove_filename() == "foo/");
//     CHECK(path("/foo").remove_filename() == "/");
//     CHECK(path("/").remove_filename() == "/");

	// CHECK(path("/foo").replace_filename("bar") == "/bar");
    // CHECK(path("/").replace_filename("bar") == "/bar");
    // CHECK(path("/foo").replace_filename("b//ar") == "/b/ar");

    // CHECK(path("/foo/bar.txt").replace_extension("odf") == "/foo/bar.odf");
    // CHECK(path("/foo/bar.txt").replace_extension() == "/foo/bar");
    // CHECK(path("/foo/bar").replace_extension("odf") == "/foo/bar.odf");
    // CHECK(path("/foo/bar").replace_extension(".odf") == "/foo/bar.odf");
    // CHECK(path("/foo/bar.").replace_extension(".odf") == "/foo/bar.odf");
    // CHECK(path("/foo/bar/").replace_extension("odf") == "/foo/bar/.odf");

	// path p1 = "foo";
    // path p2 = "bar";
    // p1.swap(p2);
    // CHECK(p1 == "bar");
    // CHECK(p2 == "foo");
// }

// TEST_CASE( "path - Modifier", "[class][path][.]" ) {

//     CHECK(path("/foo/b").compare("/foo/a") > 0);
//     CHECK(path("/foo/b").compare("/foo/b") == 0);
//     CHECK(path("/foo/b").compare("/foo/c") < 0);

//     CHECK(path("/foo/b").compare(std::string("/foo/a")) > 0);
//     CHECK(path("/foo/b").compare(std::string("/foo/b")) == 0);
//     CHECK(path("/foo/b").compare(std::string("/foo/c")) < 0);

//     CHECK(path("/foo/b").compare(path("/foo/a")) > 0);
//     CHECK(path("/foo/b").compare(path("/foo/b")) == 0);
//     CHECK(path("/foo/b").compare(path("/foo/c")) < 0);


//     CHECK(path("/a/b/").compare("/a/b/c") < 0);
//     CHECK(path("/a/b/").compare("a/c") > 0);
// }

// TEST_CASE( "path - Decompose", "[class][path][.]" ) {
//     CHECK(path("").root_directory() == "");
//     CHECK(path(".").root_directory() == "");
//     CHECK(path("..").root_directory() == "");
//     CHECK(path("foo").root_directory() == "");
//     CHECK(path("/").root_directory() == "/");
//     CHECK(path("/foo").root_directory() == "/");
//     CHECK(path("foo/").root_directory() == "");
//     CHECK(path("/foo/").root_directory() == "/");
//     CHECK(path("foo/bar").root_directory() == "");
//     CHECK(path("/foo/bar").root_directory() == "/");
//     CHECK(path("///foo/bar").root_directory() == "/");


//     CHECK(path("").root_path() == "");
//     CHECK(path(".").root_path() == "");
//     CHECK(path("..").root_path() == "");
//     CHECK(path("foo").root_path() == "");
//     CHECK(path("/").root_path() == "/");
//     CHECK(path("/foo").root_path() == "/");
//     CHECK(path("foo/").root_path() == "");
//     CHECK(path("/foo/").root_path() == "/");
//     CHECK(path("foo/bar").root_path() == "");
//     CHECK(path("/foo/bar").root_path() == "/");
//     CHECK(path("///foo/bar").root_path() == "/");

// 	CHECK(path("").relative_path() == "");
//     CHECK(path(".").relative_path() == ".");
//     CHECK(path("..").relative_path() == "..");
//     CHECK(path("foo").relative_path() == "foo");
//     CHECK(path("/").relative_path() == "");
//     CHECK(path("/foo").relative_path() == "foo");
//     CHECK(path("foo/").relative_path() == "foo/");
//     CHECK(path("/foo/").relative_path() == "foo/");
//     CHECK(path("foo/bar").relative_path() == "foo/bar");
//     CHECK(path("/foo/bar").relative_path() == "foo/bar");
//     CHECK(path("///foo/bar").relative_path() == "foo/bar");

// 	CHECK(path("").parent_path() == "");
//     CHECK(path(".").parent_path() == "");
//     CHECK(path("..").parent_path() == "");  // unintuitive but as defined in the standard
//     CHECK(path("foo").parent_path() == "");
//     CHECK(path("/").parent_path() == "/");
//     CHECK(path("/foo").parent_path() == "/");
//     CHECK(path("foo/").parent_path() == "foo");
//     CHECK(path("/foo/").parent_path() == "/foo");
//     CHECK(path("foo/bar").parent_path() == "foo");
//     CHECK(path("/foo/bar").parent_path() == "/foo");
//     CHECK(path("///foo/bar").parent_path() == "/foo");

// 	CHECK(path("").filename() == "");
//     CHECK(path(".").filename() == ".");
//     CHECK(path("..").filename() == "..");
//     CHECK(path("foo").filename() == "foo");
//     CHECK(path("/").filename() == "");
//     CHECK(path("/foo").filename() == "foo");
//     CHECK(path("foo/").filename() == "");
//     CHECK(path("/foo/").filename() == "");
//     CHECK(path("foo/bar").filename() == "bar");
//     CHECK(path("/foo/bar").filename() == "bar");
//     CHECK(path("///foo/bar").filename() == "bar");


// 	// stem()
//     CHECK(path("/foo/bar.txt").stem() == "bar");
//     {
//         path p = "foo.bar.baz.tar";
//         CHECK(p.extension() == ".tar");
//         p = p.stem();
//         CHECK(p.extension() == ".baz");
//         p = p.stem();
//         CHECK(p.extension() == ".bar");
//         p = p.stem();
//         CHECK(p == "foo");
//     }
//     CHECK(path("/foo/.profile").stem() == ".profile");
//     CHECK(path(".bar").stem() == ".bar");
//     CHECK(path("..bar").stem() == ".");

//     // extension()
//     CHECK(path("/foo/bar.txt").extension() == ".txt");
//     CHECK(path("/foo/bar").extension() == "");
//     CHECK(path("/foo/.profile").extension() == "");
//     CHECK(path(".bar").extension() == "");
//     CHECK(path("..bar").extension() == ".bar");
// }

// TEST_CASE( "path - Query", "[class][path][.]" ) {
//    // has_relative_path()
//     CHECK(!path("").has_relative_path());
//     CHECK(!path("/").has_relative_path());
//     CHECK(path("/foo").has_relative_path());

//     // has_parent_path()
//     CHECK(!path("").has_parent_path());
//     CHECK(!path(".").has_parent_path());
//     CHECK(!path("..").has_parent_path());  // unintuitive but as defined in the standard
//     CHECK(!path("foo").has_parent_path());
//     CHECK(path("/").has_parent_path());
//     CHECK(path("/foo").has_parent_path());
//     CHECK(path("foo/").has_parent_path());
//     CHECK(path("/foo/").has_parent_path());

//     // has_filename()
//     CHECK(path("foo").has_filename());
//     CHECK(path("foo/bar").has_filename());
//     CHECK(!path("/foo/bar/").has_filename());

//     // has_stem()
//     CHECK(path("foo").has_stem());
//     CHECK(path("foo.bar").has_stem());
//     CHECK(path(".profile").has_stem());
//     CHECK(!path("/foo/").has_stem());

//     // has_extension()
//     CHECK(!path("foo").has_extension());
//     CHECK(path("foo.bar").has_extension());
//     CHECK(!path(".profile").has_extension());

//     // is_absolute()
//     CHECK(!path("foo/bar").is_absolute());
//     CHECK(path("/foo").is_absolute());

//     // is_relative()
//     CHECK(path("foo/bar").is_relative());
// 	CHECK(!path("/foo").is_relative());

// }

// TEST_CASE( "path - Generation", "[class][path][.]" ) {

//     // lexically_normal()
//     CHECK(path("foo/./bar/..").lexically_normal() == "foo/");
//     CHECK(path("foo/.///bar/../").lexically_normal() == "foo/");
//     CHECK(path("/foo/../..").lexically_normal() == "/");
//     CHECK(path("foo/..").lexically_normal() == ".");
//     CHECK(path("ab/cd/ef/../../qw").lexically_normal() == "ab/qw");
//     CHECK(path("a/b/../../../c").lexically_normal() == "../c");
//     CHECK(path("../").lexically_normal() == "..");

// 	// lexically_relative()
//     CHECK(path("/a/d").lexically_relative("/a/b/c") == "../../d");
//     CHECK(path("/a/b/c").lexically_relative("/a/d") == "../b/c");
//     CHECK(path("a/b/c").lexically_relative("a") == "b/c");
//     CHECK(path("a/b/c").lexically_relative("a/b/c/x/y") == "../..");
//     CHECK(path("a/b/c").lexically_relative("a/b/c") == ".");
//     CHECK(path("a/b").lexically_relative("c/d") == "../../a/b");
//     CHECK(path("a/b").lexically_relative("a/") == "b");

//  // lexically_proximate()
//     CHECK(path("/a/d").lexically_proximate("/a/b/c") == "../../d");
//     CHECK(path("a/d").lexically_proximate("/a/b/c") == "a/d");

//     CHECK(path("/foo").lexically_proximate("bar") == "/foo");
//     CHECK(path("foo").lexically_proximate("/bar") == "foo");
// }

TEST_CASE( "path - Non member functions", "[class][path][non-member-function]" )
{
    fs::path p1("foo/bar");
    fs::path p2("some/other");
    fs::swap(p1, p2);
    CHECK(p1 == "some/other");
    CHECK(p2 == "foo/bar");
    CHECK(p2 < p1);
    CHECK(p2 <= p1);
    CHECK(p1 <= p1);
    CHECK(!(p1 < p2));
    CHECK(!(p1 <= p2));
    CHECK(p1 > p2);
    CHECK(p1 >= p2);
    CHECK(p1 >= p1);
    CHECK(!(p2 > p1));
    CHECK(!(p2 >= p1));
    CHECK(p1 != p2);
    CHECK(p1 / p2 == "some/other/foo/bar");
}