#include "ft/filesystem/directory_entry.hpp"

#include "ft/filesystem/filesystem_error.hpp"
#include "ft/filesystem/file_status.hpp"
#include "ft/filesystem/filesystem.hpp"
#include "ft/filesystem/path.hpp"
#include "ft/error_code.hpp"


namespace ft{
namespace filesystem {

// constructors and destructor
directory_entry::directory_entry() throw()
	: _path(filesystem::path())
{}
directory_entry::directory_entry(const directory_entry& other):
	_path(other._path),
	_fileSize(other._fileSize),
	_hardLinkCount(other._hardLinkCount)
{}

// directory_entry(directory_entry&&) throw()
// {}
directory_entry::directory_entry(const filesystem::path& p)
	: _path(p)
{
	refresh();
}
directory_entry::directory_entry(const filesystem::path& p, error_code& ec)
	: _path(p)
{
	refresh(ec);     
}
directory_entry::~directory_entry() {}

// assignments
directory_entry& directory_entry::operator=(const directory_entry& other)
{
	if (this == &other)
		return *this;
	_path = other._path;
	_status = other._status;
	_fileSize = other._fileSize;
	_hardLinkCount = other._hardLinkCount;
	return *this;
}
// directory_entry& operator=(directory_entry&&) throw();

// modifiers
void directory_entry::assign(const filesystem::path& p) {
	_path = p;
	error_code ec;
	refresh(ec);
	if (ec)
		throw filesystem_error("assign::refresh() : " + ec.message(), _path, ec);
}
void directory_entry::assign(const filesystem::path& p, error_code& ec)
{
	_path = p;
	refresh(ec);
}
// void directory_entry::replace_filename(const filesystem::path& p);
// void directory_entry::replace_filename(const filesystem::path& p, error_code& ec);
void directory_entry::refresh(){
	error_code ec;
	refresh(ec);
	if (ec)
		throw filesystem_error("directory_entry::refresh() : " + ec.message(), _path, ec);
}
void directory_entry::refresh(error_code& ec) throw()
{
	ec.clear();
	_status = filesystem::status(_path, ec);
}

// observers
const path& directory_entry::path() const throw(){
	return _path;
}
directory_entry::operator const class path&() const throw()
{
	return _path;
}
bool			directory_entry::exists() const
{
	error_code ec;
	bool result = exists(ec);
	if (ec)
		throw filesystem_error("directory_entry::exists() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::exists(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::exists(_status);
	return filesystem::exists(_path, ec);
}
bool			directory_entry::is_block_file() const
{
	error_code ec;
	bool result = is_block_file(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_block_file() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_block_file(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::block;
	return _status.type() == file_type::block;
}
bool			directory_entry::is_character_file() const
{
	error_code ec;
	bool result = is_character_file(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_character_file() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_character_file(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::block;
	return _status.type() == file_type::block;
}
bool			directory_entry::is_directory() const
{
	error_code ec;
	bool result = is_directory(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_directory() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_directory(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::directory;
	return _status.type() == file_type::directory;
}
bool			directory_entry::is_fifo() const
{
	error_code ec;
	bool result = is_fifo(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_fifo() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_fifo(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::fifo;
	return _status.type() == file_type::fifo;
}
bool			directory_entry::is_other() const
{
	error_code ec;
	bool result = is_other(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_other() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_other(error_code& ec) const throw()
{
	file_status s = _status;
	ec.clear();
	if (!status_known(s.type()))
		s = filesystem::status(_path, ec);
	return filesystem::is_other(s);
}
bool			directory_entry::is_regular_file() const
{
	error_code ec;
	bool result = is_regular_file(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_regular_file() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_regular_file(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::regular;
	return _status.type() == file_type::regular;
}
bool			directory_entry::is_socket() const
{
	error_code ec;
	bool result = is_socket(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_socket() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_socket(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::socket;
	return _status.type() == file_type::socket;
}
bool			directory_entry::is_symlink() const
{
	error_code ec;
	bool result = is_symlink(ec);
	if (ec)
		throw filesystem_error("directory_entry::is_symlink() : " + ec.message(), _path, ec);
	return result;
}
bool			directory_entry::is_symlink(error_code& ec) const throw()
{
	ec.clear();
	if (!status_known(_status.type()))
		return filesystem::status(_path, ec).type() == file_type::symlink;
	return _status.type() == file_type::symlink;
}
// uintmax_t        directory_entry::file_size() const{}
// uintmax_t        directory_entry::file_size(error_code& ec) const throw(){}
// uintmax_t        directory_entry::hard_link_count() const{}
// uintmax_t        directory_entry::hard_link_count(error_code& ec) const throw(){}
// file_time_type   directory_entry::last_write_time() const{}
// file_time_type   directory_entry::last_write_time(error_code& ec) const throw(){}
file_status directory_entry::status() const {
	error_code ec;
	file_status result = status(ec);
	if (result.type() == file_type::none)
		throw filesystem_error("directory_entry::status(): " + ec.message(), _path, ec);
	return result;
}
file_status directory_entry::status(error_code& ec) const throw(){
	return filesystem::status(_path, ec);
}
// file_status symlink_status() const;
// file_status symlink_status(error_code& ec) const throw();

// bool operator==(const directory_entry& rhs) const throw();

// inserter
// template<class charT, class traits>
//   friend basic_ostream<charT, traits>&
// 	operator<<(basic_ostream<charT, traits>& os, const directory_entry& d);

} /* namespace filesystem */
} /* namespace ft */
