#ifndef  RETURN_DIRECTIVE_HPP
#define RETURN_DIRECTIVE_HPP

#include "Uri.hpp"

#include <string>
#include <stdint.h>

class ReturnDirective
{
public:
	ReturnDirective() : _code(0), _uri() {}

	inline u_short 				getCode() const;
	inline const Uri&			getUri() const;
	inline const std::string& 	getText() const;

	void setCode(u_short code);
	void setUri(const std::string& uri);
	void setText(const std::string& text);

	inline bool hasCode() const;
	inline bool hasUri() const;
	inline bool hasText() const;

private:
	u_short 	_code;
	Uri			_uri;
	std::string _text;
}; /* class ReturnDirective */

inline bool 				ReturnDirective::hasCode() const	{ return _code != 0; }
inline bool 				ReturnDirective::hasUri() const		{ return !_uri.empty(); }
inline bool 				ReturnDirective::hasText() const	{ return !_text.empty(); }
inline u_short 				ReturnDirective::getCode() const	{ return _code; }
inline const Uri&			ReturnDirective::getUri() const		{ return _uri; }
inline const std::string& 	ReturnDirective::getText() const	{ return _text; }

#endif /* RETURN_DIRECTIVE_HPP */
