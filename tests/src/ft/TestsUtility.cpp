#include "catch_amalgamated.hpp"
#include "utility.hpp"

using namespace ft;
using namespace ft::filesystem;

TEST_CASE( "ft::comparePaths", "[namespace][ft][utility][pathsComponentsAreEqual]" )
{
	size_t nSameComp;
	CHECK(pathsComponentsAreEqual("/youtube/", "/youtube/", nSameComp) );
	CHECK( nSameComp == 3);
	CHECK(!pathsComponentsAreEqual("/youtube/", "/youtube", nSameComp) );
	CHECK( nSameComp == 2);
	CHECK(pathsComponentsAreEqual("/youtube", "/youtube", nSameComp) );
	CHECK( nSameComp == 2);
	CHECK(!pathsComponentsAreEqual("/youtube", "/youtub", nSameComp) );
	CHECK( nSameComp == 1);
	CHECK(!pathsComponentsAreEqual("/youtube", "/", nSameComp) );
	CHECK( nSameComp == 1);
	CHECK(!pathsComponentsAreEqual("/youtube", "youtube", nSameComp) );
	CHECK( nSameComp == 0);
}