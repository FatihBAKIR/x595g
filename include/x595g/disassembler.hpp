#pragma once

#include <istream>
#include <memory>
#include <x595g/common.hpp>

namespace x595g {

instr_type classify_instruction(uint8_t first_word);

struct disassembler {
public:
    disassembler(std::istream& str);

    void advance();
    const instr* get();
    bool done();

    ~disassembler();

private:
    struct disassembler_impl;
    std::unique_ptr<disassembler_impl> m_impl;
};
} // namespace x595g