#include <array>
#include <string>
#include <x595g/opcodes.hpp>

namespace x595g {
namespace {
std::array<std::string, int(opcodes::opcode_count)> names{
    "add", "sub", "mul", "div", "cmp", "tst", "and", "orr", "xor", "str", "stb", "lod",

    "jmp", "jlt", "jeq", "cal", "psh", "pop", "not", "out", "inp", "amp", "alt", "aeq",
    "aal",

    "ret", "nop",
};
}
std::ostream& operator<<(std::ostream& os, opcodes opcode) {
    return os << names[int(opcode)];
}
} // namespace x595g