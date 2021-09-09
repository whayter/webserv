#ifndef ERROR_CODE_HPP
#define ERROR_CODE_HPP

#include "ft/system_error.hpp"
#include <string>
#include <string.h>

namespace ft {

class error_code {
public:
	// constructors
	error_code() throw(): _val(0) {}
	// error_code(int val, const error_category& cat) throw();
	template<class ErrorCodeEnum>
		error_code(ErrorCodeEnum e) throw(): _val(e) {}

	// modifiers
	void assign(int val) throw() { _val = val;}
	// void assign(int val, const error_category& cat) throw();
	template<class ErrorCodeEnum>
	error_code& operator=(ErrorCodeEnum e) throw() {
		assign(e);
	}
	void clear() throw() { _val = 0;}

	// observers
	int value() const throw() {return _val;}
	// const error_category& category() const throw();
	// error_condition default_error_condition() const throw();
	std::string message() const { return strerror(value());}
	operator bool() const throw() {return value() == 0;}
 
private:
	int _val;
    // const error_category* cat_; // exposition only
};

  // non-member functions
  error_code make_error_code(errc e) throw();

//   template<class charT, class traits>
//     basic_ostream<charT, traits>&
//       operator<<(basic_ostream<charT, traits>& os, const error_code& ec);
} /* namespace ft*/

#endif /* ERROR_CODE_HPP */
