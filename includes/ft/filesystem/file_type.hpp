#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP

namespace ft { namespace filesystem { 

// equivalent to enum class ... (c++98 don't have enum class)
struct file_type{
    enum file_typeEnum {
      none = 0,
      not_found = -1,
      regular = 1,
      directory = 2,
      symlink = 3,
      block = 4,
      character = 5,
      fifo = 6,
      socket = 7,
      unknown = 8
    };
    file_type(file_typeEnum e): _e(e) {}
    operator file_typeEnum() const throw(){
      return _e;
    }
    friend bool operator==(const file_type& lhs, const file_type& rhs){
      return lhs._e == rhs._e;
    }
private:
  file_typeEnum _e;
};

} /* namespace filesystem */
} /* namespace ft */

#endif /* FILE_TYPE_HPP */
