#include "ft/filesystem/filesystem.hpp"

#include "ft/filesystem/filesystem_error.hpp"
#include "ft/filesystem/file_status.hpp"
#include "ft/filesystem/file_type.hpp"
#include "ft/system_error.hpp"
#include "ft/error_code.hpp"

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ft { namespace filesystem {

void swap(path& lhs, path& rhs) throw() 
{
	path tmp(lhs);
	lhs = rhs;
	rhs = tmp;
}

bool operator==(const path& lhs, const path& rhs) throw() {return lhs._path == rhs._path;}
bool operator< (const path& lhs, const path& rhs) throw() {return lhs._path < rhs._path;}
bool operator!=(const path& lhs, const path& rhs) throw() { return !(lhs == rhs); }
bool operator<=(const path& lhs, const path& rhs) throw() { return !(rhs < lhs); }
bool operator> (const path& lhs, const path& rhs) throw() { return rhs < lhs; }
bool operator>=(const path& lhs, const path& rhs) throw() { return !(lhs < rhs); }
path operator/ (const path& lhs, const path& rhs) { return path(lhs) /= rhs;}
 
path current_path()
{
	error_code ec;
	path result = current_path(ec);
	if (ec.value())
		throw filesystem_error("current_path(): " + ec.message() , ec);
	return result;
}

path current_path(error_code& ec)
{
	std::string result;
	char cwd[PATH_MAX];
	if (::getcwd(cwd, sizeof(cwd)) != NULL)
	{
		result = cwd;
		ec.clear();
	}
	else
		ec = make_error_code();
	return path(result);
}

void current_path(const path& p)
{
	error_code ec;
	current_path(p, ec);
	if (ec.value())
		throw filesystem_error("current_path(const path&): " + ec.message(), p, ec);
}

void current_path(const path& p, error_code& ec) throw()
{
	if (::chdir(p.c_str()) ==  -1)
		ec = make_error_code();
	else
		ec.clear();
}

path absolute(const path& p)
{
	error_code ec;
	path result = absolute(p, ec);
	if (ec.value())
		throw filesystem_error("absolute(const path&): " + ec.message(), p, ec);
	return result;
}

path absolute(const path& p, error_code& ec)
{
	path result = current_path(ec) / p;
	if (ec.value())
		result = path();
	return result;
}

bool status_known(file_status s) throw() 
{
	return s.type() != file_type::none;
}

static file_status stModeToFileStatus(mode_t mode)
{
	file_type type = file_type::unknown;
	if (S_ISREG(mode))
		type = file_type::regular;
	else if (S_ISDIR(mode))
		type = file_type::directory;
	else if (S_ISCHR(mode))
		type = file_type::character;
	else if (S_ISBLK(mode))
		type = file_type::block;
	else if (S_ISFIFO(mode))
		type = file_type::fifo;
	else if (S_ISLNK(mode))
		type = file_type::symlink;
	else if (S_ISSOCK(mode))
		type = file_type::socket;
	return file_status(type, mode & 0xfff);
}

file_status status(const path& p){
	error_code ec;
	file_status result = status(p, ec);
	if (result.type() == file_type::none)
		throw filesystem_error("status(const path& p=\""+ p.string() +"\"): " + ec.message(), p, ec);
	return result;
}

file_status status(const path& p, error_code& ec) throw(){
	struct stat st;
	if (::stat(p.c_str(), &st) == -1)
	{
		ec = make_error_code();
		if (ec.value() == errc::no_such_file_or_directory)
			return file_status(file_type::not_found);
		return file_status();
	}
	ec.clear();
	return stModeToFileStatus(st.st_mode);
}


// void t(){

//    ec.clear();
//     struct ::stat st;
//     auto result = ::lstat(p.c_str(), &st);
//     if (result == 0) {
//         ec.clear();
//         file_status fs = detail::file_status_from_st_mode(st.st_mode);
//         if (sls) {
//             *sls = fs;
//         }
//         if (fs.type() == file_type::symlink) {
//             result = ::stat(p.c_str(), &st);
//             if (result == 0) {
//                 fs = detail::file_status_from_st_mode(st.st_mode);
//             }
//             else {
//                 ec = detail::make_system_error();
//                 if (detail::is_not_found_error(ec)) {
//                     return file_status(file_type::not_found, perms::unknown);
//                 }
//                 return file_status(file_type::none);
//             }
//         }
//         if (sz) {
//             *sz = static_cast<uintmax_t>(st.st_size);
//         }
//         if (nhl) {
//             *nhl = st.st_nlink;
//         }
//         if (lwt) {
//             *lwt = st.st_mtime;
//         }
//         return fs;
//     }
//     else {
//         ec = detail::make_system_error();
//         if (detail::is_not_found_error(ec)) {
//             return file_status(file_type::not_found, perms::unknown);
//         }
//         return file_status(file_type::none);
//     }
//  }



// bool exists(file_status s) throw(){
// 	return status_known(s) && s.type() != file_type::not_found;
// }

// bool exists(const path& p){

// }

// bool exists(const path& p, error_code& ec) throw(){

// }



} /* namespace filesystem */
} /* namespace ft */
