
#include "error_code.hpp"
#include <string.h>

namespace ft {

error_code::error_code() throw()
	: _val(0) {}

// template<class ErrorCodeEnum>
// error_code::error_code(ErrorCodeEnum e) throw()
// 	: _val(e) {}

error_code::error_code(int errc) throw()
	: _val(errc) {}

void	error_code::assign(int val) throw(){
	_val = val;
}

template<class ErrorCodeEnum>
error_code& error_code::operator=(ErrorCodeEnum e) throw(){
	assign(e);
	return *this;
}

void error_code::clear() throw(){
	_val = 0;
}

int	error_code::value() const throw(){
	return _val;
}
std::string error_code::message() const{
	return strerror(value());
}

error_code::operator bool() const throw(){
	return value() == 0;
}

} /* namespace ft*/
