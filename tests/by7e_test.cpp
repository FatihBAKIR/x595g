#include <doctest.h>
#include <x595g/by7e.hpp>

namespace x595g {
namespace {
TEST_CASE("word28 constructor works") {
    std::array<uint8_t, 4> buf{0x1, 0x2, 0x3, 0x4};
    word28_le_t word(buf);
    REQUIRE_EQ(0b0000'0000100'0000011'0000010'0000001, word.m_internal);
};

TEST_CASE("Signed overflow detection words")
{
    std::array<uint8_t, 2> buf1{0x7F, 0x7F};
    std::array<uint8_t, 2> buf2{0x1, 0x0};
    word14_le_t num1(buf1);
    word14_le_t num2(buf2);

    auto sum = num1 + num2;
    REQUIRE(sum.overflown);
    REQUIRE(sum.signed_overflown);
}

TEST_CASE("Signed overflow detection words")
{
    std::array<uint8_t, 2> buf1{0x7F, 0x3F};
    std::array<uint8_t, 2> buf2{0x1, 0x0};
    word14_le_t num1(buf1);
    word14_le_t num2(buf2);

    auto sum = num1 + num2;
    REQUIRE_FALSE(sum.overflown);
    REQUIRE(sum.signed_overflown);
}
} // namespace
} // namespace x595g