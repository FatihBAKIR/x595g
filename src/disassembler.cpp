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
        case instr_type::type1a: {
            m_cur = parse_type1a();
            return;
        }
        case instr_type::type1b: {
            m_cur = parse_type1b();
            return;
        }
        case instr_type::type2a: {
            m_cur = parse_type2a();
            return;
        }
        case instr_type::type2b: {
            m_cur = parse_type2b();
            return;
        }
        case instr_type::type3: {
            m_cur = parse_type3();
            return;
        }
        default:
            throw std::runtime_error(fmt::format("not implemented: {}", type));
        }
    }

    const instr* get() {
        return std::visit([](auto& x) { return static_cast<instr*>(&x); }, m_cur);
    }

    // Disassembling Type1 instructions
    type1a_instr parse_type1a() {
        auto instr = m_str.read_word21_be();
        type1a_instr res;

        auto o_field = ((instr.m_internal >> 14) & 0x0F);
        auto s_field = ((instr.m_internal >> 10) & 0x0F);
        auto d_field = ((instr.m_internal >> 6) & 0x0F);

        res.opcode = opcodes(o_field + int(opcodes::type1_begin));
        res.src = registers(s_field);
        res.dst = registers(d_field);

        return res;
    }

    type1b_instr parse_type1b() {
        auto instr = m_str.read_word42_be();
        type1b_instr res;

        auto o_field = ((instr.m_internal >> 35) & 0x0F);
        auto d_field = ((instr.m_internal >> 31) & 0x0F);
        auto s_field = (instr.m_internal & 0x0FFFFFFF);

        res.opcode = opcodes(o_field + int(opcodes::type1_begin));
        res.literal = s_field;
        res.dst = registers(d_field);

        return res;
    }

    // Disassembling Type2 instructions
    type2a_instr parse_type2a() {
        auto instr = m_str.read_short_be();
        type2a_instr res;

        auto o_field = ((instr.m_internal >> 7) & 0x0F);
        auto v_field = ((instr.m_internal >> 3) & 0x0F);

        res.opcode = opcodes(o_field + int(opcodes::type2_begin));
        res.val = registers(v_field);

        return res;
    }

    type2b_instr parse_type2b() {
        auto instr = m_str.read_word35_be();
        type2b_instr res;

        auto o_field = ((instr.m_internal >> 28) & 0x0F);
        auto v_field = (instr.m_internal & 0x0FFFFFFF);

        res.opcode = opcodes(o_field + int(opcodes::type2_begin));
        res.literal = v_field;

        return res;
    }

    // Disassembling Type3 instructions
    type3_instr parse_type3() {
        auto instr = m_str.read_basic<uint8_t>();
        type3_instr res;

        auto o_field = ((instr >> 4) & 0x1);

        res.opcode = opcodes(o_field + int(opcodes::type3_begin));

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

disassembler_iterator& disassembler_iterator::operator++() {
    m_disas->advance();
    return *this;
}

const instr& disassembler_iterator::operator*() const {
    return *m_disas->get();
}

bool operator==(const disassembler_iterator& iter,
                const disassembler_iterator_sentinel&) {
    return iter.m_disas->done();
}

bool operator==(const disassembler_iterator_sentinel&,
                const disassembler_iterator& iter) {
    return iter.m_disas->done();
}
} // namespace x595g