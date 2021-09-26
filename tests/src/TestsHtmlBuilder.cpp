#include "catch_amalgamated.hpp"

#include "HtmlBuilder.hpp"
#include "filesystem.h"

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

TEST_CASE("html::Builder2", "[namespace][html][builder][element]")
{
	std::ifstream file;
	file.open("./html/htmlBuilder2.html", std::ifstream::in);
	std::string expected((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	html::Builder html = html::Builder("html");
	html::Builder head = html::Builder("head");
	html::Builder body = html::Builder("body");
	html::Builder pre = html::Builder("pre");
	pre.addChild( html::Builder("a", "../").addAttribute("href", "../"));
	{
		ft::filesystem::directory_iterator it("./one_file");
		while (it != ft::filesystem::directory_iterator())
		{
			pre.addChild(
				html::Builder("a", it->path().filename()).addAttribute("href", it->path().filename())
			);
			++it;
		}
	}
	head.addChild("title", "Index of /webserv");
	body.addChild("h1", "Index of /webserv");
	body.addChild("hr","");
	body.addChild(pre);
	body.addChild("hr","");
	html.addChild(head)->addChild(body);	
	CHECK (html.str() == expected);
}

/////////////////////////////////////////////
// creer un nouveau fichier TestsMessageBuilder.cpp pour cette fonction
#include "MessageBuilder.hpp"
TEST_CASE("html::make_autoindex", "[namespace][html][builder][element][make_autoindex]")
{
	std::ifstream file;
	file.open("./html/htmlBuilder2.html", std::ifstream::in);
	std::string expected((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	
	CHECK (http::make_autoindex("./one_file") == expected);
}