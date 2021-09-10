#ifndef ERROR_CODE_HPP
#define ERROR_CODE_HPP

#include <string>

namespace ft {

class error_code {
public:
	// constructors
	error_code() throw();
	// error_code(int val, const error_category& cat) throw();
	// template<class ErrorCodeEnum>
	// 	error_code(ErrorCodeEnum e) throw();
	error_code(int errc) throw();

	// modifiers
	void assign(int val) throw();
	// void assign(int val, const error_category& cat) throw();
	// template<class ErrorCodeEnum>
	// 	error_code& operator=(ErrorCodeEnum e) throw();
	error_code& operator=(int e) throw();
	void clear() throw();

	// observers
	int value() const throw();
	// const error_category& category() const throw();
	// error_condition default_error_condition() const throw();
	std::string message() const;
	operator bool() const throw();
 
private:
	int _val;
    // const error_category* cat_; // exposition only
};

} /* namespace ft*/

#endif /* ERROR_CODE_HPP */
