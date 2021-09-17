#ifndef  RETURN_DIRECTIVE_HPP
#define RETURN_DIRECTIVE_HPP

#include <string>
#include "Uri.hpp"

class ReturnDirective
{
public:
	ReturnDirective() : _code(0), _uri() {}
	// ReturnDirective(const ReturnDirective& other) :
	// 	_code(other._code),
	// 	_uri(other._uri),
	// 	_text(other._text)
	// {}

	inline u_short 				getCode() const	{ return _code; }
	inline const Uri&			getUri() const	{ return _uri; }
	inline const std::string& 	getText() const	{ return _text; }

	void setCode(u_short code)				{ _code = code; }
	void setUri(const std::string& uri)		{ _uri = uri; }
	void setText(const std::string& text)	{ _text = text; }

	inline bool hasCode() const	{ return _code != 0; }
	inline bool hasUri() const	{ return !_uri.empty(); }
	inline bool hasText() const { return !_text.empty(); }

private:
	u_short 	_code;
	Uri			_uri;
	std::string _text;
}; /* class ReturnDirective */

#endif /* RETURN_DIRECTIVE_HPP */
