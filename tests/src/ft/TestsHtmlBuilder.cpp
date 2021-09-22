#include "catch_amalgamated.hpp"

#include "HtmlBuilder.hpp"

#include <fstream>
#include <string>

TEST_CASE("html::Builder", "[namespace][html][builder][element]")
{
	std::ifstream file;
	file.open("./html/htmlBuilder.html", std::ifstream::in);
	std::string expected((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	html::Builder builder = html::Builder("html");
	builder.addAttribute("attr1", "v1")->addAttribute("attr2", "v2");
	builder.addChild("head", "content")->addChild("body", "test");
	CHECK (builder.str() == expected);

}
