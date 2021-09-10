#ifndef FILE_STATUS_HPP
#define FILE_STATUS_HPP

namespace ft { 
namespace filesystem { 

class file_status {
  public:
    // constructors and destructor
    file_status() noexcept : file_status(file_type::none) {}
    explicit file_status(file_type ft,
                         perms prms = perms::unknown) noexcept;
    file_status(const file_status&) noexcept = default;
    file_status(file_status&&) noexcept = default;
    ~file_status();
 
    // assignments
    file_status& operator=(const file_status&) noexcept = default;
    file_status& operator=(file_status&&) noexcept = default;
 
    // modifiers
    void       type(file_type ft) noexcept;
    void       permissions(perms prms) noexcept;
 
    // observers
    file_type  type() const noexcept;
    perms      permissions() const noexcept;
 
    friend bool operator==(const file_status& lhs, const file_status& rhs) noexcept
      { return lhs.type() == rhs.type() && lhs.permissions() == rhs.permissions(); }
  };

} /* namespace filesystem */
} /* namespace ft */

#endif /* FILE_STATUS_HPP */
