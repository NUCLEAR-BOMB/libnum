#include <doctest/doctest.h>

#include <libnum/int128.hpp>

namespace {

using namespace libnum;

TEST_CASE("int128::int128") {
    SUBCASE("from int64_t") {
        int128 x{1};
        CHECK(x == 1);
        CHECK(x != 2);

        int128 y{-1};
        CHECK(y.low() == std::uint64_t(-1));
        CHECK(y.high() == -1);
        CHECK(y == -1);

        int128 z{-2};
        CHECK(z.low() == std::uint64_t(-2));
        CHECK(z.high() == -1);
        CHECK(z == -2);
    }
    SUBCASE("from double int64_t") {
        int128 x{-1, 5};
        CHECK(x == int128{-1, 5});
    }
}

TEST_CASE("int128::operator+") {
    CHECK(int128{1} + int128{2} == int128{3});
    CHECK(int128{0} + int128{2} == int128{2});
    CHECK(int128{-1} + int128{1} == int128{0});
    CHECK(int128{-1, 0} + int128{1, 0} == int128{0, 0});
    CHECK(int128{0, std::uint64_t(-1)} + int128{2} == int128{1, 1});
    CHECK(int128{0, std::uint64_t(-1)} + int128{0, std::uint64_t(-1)} == int128{1, std::uint64_t(-2)});
}

}
