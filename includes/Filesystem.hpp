#ifndef


/*
    filesystem synopsis

namespace std { namespace filesystem {

    class path;
    class directory_entry;
    class directory_iterator;


    directory_iterator begin(directory_iterator iter) noexcept;
    directory_iterator end(const directory_iterator&) noexcept;

    class file_status;

    struct space_info
    {
      uintmax_t capacity;
      uintmax_t free;
      uintmax_t available;
    };

    enum class file_type;
    enum class perms;
    enum class perm_options;
    enum class copy_options;
    enum class directory_options;

    path absolute(const path& p);
    path absolute(const path& p, error_code &ec);

    path canonical(const path& p);
    path canonical(const path& p, error_code& ec);

    path current_path();
    path current_path(error_code& ec);
    void current_path(const path& p);
    void current_path(const path& p, error_code& ec) noexcept;

    bool exists(file_status s) noexcept;
    bool exists(const path& p);
    bool exists(const path& p, error_code& ec) noexcept;

    bool equivalent(const path& p1, const path& p2);
    bool equivalent(const path& p1, const path& p2, error_code& ec) noexcept;

    uintmax_t    file_size(const path& p);
    uintmax_t    file_size(const path& p, error_code& ec) noexcept;

    bool is_block_file(file_status s) noexcept;
    bool is_block_file(const path& p);
    bool is_block_file(const path& p, error_code& ec) noexcept;

    bool is_character_file(file_status s) noexcept;
    bool is_character_file(const path& p);
    bool is_character_file(const path& p, error_code& ec) noexcept;

    bool is_directory(file_status s) noexcept;
    bool is_directory(const path& p);
    bool is_directory(const path& p, error_code& ec) noexcept;

    bool is_empty(const path& p);
    bool is_empty(const path& p, error_code& ec) noexcept;

    bool is_other(file_status s) noexcept;
    bool is_other(const path& p);
    bool is_other(const path& p, error_code& ec) noexcept;

    bool is_regular_file(file_status s) noexcept;
    bool is_regular_file(const path& p);
    bool is_regular_file(const path& p, error_code& ec) noexcept;


    path proximate(const path& p, error_code& ec);
    path proximate(const path& p, const path& base = current_path());
    path proximate(const path& p, const path& base, error_code &ec);


    path relative(const path& p, error_code& ec);
    path relative(const path& p, const path& base=current_path());
    path relative(const path& p, const path& base, error_code& ec);


    space_info   space(const path& p);
    space_info   space(const path& p, error_code& ec) noexcept;

    file_status  status(const path& p);
    file_status  status(const path& p, error_code& ec) noexcept;

    path temp_directory_path();
    path temp_directory_path(error_code& ec);


} }  // namespaces std::filesystem

*/