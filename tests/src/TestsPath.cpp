#include "catch_amalgamated.hpp"

#include "Path.hpp"

#include <filesystem>

TEST_CASE( "Path - Construction", "[class][path][.]" ) {

	// std::filesystem::path(".").extension().extension().extension().generic_u8string();
	CHECK( Path().empty());
	CHECK( Path().root_directory().empty() );
	CHECK( Path().relative_path().empty() );


	CHECK( Path().has_root_directory() == false );
	CHECK( Path().has_root_path() == false );
	CHECK( Path().has_relative_path() == false );
	CHECK( Path().has_parent_path() == false );
	CHECK( Path().has_filename() == false );
	CHECK( Path().has_stem() == false );
	CHECK( Path().has_extension() == false );
	CHECK( Path().is_absolute() == false );
	CHECK( Path().is_relative() == true );


    CHECK(Path("///foo/bar").empty() == false);

    CHECK(Path("///foo/bar") == Path("/foo/bar"));
    // CHECK(Path("//foo//bar") == "//foo/bar");
    // CHECK("/usr/local/bin" == Path("/usr/local/bin"));


}

// TEST_CASE( "Path - Assign", "[class][path][.]" ) {
//     Path p1("/foo/bar");
//     Path p2("/usr/local");
//     Path p3;
//     p3 = p1;
//     REQUIRE(p1 == p3);
//     p3 = Path("/usr/local");
//     REQUIRE(p2 == p3);
//     p3 = Path("/usr/local");
//     REQUIRE(p2 == p3);
//     p3.assign(L"/usr/local");
//     REQUIRE(p2 == p3);

// 	p3 = Path::string_type("/foo/bar");
//     REQUIRE(p1 == p3);
// }

// TEST_CASE( "Path - Append", "[class][path][.]" ) {
//     CHECK(Path("foo") / "" == "foo/");
//     CHECK(Path("foo") / "/bar" == "/bar");
//     CHECK(Path("/foo") / "/" == "/");
//     // if (has_host_root_name_support()) {
//     //     CHECK(Path("//host/foo") / "/bar" == "/bar");
//     //     CHECK(Path("//host") / "/" == "//host/");
//     //     CHECK(Path("//host/foo") / "/" == "/");
//     // }

// 	CHECK(Path("/foo/bar") / "some///other" == "/foo/bar/some/other");
//     Path p1("/tmp/test");
//     Path p2("foobar.txt");
//     Path p3 = p1 / p2;
//     CHECK("/tmp/test/foobar.txt" == p3);
// }

// TEST_CASE( "Path - Concat", "[class][path][.]" ) {
// {
//     CHECK((Path("foo") += Path("bar")) == "foobar");
//     CHECK((Path("foo") += Path("/bar")) == "foo/bar");

//     CHECK((Path("foo") += std::string("bar")) == "foobar");
//     CHECK((Path("foo") += std::string("/bar")) == "foo/bar");

//     CHECK((Path("foo") += "bar") == "foobar");
//     CHECK((Path("foo") += "/bar") == "foo/bar");

// 	CHECK((Path("foo") += 'b') == "foob");
//     CHECK((Path("foo") += '/') == "foo/");

// 	CHECK((Path("foo") += std::string("bar")) == "foobar");
//     CHECK((Path("foo") += std::string("/bar")) == "foo/bar");


//     CHECK(Path("foo").concat("bar") == "foobar");
//     CHECK(Path("foo").concat("/bar") == "foo/bar");
//     CHECK(Path("foo").concat(L"bar") == "foobar");
//     CHECK(Path("foo").concat(L"/bar") == "foo/bar");

//     std::string bar = "bar";
//     CHECK(Path("foo").concat(bar.begin(), bar.end()) == "foobar");

//     CHECK((Path("/foo/bar") += "/some///other") == "/foo/bar/some/other");


// }

// TEST_CASE( "Path - Modifier", "[class][path][.]" ) {

//     CHECK(Path("foo/bar").remove_filename() == "foo/");
//     CHECK(Path("foo/").remove_filename() == "foo/");
//     CHECK(Path("/foo").remove_filename() == "/");
//     CHECK(Path("/").remove_filename() == "/");

// 	CHECK(Path("/foo").replace_filename("bar") == "/bar");
//     CHECK(Path("/").replace_filename("bar") == "/bar");
//     CHECK(Path("/foo").replace_filename("b//ar") == "/b/ar");

//     CHECK(Path("/foo/bar.txt").replace_extension("odf") == "/foo/bar.odf");
//     CHECK(Path("/foo/bar.txt").replace_extension() == "/foo/bar");
//     CHECK(Path("/foo/bar").replace_extension("odf") == "/foo/bar.odf");
//     CHECK(Path("/foo/bar").replace_extension(".odf") == "/foo/bar.odf");
//     CHECK(Path("/foo/bar.").replace_extension(".odf") == "/foo/bar.odf");
//     CHECK(Path("/foo/bar/").replace_extension("odf") == "/foo/bar/.odf");

