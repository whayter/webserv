#include "ft/filesystem/directory_iterator.hpp"

#include "ft/filesystem/path.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/error_code.hpp"



namespace ft
{
namespace filesystem
{

directory_iterator::directory_iterator() throw()
    : _path(path()), _dirp(0), _dirEntry(0), _options(directory_options::none)
{

}

directory_iterator::directory_iterator(const path &p)
    : _path(p), _dirp(0), _dirEntry(0), _options(directory_options::none)
{}

directory_iterator::directory_iterator(const path &p, directory_options options)
    : _path(p), _dirp(0), _dirEntry(0), _options(options)
{}

directory_iterator::directory_iterator(const path &p, error_code &ec) throw()
    : _path(p), _dirp(0), _dirEntry(0), _options(directory_options::none)
{
    (void)ec;
}
directory_iterator::directory_iterator(const path &p, directory_options options, error_code &ec) throw()
    : _path(p), _dirp(0), _dirEntry(0), _options(directory_options::none)
{
    (void)ec;
}
directory_iterator::directory_iterator(const directory_iterator &rhs)
{
    (void)rhs;
}
// directory_iterator::directory_iterator(directory_iterator &&rhs) throw()
// {

// }

directory_iterator::~directory_iterator()
{
    if(_dirp)
        ::closedir(_dirp);
}

directory_iterator& directory_iterator::operator=(const directory_iterator &rhs)
{
    (void)rhs;
}
// directory_iterator& directory_iterator::operator=(directory_iterator &&rhs) throw() {}

const directory_entry& directory_iterator::operator*() const
{
    return _dir;
}
const directory_entry* directory_iterator::operator->() const
{
    
}
directory_iterator& directory_iterator::operator++()
{

}
directory_iterator& directory_iterator::increment(error_code &ec) throw()
{
    if (_dirp == NULL)
        return;
    
    _dirEntry._path = _path;

}


}; /* namespace filesystem */
}; /* namespace ft */
