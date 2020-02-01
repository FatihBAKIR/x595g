#pragma once

#include <cstdint>
#include <ostream>

namespace x595g {
enum class opcodes : uint8_t
{
    // type1 instructions
    add,
    sub,
    mul,
    div,
    cmp,
    tst,
    and_,
    orr,
    xor_,
    str,
    stb,
    lod,

    // type2 instructions
    jmp,
    jlt,
    jeq,
    cal,
    psh,
    pop,
    not_,
    out,
    inp,
    amp,
    alt,
    aeq,
    aal,

    // type3 instructions
    ret,
    nop,

    opcode_count,

    type1_begin = add,
    type2_begin = jmp,
    type3_begin = ret,
};

std::ostream& operator<<(std::ostream& os, opcodes opcode);
} // namespace x595g