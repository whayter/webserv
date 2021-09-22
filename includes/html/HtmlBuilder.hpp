#ifndef HTML_BUILDER
#define HTML_BUILDER

#include <string>
#include <sstream>
#include <vector>

#include "filesystem.h"

#define INDENT_SIZE 2 

namespace html{

class Builder;
struct Attribute
{
	std::string name;
	std::string value;

	Attribute(const std::string& name, const std::string& value)
		: name(name), value(value)
	{}
};

class Element
{
public:
	std::string str(size_t depth = 0)
	{
		std::ostringstream ss;

		ss << std::string(depth * INDENT_SIZE, ' ');
		ss << "<" << _name << _attributesToString() << ">";
		if (_childs.size())
			ss << std::endl;
		if (!_content.empty())
		{
			if (_childs.size())
				ss << std::string((depth+1) * INDENT_SIZE, ' ');
			ss << _content;
			if (_childs.size())
				ss << std::endl;
		}
		{
			std::vector<Element>::iterator it = _childs.begin();
			while (it != _childs.end())
				ss << it++->str(depth + 1);
		}
		if (_childs.size())
			ss << std::string(depth * INDENT_SIZE, ' ');
		ss << "</" << _name << ">" << std::endl;
		return ss.str();
	}

private:
	Element() {}
	Element(const std::string& name, const std::string& content)
		: _name(name), _content(content)
	{}
	Element(const std::string& name, const Element& child)
		: _name(name)
	{
		_childs.emplace_back(child);
	}

	std::string _attributesToString(){
		std::ostringstream ss;

		std::vector<Attribute>::iterator it = _attributes.begin();

		if (_attributes.size())
			ss << " ";
		while (it != _attributes.end())
		{
			ss << it->name << "=\"" << it->value << "\"";
			if (it+1 != _attributes.end())
				ss << " ";
			++it;
		}
		return ss.str();
	}

	friend class Builder;

	std::string _name;
	std::string _content;
	std::vector<Attribute> _attributes;
	std::vector<Element> _childs;
}; /* class Element */

class Builder
{
public:
	Builder(const std::string& rootName) { _root._name = rootName;}
	Builder(const std::string& rootName, const std::string& rootContent)
	{
		_root._name = rootName;
		_root._content = rootContent;
	}
	
	Builder *addChild(const std::string& name, const std::string& content) {
		_root._childs.emplace_back(Element(name, content));
		return this;
	}
	Builder *addChild(const Element& elem) {
		_root._childs.emplace_back(elem);
		return this;
	}
	Builder *addChild(const Builder* b) {
		_root._childs.emplace_back(b->_root);
		return this;
	}
	Builder *addChild(std::string name, Element child) {
		Element e(name, "");
		e._childs.push_back(child);
		_root._childs.emplace_back(e);
		return this;
	}

	Builder *addAttribute(const std::string& name, const std::string& value) {
		_root._attributes.emplace_back(Attribute(name, value));
		return this;
	}
	Builder *addAttribute(const Attribute& attribute) {
		_root._attributes.emplace_back(attribute);
		return this;
	}
	std::string str(){ return _root.str();}
	operator Element()	{ return _root;}
private:


	Element _root;
}; /* class Builder */


std::string make_autoindex(const ft::filesystem::path& path)
{
	html::Builder head = html::Builder("head");
	head.addChild("title", "Index of /webserv");

	html::Builder body = html::Builder("body");
	html::Builder pre = html::Builder("pre");
	pre.addChild( html::Builder("a", "../").addAttribute("href", "../"));
	{
		ft::filesystem::directory_iterator it(path);
		while (it != ft::filesystem::directory_iterator())
		{
			pre.addChild(
				html::Builder("a", it->path().filename()).addAttribute("href", it->path().filename())
			);
			++it;
		}
	}
	body.addChild("h1", "Index of /webserv");
	body.addChild("hr","");
	body.addChild(pre);
	body.addChild("hr","");
	return html::Builder("html").addChild(head)->addChild(body)->str();	
}

} /* namespace html */

#endif /* HTML_BUILDER */
