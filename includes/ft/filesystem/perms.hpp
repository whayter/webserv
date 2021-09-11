#ifndef PERMS_HPP
#define PERMS_HPP

namespace ft { namespace filesystem { 

struct perms{
	enum permsEnum {
		none		=  0,		//
		owner_read	=  0400,	// S_IRUSR
		owner_write	=  0200,	// S_IWUSR
		owner_exec	=  0100,	// S_IXUSR
		owner_all	=  0700,	// S_IRWXU
		group_read	=   040,	// S_IRGRP
		group_write	=   020,	// S_IWGRP
		group_exec	=   010,	// S_IXGRP
		group_all	=   070,	// S_IRWXG
		others_read	=    04,	// S_IROTH
		others_write=    02,	// S_IWOTH
		others_exec	=    01,	// S_IXOTH
		others_all	=    07,	// S_IRWXO
		all			=  0777,	// equivalent to: owner_all | group_all | others_all
		set_uid		= 04000,	// S_ISUID 
		set_gid		= 02000,	// S_ISGID
		sticky_bit	= 01000,	// S_ISVTX
		mask		= 07777,	// equivalent to: all | set_uid | set_gid | sticky_bit
		unknown		= 0xFFFF,	// 
	};
	perms(permsEnum e): _e(e) {}
	perms(unsigned int e) {
		_e = static_cast<perms>(e);
	}
	operator permsEnum() const throw(){
		return _e;
    }
    friend bool operator==(const perms& lhs, const perms& rhs){
      return lhs._e == rhs._e;
    }
private:
	permsEnum _e;
};

// perms  operator&(perms x, perms y) throw() {
// 	return static_cast<perms>(
// 		static_cast<uint32_t>(x) & static_cast<uint32_t>(y)
// 		);
// }

// perms operator|(perms x, perms y) throw() {
// 	return static_cast<perms>(
// 		static_cast<uint32_t>(x) | static_cast<uint32_t>(y)
// 		);
// }

// perms operator^(perms x, perms y) throw() {
// 	return static_cast<perms>(
// 		static_cast<uint32_t>(x) ^ static_cast<uint32_t>(y)
// 		);
// }

// perms operator~(perms x) throw(){
// 	return static_cast<perms>(
// 		~static_cast<uint32_t>(x)
// 		);
// }

// inline perms& operator&=(perms& x, perms y) throw() { return x = x & y; }
// inline perms& operator|=(perms& x, perms y) throw() { return x = x | y; }
// inline perms& operator^=(perms& x, perms y) throw() { return x = x ^ y; }

} /* namespace filesystem */
} /* namespace ft */

#endif /* PERMS_HPP */
