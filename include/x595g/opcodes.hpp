#pragma once

#include <cstdint>
#include <ostream>

namespace x595g {
enum class opcodes : uint8_t
{
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

    ret,
    nop,

    opcode_count,

    type1_begin = add,
    type2_begin = jmp,
    type3_begin = ret,
};

std::ostream& operator<<(std::ostream& os, opcodes opcode);
} // namespace x595g