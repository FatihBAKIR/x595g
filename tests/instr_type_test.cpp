#include <doctest.h>
#include <x595g/disassembler.hpp>

namespace x595g {
namespace {
TEST_CASE("instruction classification works") {
    uint8_t first_by7e = 0b0010'0000; // add instruction
    REQUIRE_EQ(instr_type::type1a, classify_instruction(first_by7e));
}
} // namespace
} // namespace x595g