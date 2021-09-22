#ifndef HTML_BUILDER
#define HTML_BUILDER

#include <string>
#include <sstream>
#include <vector>

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
		ss << "<" << _name << _attributesToString() << ">" << std::endl;
		if (!_content.empty())
		{
			ss << std::string((depth+1) * INDENT_SIZE, ' ');
			ss << _content <<std::endl;
		}
		{
			std::vector<Element>::iterator it = _childs.begin();
			while (it != _childs.end())
				ss << it++->str(depth + 1);
		}
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

		ss << " ";
		while (it != _attributes.end())
			ss << it->name << "=\"" << it++->value << "\" ";
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
	Builder(std::string rootName) { _root._name = rootName;}
	
	Builder *addChild(const std::string& name, const std::string& content) {
		_root._childs.emplace_back(Element(name, content));
		return this;
	}
	Builder *addChild(const Element& elem) {
		_root._childs.emplace_back(elem);
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

} /* namespace html */

#endif /* HTML_BUILDER */
