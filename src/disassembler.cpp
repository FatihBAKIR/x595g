#include "x595g/common.hpp"
#include "x595g/opcodes.hpp"
#include "x595g/stream_utils.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <istream>
#include <stdexcept>
#include <variant>
#include <x595g/disassembler.hpp>

namespace x595g {

// Disassembling Type1 instructions
type1a_instr parse_type1a(stream_helper& str) {
    auto instr = str.read_word21_be();
    type1a_instr res;

    auto o_field = ((instr.m_internal >> 14) & 0x0F);
    auto s_field = ((instr.m_internal >> 10) & 0x0F);
    auto d_field = ((instr.m_internal >> 6) & 0x0F);

    res.opcode = opcodes(o_field + int(opcodes::type1_begin));
    res.src = registers(s_field);
    res.dst = registers(d_field);

    return res;
}

type1b_instr parse_type1b(stream_helper& str) {
    auto instr = str.read_word42_be();
    type1b_instr res;

    auto o_field = ((instr.m_internal >> 35) & 0x0F);
    auto d_field = ((instr.m_internal >> 31) & 0x0F);
    uint32_t s_field = (instr.m_internal & 0x0FFFFFFF);

    res.opcode = opcodes(o_field + int(opcodes::type1_begin));
    res.literal = word28_le_t{s_field};
    res.dst = registers(d_field);

    return res;
}

// Disassembling Type2 instructions
type2a_instr parse_type2a(stream_helper& str) {
    auto instr = str.read_short_be();
    type2a_instr res;

    auto o_field = ((instr.m_internal >> 7) & 0x0F);
    auto v_field = ((instr.m_internal >> 3) & 0x0F);

    res.opcode = opcodes(o_field + int(opcodes::type2_begin));
    res.val = registers(v_field);

    return res;
}

type2b_instr parse_type2b(stream_helper& str) {
    auto instr = str.read_word35_be();
    type2b_instr res;

    auto o_field = ((instr.m_internal >> 28) & 0x0F);
    uint32_t v_field = (instr.m_internal & 0x0FFFFFFF);

    res.opcode = opcodes(o_field + int(opcodes::type2_begin));
    res.literal = word28_le_t{v_field};

    return res;
}

// Disassembling Type3 instructions
type3_instr parse_type3(stream_helper& str) {
    auto instr = str.read_basic<uint8_t>();
    type3_instr res;

    auto o_field = ((instr >> 4) & 0x1);

    res.opcode = opcodes(o_field + int(opcodes::type3_begin));

    return res;
}

instruction parse_one(stream_helper& str) {
    auto first = str.peek_byte();
    auto type = classify_instruction(first);
    switch (type) {
    case instr_type::type1a: {
        return parse_type1a(str);
    }
    case instr_type::type1b: {
        return parse_type1b(str);
    }
    case instr_type::type2a: {
        return parse_type2a(str);
    }
    case instr_type::type2b: {
        return parse_type2b(str);
    }
    case instr_type::type3: {
        return parse_type3(str);
    }
    default:
        throw std::runtime_error(fmt::format("not implemented: {}", type));
    }
}

namespace {
struct disas {
    explicit disas(std::istream& str)
        : m_str{str} {
        advance();
    }

    void advance() {
        m_cur = parse_one(m_str);
    }

    const instr* get() {
        return std::visit([](auto& x) { return static_cast<instr*>(&x); }, m_cur);
    }

    instruction m_cur;
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