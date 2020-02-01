#include <doctest.h>
#include <x595g/by7e.hpp>

namespace x595g {
namespace {
TEST_CASE("word28 constructor works") {
    std::array<uint8_t, 4> buf{0x1, 0x2, 0x3, 0x4};
    word28_le_t word(buf);
    REQUIRE_EQ(0b0000'0000100'0000011'0000010'0000001, word.m_internal);
};
} // namespace
} // namespace x595g