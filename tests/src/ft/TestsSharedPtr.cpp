#include "catch_amalgamated.hpp"
#include "shared_ptr.hpp"
#include <string>

TEST_CASE("ft::shared_ptr", "[shared_ptr]")
{
	ft::shared_ptr<std::string> str;
	CHECK( str.getReferenceCount() == 0);
	
	ft::shared_ptr<std::string> str2(new std::string("hola"));
	CHECK( str2.getReferenceCount() == 1);
	ft::shared_ptr<std::string> str3(str2);
	
	CHECK( str.getReferenceCount() == 0);
	CHECK( str2.getReferenceCount() == 2);
	CHECK( str3.getReferenceCount() == 2);

	REQUIRE( *str2 == "hola" );
	REQUIRE( str2.get() == str3.get() );
	REQUIRE( *str2 == *str3 );
	REQUIRE( str2->size() == str3->size() );

	ft::shared_ptr<std::string> str4(new std::string("bye"));
	REQUIRE( str.get() == NULL );
	REQUIRE( str.getReferenceCount() == 0 );
	
	REQUIRE( *str2 == *str3 );
	REQUIRE( str2.getReferenceCount() == 2);
	REQUIRE( str3.getReferenceCount() == 2);
	REQUIRE( *str3 != *str4 );
	REQUIRE( str4.getReferenceCount() == 1);
	REQUIRE( *str4 == "bye" );
}