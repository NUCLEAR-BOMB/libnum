#include <doctest/doctest.h>

#include <libnum/uint128.hpp>

namespace {

using namespace libnum;

TEST_CASE("uint128::operator+") {
    uint128 x = 123;
    uint128 y = 456;

    CHECK(x + y == 579);
    CHECK(y + x == 579);

    CHECK(x + 10 == 133);
    CHECK(5 + x  == 128);
}

TEST_CASE("uint128::operator-") {
    uint128 x = 100;
    uint128 y = 55;

    CHECK(x - y == 45);
    CHECK(y - x == uint128{uint64_t(-45), uint64_t(-1)});

    CHECK(x - 5 == 95);
    CHECK(x - 100 == 0);
    CHECK(100 - x == 0);
    CHECK(105 - x == 5);
    CHECK(123 - x == 23);
}

}
