#include "ft/filesystem/directory_iterator.hpp"

#include "ft/filesystem/path.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/error_code.hpp"



namespace ft
{
namespace filesystem
{

directory_iterator::directory_iterator() throw()
    : _path(path()), _dirP(0), _dirEntry(0), _options(directory_options::none)
{

}

directory_iterator::directory_iterator(const path &p)
    : _path(p), _dirP(0), _dirEntry(0), _options(directory_options::none)
{}

directory_iterator::directory_iterator(const path &p, directory_options options)
    : _path(p), _dirP(0), _dirEntry(0), _options(options)
{}

directory_iterator::directory_iterator(const path &p, error_code &ec) throw()
    : _path(p), _dirP(0), _dirEntry(0), _options(directory_options::none)
{
    
}
directory_iterator::directory_iterator(const path &p, directory_options options, error_code &ec) throw()
{}
directory_iterator::directory_iterator(const directory_iterator &rhs)
{}
directory_iterator::directory_iterator(directory_iterator &&rhs) throw()
{}
directory_iterator::~directory_iterator()
{
    if(_dp)
        ::closedir(_dp);
}

directory_iterator& directory_iterator::operator=(const directory_iterator &rhs)
{
    
}
// directory_iterator& directory_iterator::operator=(directory_iterator &&rhs) throw() {}

const directory_entry& directory_iterator::operator*() const
{

}
const directory_entry* directory_iterator::operator->() const
{

}
directory_iterator& directory_iterator::operator++()
{

}
directory_iterator& directory_iterator::increment(error_code &ec) throw()
{

}


}; /* namespace filesystem */
}; /* namespace ft */
