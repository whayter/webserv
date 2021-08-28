
#ifndef SYNTAX_ERROR_HPP
#define SYNTAX_ERROR_HPP

#include <exception>
#include <string>

class SyntaxError: public std::exception
{
	public:
		SyntaxError() {}
		SyntaxError(const char * message)		: _message(message) {}
		SyntaxError(const std::string& message)	: _message(message) {}
		virtual ~SyntaxError() throw() {}

		virtual const char* what() const throw() {
			return _message.c_str();
		}

	protected:
		std::string _message;
};

#endif /* SYNTAX_ERROR_HPP */