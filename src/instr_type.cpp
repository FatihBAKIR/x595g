#include <stdexcept>
#include <x595g/common.hpp>

namespace x595g {
instr_type classify_instruction(uint8_t first_word) {
    if ((first_word & 0x70) == 0b0010'0000) {
        return instr_type::type1a;
    }
    if ((first_word & 0x70) == 0b0011'0000) {
        return instr_type::type1b;
    }
    if ((first_word & 0x70) == 0b0100'0000) {
        return instr_type::type2a;
    }
    if ((first_word & 0x70) == 0b0101'0000) {
        return instr_type::type2b;
    }
    if ((first_word & 0b0110'1111) == 0b0110'0000) {
        return instr_type::type3;
    }
    throw std::runtime_error(fmt::format("unknown instruction type: {:#B}", first_word));
}
} // namespace x595g
