#include <doctest/doctest.h>

#include <libnum/uint128.hpp>
#include <charconv>

namespace doctest {
    template<> struct StringMaker<libnum::uint128> {
        static String convert(const libnum::uint128& value) {
            return "[" + doctest::toString(value.high) + " " + doctest::toString(value.low) + "]";
        }
    };
}

namespace {

using namespace libnum;

TEST_CASE("uint128::operator+") {
    SUBCASE("small") {
        uint128 x = 123;
        uint128 y = 456;

        CHECK(x + y == 579);
        CHECK(y + x == 579);

        CHECK(x + 10 == 133);
        CHECK(5 + x  == 128);
    }
    SUBCASE("large") {
        uint128 x = uint64_t(-1);
        uint128 y = 1;

        CHECK(x + y == uint128{1, 0});
        y = 100;
        CHECK(x + y == uint128{1, 99});
        y = uint64_t(-1) - 5;
        CHECK(x + y == uint128{1, uint64_t(-1) - 6});
    }
}

TEST_CASE("uint128::operator-") {
    uint128 x = 100;
    uint128 y = 55;

    CHECK(x - y == 45);
    CHECK(y - x == uint128{uint64_t(-1), uint64_t(-45)});

    CHECK(x - 5 == 95);
    CHECK(x - 100 == 0);
    CHECK(100 - x == 0);
    CHECK(105 - x == 5);
    CHECK(123 - x == 23);
}

TEST_CASE("uint128::operator*") {
    uint128 x = 5;
    uint128 y = 10;

    CHECK(x * y == 50);
    CHECK(y * x == 50);

    CHECK(x * 100 == 500);

    x = uint64_t(-10000);
    CHECK(x * 5 == uint128{4, uint64_t(-10000) * 5});
}

}
