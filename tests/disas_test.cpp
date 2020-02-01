#include "x595g/common.hpp"
#include "x595g/opcodes.hpp"

#include <array>
#include <doctest.h>
#include <sstream>
#include <x595g/disassembler.hpp>

namespace x595g {
namespace {
TEST_CASE("disassembling a type1a instruction works") {
    std::array<uint8_t, 3> buf{0b0010'0000, 0b0000'0000, 0b0000'0000};
    std::stringstream ss;

    ss.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    ss.seekg(0);

    disassembler dis(ss);
    auto res = dis.get();

    REQUIRE(dynamic_cast<const type1a_instr*>(res));
};

TEST_CASE("disassembling a type1b instruction works") {
    std::array<uint8_t, 6> buf{
        0b0011'0000, 0b0000'0000, 0b0000'0000, 0b0000'0000, 0b0000'0000, 0x04};
    std::stringstream ss;

    ss.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    ss.seekg(0);

    disassembler dis(ss);
    auto res = dis.get();

    REQUIRE(dynamic_cast<const type1b_instr*>(res));

    auto instruction = dynamic_cast<const type1b_instr*>(res);
    REQUIRE_EQ(instruction->literal, 4);
};

TEST_CASE("disassembling a type2a instruction works") {
    std::array<uint8_t, 2> buf{0b0100'0000, 0b0000'0000};
    std::stringstream ss;

    ss.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    ss.seekg(0);

    disassembler dis(ss);
    auto res = dis.get();

    REQUIRE(dynamic_cast<const type2a_instr*>(res));
};

TEST_CASE("disassembling a type2b instruction works") {
    std::array<uint8_t, 5> buf{
        0b0101'0000,
        0b0000'0000,
        0b0000'0000,
        0b0000'0000,
        0b0000'0000,
    };
    std::stringstream ss;

    ss.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    ss.seekg(0);

    disassembler dis(ss);
    auto res = dis.get();

    REQUIRE(dynamic_cast<const type2b_instr*>(res));
};

TEST_CASE("disassembling a type3 instruction works") {
    std::array<uint8_t, 1> buf{0b0110'0000};
    std::stringstream ss;

    ss.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    ss.seekg(0);

    disassembler dis(ss);
    auto res = dis.get();

    REQUIRE(dynamic_cast<const type3_instr*>(res));
};


} // namespace
} // namespace x595g