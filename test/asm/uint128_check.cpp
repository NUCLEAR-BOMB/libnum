#include <libnum/uint128.hpp>
#include <cstddef>

using namespace libnum;

extern "C" {

uint128 shift_left(uint128 x, uint8_t y) {
    return x << y;
}
// CHECK: shift_left
// CHECK: mov {{.+}}, qword ptr [{{.+}}]
// CHECK: mov {{.+}}, qword ptr [{{.+}}]
// CHECK: shld {{.+}}, {{.+}}, cl
// CHECK: shl {{.+}}, cl
// CHECK: test {{.+}}, 64
// CHECK: cmove [[LOW_OUT:.+]], {{.+}}
// CHECK: cmovne [[HIGH_OUT:.+]], {{.+}}
// CHECK: mov qword ptr [{{.+}}], [[LOW_OUT]]
// CHECK: mov qword ptr [{{.+}}], [[HIGH_OUT]]



}
