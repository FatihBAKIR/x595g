//
// Created by fatih on 2/7/20.
//

#include <functional>
#include <iostream>
#include <map>
#include <variant>
#include <x595g/common.hpp>
#include <x595g/disassembler.hpp>
#include <x595g/exec.hpp>

namespace x595g {
namespace {}
template<class Type>
struct lookup {
    std::map<opcodes, std::function<void(const Type&)>> table;
};

class machine {
public:
    machine(memory mem);

    void run(stream_helper& str);

private:
    template<class T>
    lookup<T>& get_lookup() {
        return std::get<lookup<T>>(lookup_table);
    }

    void ret() {
        pop_a(registers::pc);
    }

    void aal_a(registers r) {
        // 2 byte
        aal_b(read(r));
    }

    void aal_b(word28_le_t to) {
        // 5 byte
        psh_b(read(registers::pc));
        write(registers::pc, to);
    }

    void orr_a(registers dest, registers src) {
        orr_b(dest, read(src));
    }

    void orr_b(registers dest, word28_le_t src)
    {
        auto res = read(dest) | src;

        // setting the flags:
        flags.ZF = !res.m_internal;
        flags.OF = false;
        flags.CF = false;
        flags.SF = !res.is_positive();

        write(dest, res);
    }

    void xor_a(registers dest, registers src) {
        xor_b(dest, read(src));
    }

    void xor_b(registers dest, word28_le_t src)
    {
        auto res = read(dest) ^ src;

        // setting the flags:
        flags.ZF = !res.m_internal;
        flags.OF = false;
        flags.CF = false;
        flags.SF = !res.is_positive();

        write(dest, res);
    }

    void out_a(registers r) {
        out_b(read(r));
    }

    void out_b(word28_le_t constant) {
        std::cout << char(constant.get_byte7(0));
    }

    void inp_a(registers r) {
        inp_b(read(r));
    }

    void inp_b(word28_le_t constant) {
    }

    void lod_a(registers dest, registers src) {
        lod_b(dest, read(src));
    }

    void lod_b(registers dest, word28_le_t from) {
        auto res = mem.read(from.m_internal);

        // setting the flags:
        flags.ZF = !res.m_internal;
        flags.OF = false;
        flags.CF = false;
        flags.SF = !res.is_positive();

        write(dest, res);
    }

    void add_a(registers dest, registers src)
    {
        add_b(dest, read(src));
    }

    void add_b(registers dest, word28_le_t src)
    {
        auto res = read(dest) + src;

        // set the flags:
        flags.ZF = !res.m_internal;
        flags.OF = res.overflown;
        flags.CF = res.signed_overflown;
        flags.SF = !res.is_positive();

        write(dest, res);
    }

    void sub_a(registers dest, registers src)
    {
        sub_b(dest, read(src));
    }

    void sub_b(registers dest, word28_le_t src)
    {
//        auto neg_src = src * -1;
        auto res = read(dest) - src;

        // set the flags:
        flags.ZF = !res.m_internal;
        flags.OF = res.overflown;
        flags.CF = res.signed_overflown;
        flags.SF = !res.is_positive();

        write(dest, res);
    }

    void stb_a(registers dest, registers src) {
        stb_b(dest, read(src));
    }

    void stb_b(registers dest, word28_le_t to) {
        mem.write_byte(to.m_internal, read(dest).get_byte7(0));
    }

    void str_a(registers dest, registers src) {
        str_b(dest, read(src));
    }

    void str_b(registers dest, word28_le_t to) {
        mem.write(to.m_internal, read(dest));
    }

    void psh_a(registers r) {
        psh_b(read(r));
    }

    void psh_b(word28_le_t val) {
        write(registers::sp, read(registers::sp) - 4);
        mem.write(read(registers::sp).m_internal, val);
    }

    void pop_a(registers r) {
        write(r, mem.read((read(registers::sp)).m_internal));
        write(registers::sp, read(registers::sp) + 4);
    }

    void add_handler(opcodes op, void (machine::*handler)(registers)) {
        auto fwd = [this](std::function<void(registers)> f) {
          return [f = std::move(f), this](const type2a_instr& i) { f(i.val); };
        };

        using namespace std::placeholders;
        get_lookup<type2a_instr>().table.emplace(
            op, fwd(std::bind(handler, this, _1)));
    }

