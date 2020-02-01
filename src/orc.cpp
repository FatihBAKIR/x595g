#include "x595g/by7e.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <tos/span.hpp>
#include <type_traits>
#include <x595g/orc.hpp>
#include <x595g/stream_utils.hpp>

namespace x595g::orc {
namespace {
using namespace tos;
class orc_parser {
public:
    orc_parser(std::istream& str)
        : m_str{str} {
    }

    orc_file parse() {
        orc_file file;
        if (!check_header()) {
            throw std::runtime_error("not an orc file!");
        }
        file.type = read_file_type();
        auto has_entry = read_has_entry_point();
        if (has_entry) {
            file.entry_point = read_entry_point();
        }
        file.symbols = read_symbol_table();
        file.relocations = read_relocation_table();
        file.sections = read_section_table();
        file.segments = read_segment_table();
        file.contents = m_str.read_to_end();
        return file;
    }

    bool check_header() {
        auto header = m_str.read_text();
        return header == "orc";
    }

    file_type read_file_type() {
        return m_str.read_basic<file_type>();
    }

    bool read_has_entry_point() {
        return m_str.read_basic<bool>();
    }

    word28_le_t read_entry_point() {
        return m_str.read_word();
    }

    symbol_table read_symbol_table() {
        auto len = m_str.read_word();
        symbol_table table;
        table.syms.resize(len.m_internal);
        std::generate(
            table.syms.begin(), table.syms.end(), [&] { return read_symbol(); });
        return table;
    }

    symbol read_symbol() {
        symbol res;
        res.name = m_str.read_text();
        auto defined = m_str.read_bool();
        if (!defined) {
            return res;
        }
        res.def = read_def();
        return res;
    }

    definition read_def() {
        definition def;
        def.section = m_str.read_word();
        def.offset = m_str.read_word();
        return def;
    }

    relocation_table read_relocation_table() {
        auto len = m_str.read_word();
        relocation_table table;
        table.relocs.resize(len.m_internal);
        std::generate(
            table.relocs.begin(), table.relocs.end(), [&] { return read_relocation(); });
        return table;
    }

    relocation read_relocation() {
        relocation reloc;
        reloc.offset = m_str.read_word();
        reloc.section = m_str.read_word();
        reloc.symbol = m_str.read_text();
        reloc.plus = m_str.read_word();
        return reloc;
    }

    section_table read_section_table() {
        auto len = m_str.read_word();
        section_table table;
        table.sections.resize(len.m_internal);
        std::generate(
            table.sections.begin(), table.sections.end(), [&] { return read_section(); });
        return table;
    }

    section read_section() {
        section sec;
        sec.perm = read_permission();
        sec.offset = m_str.read_word();
        sec.name = m_str.read_text();
        sec.size = m_str.read_word();
        return sec;
    }

    permissions read_permission() {
        return m_str.read_basic<permissions>();
    }

    segment_table read_segment_table() {
        auto len = m_str.read_word();
        segment_table table;
        table.segs.resize(len.m_internal);
        std::generate(
            table.segs.begin(), table.segs.end(), [&] { return read_segment(); });
        return table;
    }

    segment read_segment() {
        segment seg;
        seg.name = m_str.read_text();
        seg.offset = m_str.read_word();
        seg.base = m_str.read_word();
        seg.perm = read_permission();
        seg.type = read_segment_type();
        return seg;
    }

    segment_type read_segment_type() {
        return m_str.read_basic<segment_type>();
    }

private:
    stream_helper m_str;
};
} // namespace

orc_file parse_orc(std::istream& stream) {
    orc_parser parser(stream);
    return parser.parse();
}
} // namespace x595g::orc