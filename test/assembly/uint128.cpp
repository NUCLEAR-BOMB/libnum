#include <libnum/uint128.hpp>
#include <cstring>

using namespace libnum;

uint128 value1{0};
uint128 value2{0};
uint128 value3{0};

inline void copy(uint128& out, const uint128& in) noexcept {
    *(uint64_t*)&out = in.low();
    *((uint64_t*)&out + 1) = in.high();
}

extern "C" {
    void uint128_add() {
        copy(value3, value1 + value2);
    }
    void uint128_sub() {
        copy(value3, value1 - value2);
    }
    void uint128_mul() {
        copy(value3, value1 * value2);
    }
}
