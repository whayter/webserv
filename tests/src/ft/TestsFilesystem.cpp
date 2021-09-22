#include "catch_amalgamated.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/error_code.hpp"
#include "ft/system_error.hpp"
#include "ft/filesystem/perms.hpp"
#include "ft/filesystem/filesystem_error.hpp"
#include "ft/filesystem/file_status.hpp"
#include "ft/filesystem/directory_entry.hpp"
#include "ft/filesystem/directory_iterator.hpp"
#include <filesystem>
#include <fstream>

namespace fs = ft::filesystem;
namespace fx = std::filesystem;

enum class TempOpt { none, change_path };
class TemporaryDirectory
{
public:
    TemporaryDirectory(TempOpt opt = TempOpt::none)
    {
        static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        static auto rng = std::bind(std::uniform_int_distribution<int>(0, 35), std::mt19937(static_cast<unsigned int>(seed) ^ static_cast<unsigned int>(reinterpret_cast<ptrdiff_t>(&opt))));
        std::string filename;
        do {
            filename = "test_";
            for (int i = 0; i < 8; ++i) {
                filename += "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[rng()];
            }
            _path = fx::canonical(fx::temp_directory_path()) / filename;
        } while (fx::exists(_path));
        fx::create_directories(_path);
        _orig_dir = fx::current_path();
        if (opt == TempOpt::change_path) {
            fx::current_path(_path);
        }


    }

    ~TemporaryDirectory()
    {
        if (!_orig_dir.empty()) {
            fx::current_path(_orig_dir);
        }
        fx::remove_all(_path);
    }

    const fx::path& path() const { return _path; }

private:
    fx::path _path;
    fx::path _orig_dir;
};


static void generateFile(const fs::path& pathname, int withSize = -1)
{
    std::ofstream outfile(pathname);
    if (withSize < 0) {
        outfile << "Hello world!" << std::endl;
    }
    else {
        outfile << std::string(size_t(withSize), '*');
    }
}

// TEST_CASE("Temporary Directory", "[tempdir]")
// {
//     fs::path tempPath;
//     {
//         TemporaryDirectory t;
//         tempPath = t.path();
//         REQUIRE(fs::exists(fs::path(t.path())));
//         REQUIRE(fs::is_directory(t.path()));
//     }
//     REQUIRE(!fs::exists(tempPath));
// }

TEST_CASE("fs::current_path - current_path", "[namespace][ft][filesystem][current_path]")
{
    TemporaryDirectory t;
    ft::error_code ec;
    fs::path p1 = fs::current_path();
    CHECK_NOTHROW(fs::current_path(t.path().c_str()));
    CHECK(p1.string() != fs::current_path().string());
    CHECK_NOTHROW(fs::current_path(p1, ec));
    CHECK(!ec);
    CHECK(ec.value() == 0);
    REQUIRE_THROWS_AS(fs::current_path(fs::path(t.path().c_str()) / "foo"), fs::filesystem_error);
    try{
        fs::current_path(fs::path(t.path().c_str()) / "foo");
    }
    catch (const fs::filesystem_error& e)
    {
        CHECK( e.code().value() == ft::errc::no_such_file_or_directory );
        CHECK( !e.path1().string().empty());
        CHECK( e.path2().string().empty());
        CHECK( e.what() == std::string("current_path(const path&): No such file or directory"));
    }
    CHECK(p1.string() == fs::current_path().string());
    CHECK_NOTHROW(fs::current_path(fs::path(t.path().c_str()) / "foo", ec));
    CHECK(ec);
    CHECK(ec.value() == ft::errc::no_such_file_or_directory);
}

TEST_CASE("fs::absolute - absolute", "[namespace][ft][filesystem][absolute]")
{
    CHECK(fs::absolute("").string() == (fs::current_path() / "").string());
    CHECK(fs::absolute(fs::current_path()).string() == fs::current_path().string());
    CHECK(fs::absolute(".").string() == (fs::current_path() / ".").string());
    CHECK((fs::absolute("..") == fs::current_path().parent_path() || fs::absolute("..") == fs::current_path() / ".."));
    CHECK(fs::absolute("foo").string() == (fs::current_path() / "foo").string());
    ft::error_code ec;
    CHECK(fs::absolute("", ec).string() == (fs::current_path() / "").string());
    CHECK(!ec);
    CHECK(fs::absolute("foo", ec).string() == (fs::current_path() / "foo").string());
    CHECK(!ec);
}