// 	Path p1 = "foo";
//     Path p2 = "bar";
//     p1.swap(p2);
//     CHECK(p1 == "bar");
//     CHECK(p2 == "foo");
// }

// TEST_CASE( "Path - Modifier", "[class][path][.]" ) {

//     CHECK(Path("/foo/b").compare("/foo/a") > 0);
//     CHECK(Path("/foo/b").compare("/foo/b") == 0);
//     CHECK(Path("/foo/b").compare("/foo/c") < 0);

//     CHECK(Path("/foo/b").compare(std::string("/foo/a")) > 0);
//     CHECK(Path("/foo/b").compare(std::string("/foo/b")) == 0);
//     CHECK(Path("/foo/b").compare(std::string("/foo/c")) < 0);

//     CHECK(Path("/foo/b").compare(Path("/foo/a")) > 0);
//     CHECK(Path("/foo/b").compare(Path("/foo/b")) == 0);
//     CHECK(Path("/foo/b").compare(Path("/foo/c")) < 0);


//     CHECK(Path("/a/b/").compare("/a/b/c") < 0);
//     CHECK(Path("/a/b/").compare("a/c") > 0);
// }

// TEST_CASE( "Path - Decompose", "[class][path][.]" ) {
//     CHECK(Path("").root_directory() == "");
//     CHECK(Path(".").root_directory() == "");
//     CHECK(Path("..").root_directory() == "");
//     CHECK(Path("foo").root_directory() == "");
//     CHECK(Path("/").root_directory() == "/");
//     CHECK(Path("/foo").root_directory() == "/");
//     CHECK(Path("foo/").root_directory() == "");
//     CHECK(Path("/foo/").root_directory() == "/");
//     CHECK(Path("foo/bar").root_directory() == "");
//     CHECK(Path("/foo/bar").root_directory() == "/");
//     CHECK(Path("///foo/bar").root_directory() == "/");


//     CHECK(Path("").root_path() == "");
//     CHECK(Path(".").root_path() == "");
//     CHECK(Path("..").root_path() == "");
//     CHECK(Path("foo").root_path() == "");
//     CHECK(Path("/").root_path() == "/");
//     CHECK(Path("/foo").root_path() == "/");
//     CHECK(Path("foo/").root_path() == "");
//     CHECK(Path("/foo/").root_path() == "/");
//     CHECK(Path("foo/bar").root_path() == "");
//     CHECK(Path("/foo/bar").root_path() == "/");
//     CHECK(Path("///foo/bar").root_path() == "/");

// 	CHECK(Path("").relative_path() == "");
//     CHECK(Path(".").relative_path() == ".");
//     CHECK(Path("..").relative_path() == "..");
//     CHECK(Path("foo").relative_path() == "foo");
//     CHECK(Path("/").relative_path() == "");
//     CHECK(Path("/foo").relative_path() == "foo");
//     CHECK(Path("foo/").relative_path() == "foo/");
//     CHECK(Path("/foo/").relative_path() == "foo/");
//     CHECK(Path("foo/bar").relative_path() == "foo/bar");
//     CHECK(Path("/foo/bar").relative_path() == "foo/bar");
//     CHECK(Path("///foo/bar").relative_path() == "foo/bar");

// 	CHECK(Path("").parent_path() == "");
//     CHECK(Path(".").parent_path() == "");
//     CHECK(Path("..").parent_path() == "");  // unintuitive but as defined in the standard
//     CHECK(Path("foo").parent_path() == "");
//     CHECK(Path("/").parent_path() == "/");
//     CHECK(Path("/foo").parent_path() == "/");
//     CHECK(Path("foo/").parent_path() == "foo");
//     CHECK(Path("/foo/").parent_path() == "/foo");
//     CHECK(Path("foo/bar").parent_path() == "foo");
//     CHECK(Path("/foo/bar").parent_path() == "/foo");
//     CHECK(Path("///foo/bar").parent_path() == "/foo");

// 	CHECK(Path("").filename() == "");
//     CHECK(Path(".").filename() == ".");
//     CHECK(Path("..").filename() == "..");
//     CHECK(Path("foo").filename() == "foo");
//     CHECK(Path("/").filename() == "");
//     CHECK(Path("/foo").filename() == "foo");
//     CHECK(Path("foo/").filename() == "");
//     CHECK(Path("/foo/").filename() == "");
//     CHECK(Path("foo/bar").filename() == "bar");
//     CHECK(Path("/foo/bar").filename() == "bar");
//     CHECK(Path("///foo/bar").filename() == "bar");


