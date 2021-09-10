#include "catch_amalgamated.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/filesystem/filesystem_error.hpp"
#include <filesystem>

namespace fs = ft::filesystem;

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
            _path = std::filesystem::canonical(std::filesystem::temp_directory_path()) / filename;
        } while (std::filesystem::exists(_path));
        std::filesystem::create_directories(_path);
        if (opt == TempOpt::change_path) {
            _orig_dir = std::filesystem::current_path();
            std::filesystem::current_path(_path);
        }
    }

    ~TemporaryDirectory()
    {
        if (!_orig_dir.empty()) {
            std::filesystem::current_path(_orig_dir);
        }
        std::filesystem::remove_all(_path);
    }

    const std::filesystem::path& path() const { return _path; }

private:
    std::filesystem::path _path;
    std::filesystem::path _orig_dir;
};

TEST_CASE( "fs::filesystem_error", "[namespace][ft][filesystem][filesystem_error]" )
{
}

TEST_CASE("fs::current_path - current_path", "[namespace][ft][filesystem][current_path]")
{
//  TemporaryDirectory t;
    std::error_code ec;
    fs::path p1 = fs::current_path();
    // CHECK_NOTHROW(fs::current_path(t.path().c_str()));
    // CHECK(p1 != fs::current_path());
    // CHECK_NOTHROW(fs::current_path(p1, ec));
    // CHECK(!ec);
    // CHECK_THROWS_AS(fs::current_path(t.path() / "foo"), fs::filesystem_error);
    // CHECK(p1 == fs::current_path());
    // CHECK_NOTHROW(fs::current_path(t.path() / "foo", ec));
    // CHECK(ec);
}

// TEST_CASE("fs::absolute - absolute", "[namespace][ft][filesystem][absolute]")
// {
//     CHECK(fs::absolute("") == fs::current_path() / "");
//     CHECK(fs::absolute(fs::current_path()) == fs::current_path());
//     CHECK(fs::absolute(".") == fs::current_path() / ".");
//     CHECK((fs::absolute("..") == fs::current_path().parent_path() || fs::absolute("..") == fs::current_path() / ".."));
//     CHECK(fs::absolute("foo") == fs::current_path() / "foo");
//     std::error_code ec;
//     CHECK(fs::absolute("", ec) == fs::current_path() / "");
//     CHECK(!ec);
//     CHECK(fs::absolute("foo", ec) == fs::current_path() / "foo");
//     CHECK(!ec);
// }
