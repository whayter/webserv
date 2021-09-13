#ifndef DIRECTORY_ENTRY_HPP
#define DIRECTORY_ENTRY_HPP

#include <stdint.h>
#include <ostream>

#include "ft/filesystem/path.hpp"
#include "ft/filesystem/file_status.hpp"

namespace ft
{
	class error_code;

	namespace filesystem
	{
		class directory_entry
		{
		public:
			// constructors and destructor
			directory_entry() throw();
			directory_entry(const directory_entry &);
			// directory_entry(directory_entry&&) throw();
			directory_entry(const path &p);
			directory_entry(const path &p, ::ft::error_code &ec);
			~directory_entry();

			// assignments
			directory_entry &operator=(const directory_entry &);
			// directory_entry& operator=(directory_entry&&) throw();

			// modifiers
			void assign(const path &p);
			void assign(const path &p, error_code &ec);
			// void replace_filename(const path &p);
			// void replace_filename(const path &p, error_code &ec);
			void refresh();
			void refresh(error_code &ec) throw();

			// observers
			const path& 	path() const throw();
			operator const class  path&() const throw();
			bool exists() const;
			bool exists(error_code &ec) const throw();
			bool is_block_file() const;
			bool is_block_file(error_code& ec) const throw();
			bool is_character_file() const;
			bool is_character_file(error_code& ec) const throw();
			bool is_directory() const;
			bool is_directory(error_code& ec) const throw();
			bool is_fifo() const;
			bool is_fifo(error_code& ec) const throw();
			bool is_other() const;
			bool is_other(error_code& ec) const throw();
			bool is_regular_file() const;
			bool is_regular_file(error_code& ec) const throw();
			bool is_socket() const;
			bool is_socket(error_code& ec) const throw();
			bool is_symlink() const;
			bool is_symlink(error_code& ec) const throw();
			uintmax_t file_size() const;
			uintmax_t file_size(error_code& ec) const throw();
			uintmax_t hard_link_count() const;
			uintmax_t hard_link_count(error_code& ec) const throw();
			// file_time_type last_write_time() const;
			// file_time_type last_write_time(error_code& ec) const throw();
			file_status status() const;
			file_status status(error_code &ec) const throw();
			// file_status symlink_status() const;
			// file_status symlink_status(error_code& ec) const throw();

			// bool operator==(const directory_entry &rhs) const throw();

			// inserter
			// template<class charT, class traits>
			//   friend basic_ostream<charT, traits>&
			// 	operator<<(basic_ostream<charT, traits>& os, const directory_entry& d);

		private:
			friend class directory_iterator; // exposition only

			class path			_path;		 // exposition only
			file_status 		_status;
			uintmax_t			_fileSize;
			uintmax_t			_hardLinkCount;
			// time_t				_lastWriteTime;
		}; /* class directory_entry */

	}; /* namespace filesystem */
}; /* namespace ft */

#endif /* DIRECTORY_ENTRY_HPP */
