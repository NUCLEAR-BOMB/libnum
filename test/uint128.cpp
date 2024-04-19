#include <doctest/doctest.h>

#include <libnum/uint128.hpp>

namespace {

using namespace libnum;

TEST_CASE("uint128::operator+") {
	uint128 x = 5;
	CHECK(x + 10 == 15);
	CHECK(10 + x == 15);
}

}