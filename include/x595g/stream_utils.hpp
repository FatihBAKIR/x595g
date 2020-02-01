#pragma once

#include <array>
#include <istream>
#include <iterator>
#include <tos/span.hpp>
#include <type_traits>
#include <vector>
#include <x595g/by7e.hpp>

namespace x595g {
class stream_helper {
public:
    stream_helper(std::istream& str)
        : m_str{&str} {
    }

    bool finished() const {
        return m_str->eof();
    }

    std::vector<uint8_t> read_to_end() {
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(*m_str)),
                                    std::istreambuf_iterator<char>());
    }

    uint8_t peek_byte() {
        return m_str->peek();
    }

    template<class T>
    T read_basic() {
        using namespace tos;
        static_assert(std::is_trivially_copyable_v<T>);
        T type;
        read(raw_cast<uint8_t>(monospan(type)));
        return type;
    }

    bool read_bool() {
        return read_basic<bool>();
    }

    std::string read_text() {
        using namespace tos;

        std::string str;
        uint8_t buf;
        while (read(monospan(buf)), buf != 0) {
            str.push_back(buf);
        }
        return str;
    }

    word14_be_t read_short_be() {
        using namespace tos;

        std::array<uint8_t, 2> buf;
        read(buf);
        return word14_be_t{buf};
    }

    word21_be_t read_word21_be() {
        using namespace tos;

        std::array<uint8_t, 3> buf;
        read(buf);
        return word21_be_t{buf};
    }

    word35_be_t read_word35_be() {
        using namespace tos;

        std::array<uint8_t, 5> buf;
        read(buf);
        return word35_be_t{buf};
    }

    word42_be_t read_word42_be() {
        using namespace tos;

        std::array<uint8_t, 6> buf;
        read(buf);
        return word42_be_t{buf};
    }

    word14_le_t read_short() {
        using namespace tos;

        std::array<uint8_t, 2> buf;
        read(buf);
        return word14_le_t{buf};
    }

    word28_le_t read_word() {
        using namespace tos;

        std::array<uint8_t, 4> buf;
        read(buf);
        return word28_le_t{buf};
    }

    word56_le_t read_long() {
        using namespace tos;

        std::array<uint8_t, 8> buf;
        read(buf);
        return word56_le_t{buf};
    }

    ssize_t tell() const {
        return m_str->tellg();
    }

private:
    void read(tos::span<uint8_t> buf) {
        m_str->read(reinterpret_cast<char*>(buf.data()), buf.size());
    }

    std::istream* m_str;
};
} // namespace x595g