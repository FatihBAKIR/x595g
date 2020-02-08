#pragma once

#include <istream>
#include <memory>
#include <x595g/common.hpp>
#include <iterator>
#include <variant>
#include <x595g/stream_utils.hpp>

namespace x595g {
using instruction =
std::variant<type1a_instr, type1b_instr, type2a_instr, type2b_instr, type3_instr>;
instruction parse_one(stream_helper& str);

instr_type classify_instruction(uint8_t first_word);

struct disassembler;

struct disassembler_iterator_sentinel {};

struct disassembler_iterator : std::iterator<std::forward_iterator_tag, const instr*> {
public:
    disassembler_iterator(disassembler& disas) : m_disas{&disas} {}

    disassembler_iterator& operator++();
    const instr& operator*() const;

    friend bool operator==(const disassembler_iterator& iter, const disassembler_iterator_sentinel&);
    friend bool operator==(const disassembler_iterator_sentinel&, const disassembler_iterator& iter);

private:
    disassembler* m_disas;
};

struct disassembler {
public:
    disassembler(std::istream& str);

    void advance();
    const instr* get();
    bool done();

    disassembler_iterator begin() { return {*this}; }
    disassembler_iterator_sentinel end() { return {}; }

    ~disassembler();

private:
    struct disassembler_impl;
    std::unique_ptr<disassembler_impl> m_impl;
};
} // namespace x595g