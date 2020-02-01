#include "x595g/common.hpp"
#include "x595g/opcodes.hpp"
#include "x595g/stream_utils.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <variant>
#include <x595g/disassembler.hpp>

namespace x595g {
namespace {
struct disas {
    explicit disas(std::istream& str)
        : m_str{str} {
        advance();
    }

    void advance() {
        std::cerr << m_str.tell() << '\n';
        auto first = m_str.peek_byte();
        auto type = classify_instruction(first);
        switch (type) {
        case instr_type::type2a: {
            m_cur = parse_type2a();
            return;
        }
        default:
            throw std::runtime_error(fmt::format("not implemented: {}", type));
        }
    }

    const instr* get() {
        return std::visit([](auto& x) { return static_cast<instr*>(&x); }, m_cur);
    }

    type2a_instr parse_type2a() {
        auto instr = m_str.read_short_be();
        type2a_instr res;
        auto ofield = ((instr.m_internal >> 7) & 0x0F);
        res.opcode =
            opcodes(ofield + int(opcodes::type2_begin));
        res.val = registers((instr.m_internal >> 3) & 0x0F);
        return res;
    }

    std::variant<type1a_instr, type1b_instr, type2a_instr, type2b_instr, type3_instr>
        m_cur;
    stream_helper m_str;
};
} // namespace

struct disassembler::disassembler_impl : disas {
    using disas::disas;
};

disassembler::disassembler(std::istream& str)
    : m_impl(std::make_unique<disassembler::disassembler_impl>(str)) {
}

void disassembler::advance() {
    m_impl->advance();
}

const instr* disassembler::get() {
    return m_impl->get();
}

bool disassembler::done() {
    return m_impl->m_str.finished();
}

disassembler::~disassembler() = default;
} // namespace x595g