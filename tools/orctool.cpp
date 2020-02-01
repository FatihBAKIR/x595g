#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>
#include <x595g/disassembler.hpp>
#include <x595g/orc.hpp>

std::string hex_string(tos::span<const uint8_t> buf) {
    std::string s;
    s.reserve(buf.size() * 2);
    for (auto byte : buf) {
        s += fmt::format("{:02X}", byte);
    }
    return s;
}

namespace x595g {
void run(const std::vector<std::string_view>& args) {
    orc::orc_file file;
    if (args[1] == "-i") {
        file = orc::parse_orc(std::cin);
    } else if (args[1] == "-f") {
        std::ifstream f{std::string(args[2])};
        file = orc::parse_orc(f);
    }

    std::cout << int(file.type) << '\n';
    std::cout << file.symbols.syms.size() << '\n';

    std::cout << file.contents.size() << '\n';

    fmt::print("Relocations {}\n", file.relocations.relocs.size());
    for (auto& reloc : file.relocations.relocs) {
        fmt::print(
            "{}\t{}\t{}\t{}\n", reloc.symbol, reloc.section, reloc.offset, reloc.plus);
    }

    fmt::print("Sections {}\n", file.sections.sections.size());

    for (auto& section : file.sections.sections) {
        fmt::print(
            "{}\t{}\t{}\t{}\n", section.name, section.perm, section.offset, section.size);
        if ((section.perm & orc::permissions::execute) != orc::permissions::none) {
            auto body = file.section_body(section);
            std::stringstream sstr(std::string(body.begin(), body.end()));

            try {
                for (disassembler d(sstr); !d.done(); d.advance()) {
                    fmt::print("{}\n", *d.get());
                }
            } catch (std::exception& err) {
                std::cout << fmt::format("Dissassembly failed: {}\n", err.what());
            }
        }
    }

    fmt::print("Segments {}\n", file.segments.segs.size());
    for (auto& segment : file.segments.segs) {
        fmt::print("{}\t{}\n", segment.name, segment.perm);
    }

    for (auto& sym : file.symbols.syms) {
        fmt::print("{}", sym.name);
        if (sym.def) {
            fmt::print("\t{}\n", *sym.def);
            auto& sect = file.sections.sections.at(sym.def->section.m_internal);
            auto body = file.section_body(sect);
            auto symbol_slice = body.slice(sym.def->offset.m_internal);

            std::cout << hex_string(symbol_slice) << std::endl;
            std::stringstream sstr(std::string(symbol_slice.begin(), symbol_slice.end()));

            for (disassembler d(sstr); !d.done(); d.advance()) {
                fmt::print("{}\n", *d.get());
            }
        }
        fmt::print("\n");
    }
}
} // namespace x595g
int main(int argc, char** argv) {
    std::vector<std::string_view> args(argv, argv + argc);
    x595g::run(args);
}