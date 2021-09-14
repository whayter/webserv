#ifndef DIRECTORY_OPTION_HPP
#define DIRECTORY_OPTION_HPP

namespace ft {
namespace filesystem { 

// equivalent to enum class ... (c++98 don't have enum class)
struct directory_options{
	enum directory_optionsEnum {
		none,
		follow_directory_symlink,
		skip_permission_denied
	};

	directory_options(directory_optionsEnum e): _e(e) {}
	directory_options(unsigned int e) {
		_e = static_cast<directory_optionsEnum>(e);
	}
	// operator directory_optionsEnum() const throw(){
	//   return _e;
	// }
	friend bool operator==(const directory_options& lhs, const directory_options& rhs){
	  return lhs._e == rhs._e;
	}
	friend bool operator!=(const directory_options& lhs, const directory_options& rhs){
	  return lhs._e != rhs._e;
	}
	friend directory_options  operator&(directory_options x, directory_options y) throw(){
		return x._e & y._e;
	}
	friend directory_options  operator|(directory_options x, directory_options y) throw(){
		return x._e | y._e;
	}
	friend directory_options  operator^(directory_options x, directory_options y) throw(){
		return x._e ^ y._e;
	}
	friend directory_options  operator~(directory_options x) throw(){
		return ~x._e;
	}
private:
  directory_optionsEnum _e;
};

} /* namespace filesystem */
} /* namespace ft */

#endif /* DIRECTORY_OPTION_HPP */
