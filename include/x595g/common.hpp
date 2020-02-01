#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <ostream>
#include <stdexcept>
#include <x595g/opcodes.hpp>

namespace x595g {
enum class instr_type
{
    type1a,
    type1b,
    type2a,
    type2b,
    type3
};

enum class registers : uint8_t
{
    ra,
    rb,
    rc,
    rd,
    rr,
    rs,
    rx,
    ry,
    rz,
    pc,
    sp
};

inline std::ostream& operator<<(std::ostream& os, registers reg) {
    switch (reg) {
    case registers::ra:
        return os << "ra";
    case registers::rb:
        return os << "rb";
    case registers::rc:
        return os << "rc";
    case registers::rd:
        return os << "rd";
    case registers::rr:
        return os << "rr";
    case registers::rs:
        return os << "rs";
    case registers::rx:
        return os << "rx";
    case registers::ry:
        return os << "ry";
    case registers::rz:
        return os << "rz";
    case registers::pc:
        return os << "pc";
    case registers::sp:
        return os << "sp";
    }
    throw std::runtime_error("Unknown register");
}

struct instr {
    opcodes opcode;

    friend std::ostream& operator<<(std::ostream& os, const instr& ins) {
        return os << ins.dump();
    }

private:
    virtual std::string dump() const = 0;
};


struct type1_instr : instr {
    registers dst;
    virtual ~type1_instr() = 0;
};
inline type1_instr::~type1_instr() = default;

struct type1a_instr : type1_instr {
    registers src;

private:
    std::string dump() const override {
        return fmt::format("{} %{} %{}", opcode, dst, src);
    }
};

struct type1b_instr : type1_instr {
    uint32_t literal;

private:
    std::string dump() const override {
        return fmt::format("{} %{} ${:#X}", opcode, dst, literal);
    }
};

struct type2_instr : instr {
    virtual ~type2_instr() = 0;
};

inline type2_instr::~type2_instr() = default;

struct type2a_instr : type2_instr {
    registers val;

private:
    std::string dump() const override {
        return fmt::format("{} %{}", opcode, val);
    }
};

struct type2b_instr : type2_instr {
    uint32_t literal;

private:
    std::string dump() const override {
        return fmt::format("{} ${:#X}", opcode, literal);
    }
};

struct type3_instr : instr {
private:
    std::string dump() const override {
        return fmt::format("{}", opcode);
    }
};
} // namespace x595g