    void add_handler(opcodes op, void (machine::*handler)(word28_le_t)) {
        auto fwd = [this](std::function<void(word28_le_t)> f) {
          return [f = std::move(f), this](const type2b_instr& i) { f(i.literal); };
        };

        using namespace std::placeholders;
        get_lookup<type2b_instr>().table.emplace(
            op, fwd(std::bind(handler, this, _1)));
    }

    void add_handler(opcodes op, void (machine::*handler)(registers, registers)) {
        auto fwd = [this](std::function<void(registers, registers)> f) {
          return [f = std::move(f), this](const type1a_instr& i) { f(i.dst, i.src); };
        };

        using namespace std::placeholders;
        get_lookup<type1a_instr>().table.emplace(
            op, fwd(std::bind(handler, this, _1, _2)));
    }

    void add_handler(opcodes op, void (machine::*handler)(registers, word28_le_t)) {
        auto fwd = [this](std::function<void(registers, word28_le_t)> f) {
          return [f = std::move(f), this](const type1b_instr& i) { f(i.dst, i.literal); };
        };

        using namespace std::placeholders;
        get_lookup<type1b_instr>().table.emplace(
            op, fwd(std::bind(handler, this, _1, _2)));
    }

    void add_handler(opcodes op, void (machine::*handler)()) {
        auto fwd = [this](std::function<void()> f) {
          return [f = std::move(f), this](const type3_instr& i) { f(); };
        };

        using namespace std::placeholders;
        get_lookup<type3_instr>().table.emplace(
            op, fwd(std::bind(handler, this)));
    }

    std::tuple<lookup<type1a_instr>,
               lookup<type1b_instr>,
               lookup<type2a_instr>,
               lookup<type2b_instr>,
               lookup<type3_instr>>
        lookup_table{};

    void write(registers r, word28_le_t val) {
        if (r == registers::pc) {
            m_ins_str->seek(val.m_internal);
        }
        m_regs[int(r)] = val;
    }

    word28_le_t read(registers r) {
        if (r == registers::pc) {
            return m_regs[int(r)] = word28_le_t{uint32_t(m_ins_str->tell()), false};
        }
        return m_regs[int(r)];
    }

    stream_helper* m_ins_str;
    std::array<word28_le_t, 11> m_regs{};

    struct {
        bool CF;
        bool ZF;
        bool OF;
        bool SF;
    } flags{};

public:
    memory mem;
};

machine::machine(memory m) : mem(std::move(m)) {
    add_handler(opcodes::out, &machine::out_a);
    add_handler(opcodes::out, &machine::out_b);
    add_handler(opcodes::inp, &machine::inp_a);
    add_handler(opcodes::inp, &machine::inp_b);
    add_handler(opcodes::xor_, &machine::xor_a);
    add_handler(opcodes::xor_, &machine::xor_b);
    add_handler(opcodes::orr, &machine::orr_a);
    add_handler(opcodes::orr, &machine::orr_b);
    add_handler(opcodes::aal, &machine::aal_a);
    add_handler(opcodes::aal, &machine::aal_b);
    add_handler(opcodes::stb, &machine::stb_a);
    add_handler(opcodes::stb, &machine::stb_b);
    add_handler(opcodes::str, &machine::str_a);
    add_handler(opcodes::str, &machine::str_b);
    add_handler(opcodes::lod, &machine::lod_a);
    add_handler(opcodes::lod, &machine::lod_b);
    add_handler(opcodes::psh, &machine::psh_a);
    add_handler(opcodes::psh, &machine::psh_b);
    add_handler(opcodes::pop, &machine::pop_a);
    add_handler(opcodes::ret, &machine::ret);

    write(registers::sp, word28_le_t{0x0FFFFFFF, false});
}

void machine::run(stream_helper &str) {
    m_ins_str = &str;
    while (!str.finished()) {
        auto instr = parse_one(str);
        std::visit(
            [this](auto i) {
                std::cerr << "Running " << read(registers::pc) << '\t' << read(registers::sp) << '\t' << i << '\n';
                get_lookup<decltype(i)>().table[i.opcode](i);
            },
            instr);
    }
}

void interpreter::run(word28_le_t entry) {
    auto buf = m_machine->mem.str();
    stream_helper str(buf);
    str.seek(entry.m_internal);
    m_machine->run(str);
}

interpreter::interpreter(x595g::memory mem)
    : m_machine(std::make_unique<machine>(std::move(mem))) {
}

interpreter::~interpreter() = default;
} // namespace x595g