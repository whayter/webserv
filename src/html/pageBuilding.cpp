#include "pageBuilding.hpp"
#include "HtmlBuilder.hpp"
#include "ReturnDirective.hpp"
#include "Status.hpp"
#include "utility.hpp"

#include "ServerConfig.hpp"

namespace html {

Builder link(std::string rel, std::string type, std::string href)
{
	Builder link("link");
	link.addAttribute("rel", rel);
	link.addAttribute("type", type);
	link.addAttribute("href", href);
	return link;
}

Builder a(std::string content, std::string href)
{
	Builder a("a", content);
	a.addAttribute("href", href);
	return a;
}

std::vector<unsigned char> buildRedirectionPage(const ReturnDirective &rdir)
{
	Builder html("html");
	http::Status status(rdir.getCode());
	std::string title = ft::intToString(status.getValue());
	title += " " + status.getDefinition();
	Builder head("head");
	head.addChild("title", title);
	Builder body("body");
	body.addAttribute("style", "text-align:center;");
	body.addChild("h1", title);
	body.addChild("hr", "");
	Builder text("p", "The content you required has been moved ");
	text.addChild(a("here", rdir.getUri().toString()));
	body.addChild(text);
	html.addChild(head);
	html.addChild(body);
	return ft::vectorizeString(html.str());
}

std::string buildErrorPage(http::Status error)
{
	std::string title = ft::intToString(error.getValue());
	title += " - " + error.getDefinition();
	Builder head("head");
	head.addChild("title", title);
	Builder body("body");
	body.addAttribute("style", "text-align:center;");
	body.addChild("h1", title);
	body.addChild("hr", "");
	body.addChild("p", "Webserv");
	return Builder("html").addChild(head)->addChild(body)->str();	
}

std::string buildAutoindexPage(const ft::filesystem::path& path)
{
	Builder head("head");
	std::string title = "index of ";
	title += &(path.c_str()[1]);
	head.addChild("title", title);
	Builder body("body");
	Builder pre("pre");
	std::string backHref = "/";
	backHref += path.parent_path().string();
	pre.addChild(a("../", backHref));
	ft::filesystem::directory_iterator it(path);
	for (; it != ft::filesystem::directory_iterator(); ++it)
	{
		std::string href = it->path().parent_path().filename();
		href += "/";
		href += it->path().filename();
		pre.addChild(a(it->path().filename(), href));
	}
	body.addChild("h1", title);
	body.addChild("hr","");
	body.addChild(pre);
	body.addChild("hr","");
	return Builder("html").addChild(head)->addChild(body)->str();	
}

}; /* namespace html */