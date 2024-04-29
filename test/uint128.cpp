#include <doctest/doctest.h>

#include <libnum/uint128.hpp>
#include <charconv>
#include <vector>

#include "numeric_limits_check.hpp"

namespace doctest {
    template<> struct StringMaker<libnum::uint128> {
        static String convert(const libnum::uint128& value) {
            return "[" + doctest::toString(value.high()) + " " + doctest::toString(value.low()) + "]";
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
    CHECK(uint128{0} * uint128{0} == uint128{0});
    CHECK(uint128{1} * uint128{0} == uint128{0});
    CHECK(uint128{1} * uint128{1} == uint128{1});
    CHECK(uint128{1, 1} * uint128{1} == uint128{1, 1});
    CHECK(uint128{1, 1} * uint128{2} == uint128{2, 2});
    CHECK(uint128{1, 2} * uint128{2} == uint128{2, 4});
    CHECK(uint128{1, 1} * uint128{1, 0} == uint128{1, 0});
    CHECK(uint128{1, 1} * uint128{2, 0} == uint128{2, 0});
    CHECK(uint128{1, 2} * uint128{2, 0} == uint128{4, 0});
}

TEST_CASE("uint128::operator/") {
    SUBCASE("uint64") {
        CHECK(uint128{1} / 1 == uint128{1});
        CHECK(uint128{2} / 1 == uint128{2});
        CHECK(uint128{2} / 2 == uint128{1});
        CHECK(uint128{2} / 3 == uint128{0});
        CHECK(uint128{1, 1} / 1 == uint128{1, 1});
        CHECK(uint128{1, 2} / 1 == uint128{1, 2});
        CHECK(uint128{1, 2} / 2 == uint128{0, 1});
        CHECK(uint128{3, 2} / 2 == uint128{1, 1});
    }
}

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

TEST_CASE("uint128::operator^") {
    CHECK_EQ(uint128{0b0000} ^ uint128{0b0000}, uint128{0b0000});
    CHECK_EQ(uint128{0b0000} ^ uint128{0b1010}, uint128{0b1010});
    CHECK_EQ(uint128{0b1000} ^ uint128{0b1010}, uint128{0b0010});

    CHECK_EQ(uint128{0b1010, 0b0000} ^ uint128{0b0000, 0b1111}, uint128{0b1010, 0b1111});
}

TEST_CASE("uint128::operator<<") {
    CHECK_EQ(uint128{0} << 0, uint128{0});
    CHECK_EQ(uint128{0} << 1, uint128{0});
    CHECK_EQ(uint128{0} << 63, uint128{0});
    CHECK_EQ(uint128{0} << 64, uint128{0});
    CHECK_EQ(uint128{0} << 65, uint128{0});
    CHECK_EQ(uint128{0} << 127, uint128{0});

    CHECK_EQ(uint128{1} << 1, uint128{2});
    CHECK_EQ(uint128{1} << 2, uint128{4});
    CHECK_EQ(uint128{1} << 3, uint128{8});
    CHECK_EQ(uint128{1} << 63, uint128{1ULL << 63});
    CHECK_EQ(uint128{1} << 64, uint128{1, 0});
    CHECK_EQ(uint128{1} << 65, uint128{2, 0});
    CHECK_EQ(uint128{1} << 127, uint128{1ULL << 63, 0});
    CHECK_EQ(uint128{1} << 128, uint128{1});

    CHECK_EQ(uint128{1, 1} << 1, uint128{2, 2});
    CHECK_EQ(uint128{1, 1} << 63, uint128{1ULL << 63, 1ULL << 63});
    CHECK_EQ(uint128{1, 1} << 64, uint128{1, 0});
    CHECK_EQ(uint128{1, 1} << 127, uint128{1ULL << 63, 0});
    CHECK_EQ(uint128{1, 1} << 128, uint128{1, 1});
}

TEST_CASE("uint128::operator>>") {
    CHECK_EQ(uint128{0} >> 0, uint128{0});
    CHECK_EQ(uint128{0} >> 1, uint128{0});
    CHECK_EQ(uint128{0} >> 63, uint128{0});
    CHECK_EQ(uint128{0} >> 64, uint128{0});
    CHECK_EQ(uint128{0} >> 65, uint128{0});
    CHECK_EQ(uint128{0} >> 127, uint128{0});

    CHECK_EQ(uint128{1} >> 1, uint128{0});
    CHECK_EQ(uint128{2} >> 1, uint128{1});
    CHECK_EQ(uint128{1ULL << 63} >> 63, uint128{1});
    CHECK_EQ(uint128{1ULL << 63} >> 64, uint128{0});
    CHECK_EQ(uint128{1, 0} >> 64, uint128{1});
    CHECK_EQ(uint128{1, 0} >> 65, uint128{0});
    CHECK_EQ(uint128{1ULL << 63, 0} >> 127, uint128{1});
    CHECK_EQ(uint128{1ULL << 63, 0} >> 128, uint128{1ULL << 63, 0});
}

TEST_CASE("uint128::operator~") {
    CHECK(~uint128{1} == uint128{~0ULL, ~0ULL ^ 1});
    CHECK(~uint128{0, ~0ULL} == uint128{~0ULL, 0});
    CHECK(~uint128{~0ULL, 0} == uint128{0, ~0ULL});
    CHECK(~uint128{~0ULL, ~0ULL} == uint128{0});
}

TEST_CASE("uint128::operator&=") {
    uint128 x = 0b1101;
    x &= 0b1010;
    CHECK(x == 0b1000);
}
TEST_CASE("uint128::operator|=") {
    uint128 x = 0b1001;
    x |= 0b1010;
    CHECK(x == 0b1011);
}
TEST_CASE("uint128::operator^=") {
    uint128 x = 0b1001;
    x ^= 0b1010;
    CHECK(x == 0b0011);
}
TEST_CASE("uint128::operator<<=") {
    uint128 x = 0b1001;
    x <<= 2;
    CHECK(x == 0b100100);
}
TEST_CASE("uint128::operator>>=") {
    uint128 x = 0b1001;
    x >>= 2;
    CHECK(x == 0b10);
}

TEST_CASE("uint128::operator>") {
    CHECK_FALSE(uint128{0} > uint128{0});
    CHECK(uint128{1} > uint128{0});
    CHECK(uint128{1ULL << 63} > uint128{0});
    CHECK(uint128{1, 0} > uint128{0});
    CHECK(uint128{1ULL << 63, 0} > uint128{0});

    CHECK_FALSE(uint128{0} > uint128{1});
    CHECK_FALSE(uint128{1, 0} > uint128{2, 0});
    CHECK_FALSE(uint128{1, 1} > uint128{2, 0});
}

TEST_CASE("uint128::operator>=") {
    CHECK(uint128{0} >= uint128{0});
    CHECK(uint128{1} >= uint128{0});
    CHECK(uint128{1ULL << 63} >= uint128{0});
    CHECK(uint128{1, 0} >= uint128{0});
    CHECK(uint128{1ULL << 63, 0} >= uint128{0});
    CHECK(uint128{1, 1} >= uint128{0, 0});
    CHECK(uint128{1, 1} >= uint128{0, 2});

    CHECK_FALSE(uint128{0} >= uint128{1});
    CHECK_FALSE(uint128{0} >= uint128{1, 0});
    CHECK_FALSE(uint128{1, 0} >= uint128{2, 0});
    CHECK_FALSE(uint128{1, 0} >= uint128{2, 1});
    CHECK_FALSE(uint128{1, 1} >= uint128{2, 0});
}

TEST_CASE("uint128::operator<") {
    CHECK(uint128{0} < uint128{1});
    CHECK(uint128{0} < uint128{1, 1});
    CHECK(uint128{1} < uint128{1, 1});
    CHECK(uint128{1, 1} < uint128{1, 2});

    CHECK_FALSE(uint128{2, 1} < uint128{1, 2});
    CHECK_FALSE(uint128{0} < uint128{0});
    CHECK_FALSE(uint128{1} < uint128{0});
    CHECK_FALSE(uint128{1, 0} < uint128{0});
    CHECK_FALSE(uint128{1, 1} < uint128{1, 0});
}

TEST_CASE("uint128::operator<=") {
    CHECK(uint128{0} <= uint128{0});
    CHECK(uint128{0} <= uint128{1});
    CHECK(uint128{0} <= uint128{1, 1});
    CHECK(uint128{1} <= uint128{1, 1});
    CHECK(uint128{1, 1} <= uint128{1, 2});

    CHECK_FALSE(uint128{2, 1} <= uint128{1, 2});
    CHECK_FALSE(uint128{1} <= uint128{0});
    CHECK_FALSE(uint128{1, 0} <= uint128{0});
    CHECK_FALSE(uint128{1, 1} <= uint128{1, 0});
}

TEST_CASE("uint128::operator++") {
    SUBCASE("prefix") {
        uint128 x = 1;
        CHECK((++x) == 2);
        ++x;
        CHECK(x == 3);

        uint128 y = std::uint64_t(-1);
        ++y;
        CHECK(y == uint128{1, 0});
        CHECK((++y) == uint128{1, 1});

        uint128 z{std::uint64_t(-1), 0};
        ++z;
        CHECK(z == uint128{std::uint64_t(-1), 1});

        uint128 w{std::uint64_t(-1), std::uint64_t(-1)};
        ++w;
        CHECK(w == 0);

        std::vector<uint128> is;
        for (uint128 i = 0; i < 5; ++i) {
            is.emplace_back(i);
        }
        CHECK(is == std::vector<uint128>{{0, 1, 2, 3, 4}});

        is.clear();
        for (uint128 i = 0; i <= 5; ++i) {
            is.emplace_back(i);
        }
        CHECK(is == std::vector<uint128>{{0, 1, 2, 3, 4, 5}});
    }
    SUBCASE("postfix") {
        uint128 x = 1;
        CHECK((x++) == 1);
        x++;
        CHECK(x == 3);

        uint128 y = std::uint64_t(-1);
        CHECK((y++) == uint128{0, std::uint64_t(-1)});
        y++;
        CHECK(y == uint128{1, 1});

        uint128 z{std::uint64_t(-1), 0};
        CHECK((z++) == uint128{std::uint64_t(-1), 0});
        CHECK(z == uint128{std::uint64_t(-1), 1});

        uint128 w{std::uint64_t(-1), std::uint64_t(-1)};
        CHECK((w++) == uint128{std::uint64_t(-1), std::uint64_t(-1)});
        CHECK(w == 0);
    }
}

TEST_CASE("uint128::operator--") {
    SUBCASE("prefix") {
        uint128 x = 1;
        --x;
        CHECK(x == 0);
        CHECK((--x) == uint128{std::uint64_t(-1), std::uint64_t(-1)});

        uint128 y{1, 0};
        --y;
        CHECK(y == uint128{0, std::uint64_t(-1)});
        --y;
        CHECK(y == uint128{0, std::uint64_t(-2)});

        std::vector<uint128> is;
        for (uint128 i = 5; i > 0; --i) {
            is.emplace_back(i);
        }
        CHECK(is == std::vector<uint128>{{5, 4, 3, 2, 1}});

        is.clear();
        for (uint128 i = 6; i >= 2; --i) {
            is.emplace_back(i);
        }
        CHECK(is == std::vector<uint128>{{6, 5, 4, 3, 2}});
    }
    SUBCASE("postfix") {
        uint128 x = 1;
        CHECK((x--) == 1);
        CHECK(x == 0);

        uint128 y = 0;
        CHECK((y--) == 0);
        CHECK(y == uint128{std::uint64_t(-1), std::uint64_t(-1)});
    }
}

TEST_CASE("uint128 numeric_limits") {
    numeric_limits_check<uint128>();
}

}

