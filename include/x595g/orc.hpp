#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <istream>
#include <optional>
#include <ostream>
#include <vector>
#include <x595g/by7e.hpp>

namespace x595g::orc {
enum class file_type : uint8_t
{
    executable = 0,
    object = 1
};

struct definition {
    word28_le_t section;
    word28_le_t offset;
};

inline std::ostream& operator<<(std::ostream& os, const definition& def) {
    return os << fmt::format("{{{},{}}}", def.section, def.offset);
}

struct symbol {
    std::string name;
    std::optional<definition> def;
};

struct symbol_table {
    std::vector<symbol> syms;
};

struct relocation {
    word28_le_t section;
    word28_le_t offset;
    std::string symbol;
    word28_le_t plus;
};

struct relocation_table {
    std::vector<relocation> relocs;
};

enum class permissions : uint8_t
{
    none        = 0,
    read        = 0b0100'0000,
    write       = 0b0010'0000,
    execute     = 0b0001'0000
};

inline permissions operator&(permissions p, permissions bit) {
    return permissions(uint8_t(p) & uint8_t(bit));
}

inline std::ostream& operator<<(std::ostream& os, permissions perm) {
    std::string s;
    if ((perm & permissions::read) != permissions::none) {
        s += "r";
    }
    if ((perm & permissions::write) != permissions::none) {
        s += "w";
    }
    if ((perm & permissions::execute) != permissions::none) {
        s += "x";
    }
    if (s.empty()) {
        return os << "noperm";
    }
    return os << s;
}

struct section {
    permissions perm;
    word28_le_t offset;
    std::string name;
    word28_le_t size;
};

struct section_table {
    std::vector<section> sections;
};

enum class segment_type : uint8_t
{
    progbits = 0b10,
    none = 0b01
};

struct segment {
    std::string name;
    word28_le_t offset;
    word28_le_t base;
    word28_le_t size;
    permissions perm;
    segment_type type;
};

struct segment_table {
    std::vector<segment> segs;
};

struct orc_file {
    file_type type;
    std::optional<word28_le_t> entry_point;
    symbol_table symbols;
    relocation_table relocations;
    section_table sections;
    segment_table segments;
    std::vector<uint8_t> contents;

    tos::span<const uint8_t> section_body(const section& sect) const {
        tos::span<const uint8_t> all(contents);
        return all.slice(sect.offset.m_internal, sect.size.m_internal);
    }

    tos::span<const uint8_t> segment_body(const segment& seg) const {
        tos::span<const uint8_t> all(contents);
        return all.slice(seg.offset.m_internal, seg.size.m_internal);
    }
};

orc_file parse_orc(std::istream& stream);
} // namespace x595g::orc