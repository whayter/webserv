#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP

struct file_typeScoped{
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
};
typedef file_typeScoped::file_typeEnum file_type;

#endif /* FILE_TYPE_HPP */