// 	// stem()
//     CHECK(Path("/foo/bar.txt").stem() == "bar");
//     {
//         Path p = "foo.bar.baz.tar";
//         CHECK(p.extension() == ".tar");
//         p = p.stem();
//         CHECK(p.extension() == ".baz");
//         p = p.stem();
//         CHECK(p.extension() == ".bar");
//         p = p.stem();
//         CHECK(p == "foo");
//     }
//     CHECK(Path("/foo/.profile").stem() == ".profile");
//     CHECK(Path(".bar").stem() == ".bar");
//     CHECK(Path("..bar").stem() == ".");

//     // extension()
//     CHECK(Path("/foo/bar.txt").extension() == ".txt");
//     CHECK(Path("/foo/bar").extension() == "");
//     CHECK(Path("/foo/.profile").extension() == "");
//     CHECK(Path(".bar").extension() == "");
//     CHECK(Path("..bar").extension() == ".bar");
// }

// TEST_CASE( "Path - Query", "[class][path][.]" ) {
//    // has_relative_path()
//     CHECK(!Path("").has_relative_path());
//     CHECK(!Path("/").has_relative_path());
//     CHECK(Path("/foo").has_relative_path());

//     // has_parent_path()
//     CHECK(!Path("").has_parent_path());
//     CHECK(!Path(".").has_parent_path());
//     CHECK(!Path("..").has_parent_path());  // unintuitive but as defined in the standard
//     CHECK(!Path("foo").has_parent_path());
//     CHECK(Path("/").has_parent_path());
//     CHECK(Path("/foo").has_parent_path());
//     CHECK(Path("foo/").has_parent_path());
//     CHECK(Path("/foo/").has_parent_path());

//     // has_filename()
//     CHECK(Path("foo").has_filename());
//     CHECK(Path("foo/bar").has_filename());
//     CHECK(!Path("/foo/bar/").has_filename());

//     // has_stem()
//     CHECK(Path("foo").has_stem());
//     CHECK(Path("foo.bar").has_stem());
//     CHECK(Path(".profile").has_stem());
//     CHECK(!Path("/foo/").has_stem());

//     // has_extension()
//     CHECK(!Path("foo").has_extension());
//     CHECK(Path("foo.bar").has_extension());
//     CHECK(!Path(".profile").has_extension());

//     // is_absolute()
//     CHECK(!Path("foo/bar").is_absolute());
//     CHECK(Path("/foo").is_absolute());

//     // is_relative()
//     CHECK(Path("foo/bar").is_relative());
// 	CHECK(!Path("/foo").is_relative());

// }

// TEST_CASE( "Path - Generation", "[class][path][.]" ) {

//     // lexically_normal()
//     CHECK(Path("foo/./bar/..").lexically_normal() == "foo/");
//     CHECK(Path("foo/.///bar/../").lexically_normal() == "foo/");
//     CHECK(Path("/foo/../..").lexically_normal() == "/");
//     CHECK(Path("foo/..").lexically_normal() == ".");
//     CHECK(Path("ab/cd/ef/../../qw").lexically_normal() == "ab/qw");
//     CHECK(Path("a/b/../../../c").lexically_normal() == "../c");
//     CHECK(Path("../").lexically_normal() == "..");

// 	// lexically_relative()
//     CHECK(Path("/a/d").lexically_relative("/a/b/c") == "../../d");
//     CHECK(Path("/a/b/c").lexically_relative("/a/d") == "../b/c");
//     CHECK(Path("a/b/c").lexically_relative("a") == "b/c");
//     CHECK(Path("a/b/c").lexically_relative("a/b/c/x/y") == "../..");
//     CHECK(Path("a/b/c").lexically_relative("a/b/c") == ".");
//     CHECK(Path("a/b").lexically_relative("c/d") == "../../a/b");
//     CHECK(Path("a/b").lexically_relative("a/") == "b");

//  // lexically_proximate()
//     CHECK(Path("/a/d").lexically_proximate("/a/b/c") == "../../d");
//     CHECK(Path("a/d").lexically_proximate("/a/b/c") == "a/d");

//     CHECK(Path("/foo").lexically_proximate("bar") == "/foo");
//     CHECK(Path("foo").lexically_proximate("/bar") == "foo");
// }

// TEST_CASE( "Path - Non member functions", "[class][path][.]" ) {
// {
//     Path p1("foo/bar");
//     Path p2("some/other");
//     fs::swap(p1, p2);
//     CHECK(p1 == "some/other");
//     CHECK(p2 == "foo/bar");
//     CHECK(hash_value(p1));
//     CHECK(p2 < p1);
//     CHECK(p2 <= p1);
//     CHECK(p1 <= p1);
//     CHECK(!(p1 < p2));
//     CHECK(!(p1 <= p2));
//     CHECK(p1 > p2);
//     CHECK(p1 >= p2);
//     CHECK(p1 >= p1);
//     CHECK(!(p2 > p1));
//     CHECK(!(p2 >= p1));
//     CHECK(p1 != p2);
//     CHECK(p1 / p2 == "some/other/foo/bar");
// }