#pragma once

#include <tos/span.hpp>
#include <x595g/by7e.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

namespace x595g {
class memory {
public:
    memory(const memory&) = delete;
    memory(memory&&) = default;

    memory()
        : m_flat(1 << 28, 0xFF) {
    }

    word28_le_t read(int addr) {
        return word28_le_t(whole().slice(addr, 4));
    }

    void write(int addr, word28_le_t data) {
        auto word_slice = whole().slice(addr, 4);
        for (int i = 0; i < 4; ++i) {
            word_slice[i] = data.get_byte7(i);
        }
    }

    void write_byte(int addr, uint8_t data) {
        auto word_slice = whole().slice(addr, 1);
        word_slice[0] = data;
    }

    void copy_bulk(int addr, tos::span<const uint8_t> from) {
        auto span = whole().slice(addr, from.size());
        std::copy(from.begin(), from.end(), span.begin());
    }

    boost::interprocess::bufferstream str() {
        return {reinterpret_cast<char*>(whole().begin()),
                whole().size()};
    }

private:
    tos::span<uint8_t> whole() {
        return m_flat;
    }

    std::vector<uint8_t> m_flat;
};
} // namespace x595g