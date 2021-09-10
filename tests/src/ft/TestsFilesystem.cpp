#include "catch_amalgamated.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/error_code.hpp"
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
        if (opt == TempOpt::change_path) {
            _orig_dir = fx::current_path();
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
    CHECK_THROWS_AS(fs::current_path(fs::path(t.path().c_str()) / "foo"), fs::filesystem_error);
    CHECK(p1 == fs::current_path());
    CHECK_NOTHROW(fs::current_path(fs::path(t.path().c_str()) / "foo", ec));
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