TEST_CASE("fs::perms ", "[namespace][ft][filesystem][struct][perms]")
{
    // fs::perms pOwnerAll = fs::perms::owner_read | fs::perms::owner_write | fs::perms::owner_exec;

    // static_assert(pOwnerAll == fs::perms::owner_all, "const didn't result in owner_all");
    CHECK((fs::perms::owner_read | fs::perms::owner_write | fs::perms::owner_exec) == fs::perms::owner_all);
    CHECK((fs::perms::group_read | fs::perms::group_write | fs::perms::group_exec) == fs::perms::group_all);
    CHECK((fs::perms::others_read | fs::perms::others_write | fs::perms::others_exec) == fs::perms::others_all);
    CHECK((fs::perms::owner_all | fs::perms::group_all | fs::perms::others_all) == fs::perms::all);
    CHECK((fs::perms::all | fs::perms::set_uid | fs::perms::set_gid | fs::perms::sticky_bit) == fs::perms::mask);
}

TEST_CASE("fs::status_known - status_known", "[namespace][ft][filesystem][status_known]")
{
    CHECK(!fs::status_known(fs::file_status()));
    CHECK(fs::status_known(fs::file_status(fs::file_type::not_found)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::regular)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::directory)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::symlink)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::character)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::fifo)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::socket)));
    CHECK(fs::status_known(fs::file_status(fs::file_type::unknown)));
}

TEST_CASE("fs::status - status", "[namespace][ft][filesystem][status]")
{
    TemporaryDirectory t(TempOpt::change_path);
    ft::error_code ec;
    fs::file_status fs;
    CHECK_NOTHROW(fs = fs::status("foo"));
    CHECK(fs.type() == fs::file_type::not_found);
    CHECK(fs.permissions() == fs::perms::unknown);
    CHECK_NOTHROW(fs = fs::status("bar", ec));
    CHECK(fs.type() == fs::file_type::not_found);
    CHECK(fs.permissions() == fs::perms::unknown);
    CHECK(ec);
    ec.clear();
    fs = fs::status(t.path().c_str());
    CHECK(fs.type() == fs::file_type::directory);
    CHECK((fs.permissions() & (fs::perms::owner_read | fs::perms::owner_write)) == (fs::perms::owner_read | fs::perms::owner_write));
    generateFile("foobar");
    fs = fs::status(fs::path(t.path().c_str()) / "foobar");
    CHECK(fs.type() == fs::file_type::regular);
    CHECK((fs.permissions() & (fs::perms::owner_read | fs::perms::owner_write)) == (fs::perms::owner_read | fs::perms::owner_write));
    fx::create_symlink(t.path() / "foobar", t.path() / "barfoo");
    fs = fs::status(fs::path(t.path().c_str()) / "barfoo");
    CHECK(fs.type() == fs::file_type::regular);
    CHECK((fs.permissions() & (fs::perms::owner_read | fs::perms::owner_write)) == (fs::perms::owner_read | fs::perms::owner_write));
}

