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
	std::string txt;
	if (status == http::Status::Created)
		txt = "The content you posted has been created ";
	else
		txt = "The content you required has been moved ";
	Builder text("p", txt);
	text.addChild(a("here", rdir.getUri().toString()));
	body.addChild(text);
	html.addChild(head);
	html.addChild(body);
	return ft::vectorizeString(html.str());
}

std::vector<unsigned char> buildErrorPage(http::Status error)
{
	Builder html("html");
	std::string title = ft::intToString(error.getValue());
	title += " - " + error.getDefinition();
	Builder head("head");
	head.addChild("title", title);
	Builder body("body");
	body.addAttribute("style", "text-align:center;");
	body.addChild("h1", title);
	body.addChild("hr", "");
	body.addChild("p", "Webserv");
	html.addChild(head);
	html.addChild(body);
	return ft::vectorizeString(html.str());
}

std::vector<unsigned char> buildAutoindexPage(const ft::filesystem::path& path, const Uri& uri)
{
	Builder html("html");
	Builder head("head");
	std::string title = "index of ";
	title += uri.getPath();
	head.addChild("title", title);
	Builder body("body");
	Builder pre("pre");
	pre.addChild(a("../", "../"));
	ft::filesystem::directory_iterator it(path);
	for (; it != ft::filesystem::directory_iterator(); ++it)
		pre.addChild(a(it->path().filename(), it->path().filename()));
	body.addChild("h1", title);
	body.addChild("hr","");
	body.addChild(pre);
	body.addChild("hr","");
	html.addChild(head);
	html.addChild(body);
	return ft::vectorizeString(html.str());
}

std::vector<unsigned char> buildSimplePage(std::string content)
{
	Builder html("html");
	Builder head("head");
	head.addChild("title", content);
	Builder body("body");
	body.addChild("p", content);
	html.addChild(head);
	html.addChild(body);
	return ft::vectorizeString(html.str());
}

}; /* namespace html */