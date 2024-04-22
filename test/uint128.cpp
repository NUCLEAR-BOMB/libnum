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

// TEST_CASE("uint128::operator/") {
//     uint128 x = 10;
// 
//     CHECK(x / 2 == 5);
//     CHECK(x / 3 == 3);
//     CHECK(x / 10 == 1);
//     CHECK(x / 11 == 0);
// 
//     CHECK(uint128{1, 0} / 2 == 9223372036854775808);
//     CHECK(uint128{1, 8} / 2 == 9223372036854775808 + 4);
//     CHECK(uint128{2, 0} / 2 == 0);
// }

TEST_CASE("uint128::operator&") {
    CHECK_EQ(uint128{0b1111} & uint128{0b1111}, uint128{0b1111});
    CHECK_EQ(uint128{0b1111} & uint128{0b1110}, uint128{0b1110});
    CHECK_EQ(uint128{0b1101} & uint128{0b1110}, uint128{0b1100});

    CHECK_EQ(uint128{0b1111, 0b1111} & uint128{0b1111}, uint128{0b1111});
    CHECK_EQ(uint128{0b1111, 0b1111} & uint128{0b1100, 0b1111}, uint128{0b1100, 0b1111});
}

TEST_CASE("uint128::operator|") {
    CHECK_EQ(uint128{0b0000} | uint128{0b0000}, uint128{0b0000});
    CHECK_EQ(uint128{0b0000} | uint128{0b1010}, uint128{0b1010});
    CHECK_EQ(uint128{0b1000} | uint128{0b1010}, uint128{0b1010});

    CHECK_EQ(uint128{0b1010, 0b0000} | uint128{0b0000, 0b1111}, uint128{0b1010, 0b1111});
}

}
