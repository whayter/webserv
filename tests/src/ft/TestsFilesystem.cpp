#include "catch_amalgamated.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/error_code.hpp"
#include "ft/system_error.hpp"
#include "ft/filesystem/perms.hpp"
#include "ft/filesystem/filesystem_error.hpp"
#include <filesystem>

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

// TEST_CASE("fs::permissions - permissions", "[namespace][ft][filesystem][struct][perms]")
// {
//     TemporaryDirectory t(TempOpt::change_path);
//     std::error_code ec;
//     generateFile("foo", 512);
//     auto allWrite = fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write;
//     CHECK_NOTHROW(fs::permissions("foo", allWrite, fs::perm_options::remove));
//     CHECK((fs::status("foo").permissions() & fs::perms::owner_write) != fs::perms::owner_write);
//     {
//         CHECK_THROWS_AS(fs::resize_file("foo", 1024), fs::filesystem_error);
//         CHECK(fs::file_size("foo") == 512);
//     }
//     CHECK_NOTHROW(fs::permissions("foo", fs::perms::owner_write, fs::perm_options::add));
//     CHECK((fs::status("foo").permissions() & fs::perms::owner_write) == fs::perms::owner_write);
//     CHECK_NOTHROW(fs::resize_file("foo", 2048));
//     CHECK(fs::file_size("foo") == 2048);
//     CHECK_THROWS_AS(fs::permissions("bar", fs::perms::owner_write, fs::perm_options::add), fs::filesystem_error);
//     CHECK_NOTHROW(fs::permissions("bar", fs::perms::owner_write, fs::perm_options::add, ec));
//     CHECK(ec);
//     CHECK_THROWS_AS(fs::permissions("bar", fs::perms::owner_write, static_cast<fs::perm_options>(0)), fs::filesystem_error);
// }