TEST_CASE("fs::directory_entry - class directory_entry", "[namespace][ft][filesystem][directory_entry]")
{
    TemporaryDirectory t;
    ft::error_code ec;
    fs::directory_entry de = fs::directory_entry(t.path().c_str());
    CHECK(de.path().string() == t.path().string());
    CHECK((fs::path)de == fs::path(t.path().c_str()));
    CHECK(de.exists());
    CHECK(!de.is_block_file());
    CHECK(!de.is_character_file());
    CHECK(de.is_directory());
    CHECK(!de.is_fifo());
    CHECK(!de.is_other());
    CHECK(!de.is_regular_file());
    CHECK(!de.is_socket());
    CHECK(!de.is_symlink());
    CHECK(de.status().type() == fs::file_type::directory);
    ec.clear();
    CHECK(de.status(ec).type() == fs::file_type::directory);
    CHECK(!ec);
    CHECK_NOTHROW(de.refresh());

    fs::directory_entry none;
    CHECK_THROWS_AS(none.refresh(), fs::filesystem_error);
    ec.clear();
    CHECK_NOTHROW(none.refresh(ec));
    CHECK(ec);
    CHECK_THROWS_AS(de.assign(""), fs::filesystem_error);
    ec.clear();
    CHECK_NOTHROW(de.assign("", ec));
    CHECK(ec);
    generateFile(fs::path(t.path().c_str()) / "foo", 1234);
//     auto now = fs::file_time_type::clock::now();
    CHECK_NOTHROW(de.assign(fs::path(t.path().c_str()) / "foo"));
    CHECK_NOTHROW(de.assign(fs::path(t.path().c_str()) / "foo", ec));
    CHECK(!ec);
    de = fs::directory_entry(fs::path(t.path().c_str()) / "foo");
    CHECK(de.path() == fs::path(t.path().c_str()) / "foo");
    CHECK(de.exists());
    CHECK(de.exists(ec));
    CHECK(!ec);
    CHECK(!de.is_block_file());
    CHECK(!de.is_block_file(ec));
    CHECK(!ec);
    CHECK(!de.is_character_file());
    CHECK(!de.is_character_file(ec));
    CHECK(!ec);
    CHECK(!de.is_directory());
    CHECK(!de.is_directory(ec));
    CHECK(!ec);
    CHECK(!de.is_fifo());
    CHECK(!de.is_fifo(ec));
    CHECK(!ec);
    CHECK(!de.is_other());
    CHECK(!de.is_other(ec));
    CHECK(!ec);
    CHECK(de.is_regular_file());
    CHECK(de.is_regular_file(ec));
    CHECK(!ec);
    CHECK(!de.is_socket());
    CHECK(!de.is_socket(ec));
    CHECK(!ec);
    CHECK(!de.is_symlink());
    CHECK(!de.is_symlink(ec));
    CHECK(!ec);
    CHECK(de.file_size() == 1234);
    CHECK(de.file_size(ec) == 1234);
//     CHECK(std::abs(std::chrono::duration_cast<std::chrono::seconds>(de.last_write_time() - now).count()) < 3);
    ec.clear();
//     CHECK(std::abs(std::chrono::duration_cast<std::chrono::seconds>(de.last_write_time(ec) - now).count()) < 3);
    CHECK(!ec);
    CHECK(de.hard_link_count() == 1);
    CHECK(de.hard_link_count(ec) == 1);
    CHECK(!ec);
//     CHECK_THROWS_AS(de.replace_filename("bar"), fs::filesystem_error);
//     CHECK_NOTHROW(de.replace_filename("foo"));
//     ec.clear();
//     CHECK_NOTHROW(de.replace_filename("bar", ec));
//     CHECK(ec);

    fs::directory_entry de2none = fs::directory_entry();
    ec.clear();
    CHECK(de2none.hard_link_count(ec) == static_cast<uintmax_t>(-1));
    CHECK_THROWS_AS(de2none.hard_link_count(), fs::filesystem_error);
    CHECK(ec);
    ec.clear();
//     CHECK_NOTHROW(de2none.last_write_time(ec));
//     CHECK_THROWS_AS(de2none.last_write_time(), fs::filesystem_error);
//     CHECK(ec);
    ec.clear();
    CHECK_THROWS_AS(de2none.file_size(), fs::filesystem_error);
    CHECK(de2none.file_size(ec) == static_cast<uintmax_t>(-1));
    CHECK(ec);
    ec.clear();
    CHECK(de2none.status().type() == fs::file_type::not_found);
    CHECK(de2none.status(ec).type() == fs::file_type::not_found);
    CHECK(ec);
    generateFile(fs::path(t.path().c_str()) / "a");
    generateFile(fs::path(t.path().c_str()) / "b");
    fs::directory_entry  d1 = fs::directory_entry(fs::path(t.path().c_str()) / "a");
    fs::directory_entry  d2 = fs::directory_entry(fs::path(t.path().c_str()) / "b");
    CHECK(d1 < d2);
    CHECK(!(d2 < d1));
    CHECK(d1 <= d2);
    CHECK(!(d2 <= d1));
    CHECK(d2 > d1);
    CHECK(!(d1 > d2));
    CHECK(d2 >= d1);
    CHECK(!(d1 >= d2));
    CHECK(d1 != d2);
    CHECK(!(d2 != d2));
    CHECK(d1 == d1);
    CHECK(!(d1 == d2));
}

TEST_CASE("fs::is_empty", "[namespace][ft][filesystem][is_empty]")
{
    TemporaryDirectory t(TempOpt::change_path);
    ft::error_code ec;
    // not implemented for directories...
    // CHECK(fs::is_empty(fs::path(t.path().c_str())));
    // CHECK(fs::is_empty(fs::path(t.path().c_str()), ec));
    // CHECK(!ec);
    generateFile("foo", 0);
    generateFile("bar", 1234);
    CHECK(fs::is_empty("foo"));
    CHECK(fs::is_empty("foo", ec));
    CHECK(!ec);
    CHECK(!fs::is_empty("bar"));
    CHECK(!fs::is_empty("bar", ec));
    CHECK(!ec);
    CHECK_THROWS_AS(fs::is_empty("foobar"), fs::filesystem_error);
    bool result = false;
    CHECK_NOTHROW(result = fs::is_empty("foobar", ec));
    CHECK(!result);
    CHECK(ec);
}

TEST_CASE("fs::directory_iterator - class directory_iterator", "[namespace][ft][filesystem][directory_iterator][.]")
{
    {
        TemporaryDirectory t;
        {
            fs::directory_iterator e = fs::directory_iterator();
            (void)e;
        }
        CHECK( fs::directory_iterator( fs::path(t.path().c_str()) ) == fs::directory_iterator());
        generateFile(fs::path(t.path().c_str()) / "test", 1234);
        REQUIRE(fs::directory_iterator( fs::path(t.path().c_str())) != fs::directory_iterator());
        // auto iter = fs::directory_iterator(fs::path(t.path().c_str()));
        // fs::directory_iterator iter2(iter);
        // fs::directory_iterator iter3, iter4;
        // iter3 = iter;
        // CHECK(iter->path().filename() == "test");
        // CHECK(iter2->path().filename() == "test");
        // CHECK(iter3->path().filename() == "test");
        // iter4 = std::move(iter3);
        // CHECK(iter4->path().filename() == "test");
        // CHECK(iter->path() == fs::path(t.path().c_str()) / "test");
        // CHECK(!iter->is_symlink());
        // CHECK(iter->is_regular_file());
        // CHECK(!iter->is_directory());
        // CHECK(iter->file_size() == 1234);
        // CHECK(++iter == fs::directory_iterator());
        // CHECK_THROWS_AS(fs::directory_iterator(fs::path(t.path().c_str()) / "non-existing"), fs::filesystem_error);
        // int cnt = 0;
        // for(auto de : fs::directory_iterator(fs::path(t.path().c_str()))) {
        //     ++cnt;
        // }
        // CHECK(cnt == 1);
    }
    // if (is_symlink_creation_supported()) {
    //     TemporaryDirectory t;
    //     fs::path td = t.path() / "testdir";
    //     CHECK(fs::directory_iterator(t.path()) == fs::directory_iterator());
    //     generateFile(t.path() / "test", 1234);
    //     fs::create_directory(td);
    //     REQUIRE_NOTHROW(fs::create_symlink(t.path() / "test", td / "testlink"));
    //     std::error_code ec;
    //     REQUIRE(fs::directory_iterator(td) != fs::directory_iterator());
    //     auto iter = fs::directory_iterator(td);
    //     CHECK(iter->path().filename() == "testlink");
    //     CHECK(iter->path() == td / "testlink");
    //     CHECK(iter->is_symlink());
    //     CHECK(iter->is_regular_file());
    //     CHECK(!iter->is_directory());
    //     CHECK(iter->file_size() == 1234);
    //     CHECK(++iter == fs::directory_iterator());
    // }
    // {
    //     // Issue #8: check if resources are freed when iterator reaches end()
    //     TemporaryDirectory t(TempOpt::change_path);
    //     auto p = fs::path("test/");
    //     fs::create_directory(p);
    //     auto iter = fs::directory_iterator(p);
    //     while (iter != fs::directory_iterator()) {
    //         ++iter;
    //     }
    //     CHECK(fs::remove_all(p) == 1);
    //     CHECK_NOTHROW(fs::create_directory(p));
    // }
}
