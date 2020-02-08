#pragma once

#include <cstdint>
#include <ostream>
#include <tos/span.hpp>

namespace x595g {
template <class T>
struct word_base {
    uint64_t m_internal{};

    friend T operator|(T left, const word_base& right) {
        left |= right;
        return left;
    }

    friend T operator^(T left, const word_base& right) {
        left ^= right;
        return left;
    }

    friend T operator+(T left, int right) {
        left += right;
        return left;
    }

    friend T operator-(T left, int right) {
        left -= right;
        return left;
    }

    T& operator|=(word_base other) {
        m_internal |= other.m_internal;
        return self();
    }
    T& operator^=(word_base other) {
        m_internal ^= other.m_internal;
        return self();
    }
    T& operator+=(int diff) {
        m_internal += diff;
        return self();
    }
    T& operator-=(int diff) {
        m_internal -= diff;
        return self();
    }

    T& self() {
        return *static_cast<T*>(this);
    }
};

struct word56_le_t : word_base<word56_le_t> {
    explicit word56_le_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[0];
        m_internal |= static_cast<uint64_t>(bytes[1]) << 7;
        m_internal |= static_cast<uint64_t>(bytes[2]) << 14;
        m_internal |= static_cast<uint64_t>(bytes[3]) << 21;
        m_internal |= static_cast<uint64_t>(bytes[4]) << 28;
        m_internal |= static_cast<uint64_t>(bytes[5]) << 35;
        m_internal |= static_cast<uint64_t>(bytes[6]) << 42;
        m_internal |= static_cast<uint64_t>(bytes[7]) << 49;
    }
};

inline std::ostream& operator<<(std::ostream& os, word56_le_t word) {
    return os << "word56_t{" << word.m_internal << "}";
}

struct word28_le_t : word_base<word28_le_t> {
    using word_base::word_base;

    explicit word28_le_t(uint32_t raw, bool flip = true) {
        m_internal = raw;
        if (flip) {
            std::array<uint8_t, 4> bytes;
            bytes[0] = get_byte7(3);
            bytes[1] = get_byte7(2);
            bytes[2] = get_byte7(1);
            bytes[3] = get_byte7(0);
            *this = word28_le_t(bytes);
        }
    }

    explicit word28_le_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[0];
        m_internal |= static_cast<uint32_t>(bytes[1]) << 7;
        m_internal |= static_cast<uint32_t>(bytes[2]) << 14;
        m_internal |= static_cast<uint32_t>(bytes[3]) << 21;
    }

    uint8_t get_byte7(int idx) {
        return (m_internal >> (idx * 7)) & 0x7F;
    }
};

inline std::ostream& operator<<(std::ostream& os, word28_le_t word) {
    return os << "word28_t{" << word.m_internal << "}";
}

struct word14_le_t : word_base<word14_le_t> {
    explicit word14_le_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[0];
        m_internal |= static_cast<uint16_t>(bytes[1]) << 7;
    }
};

inline std::ostream& operator<<(std::ostream& os, word14_le_t word) {
    return os << "word14_t{" << word.m_internal << "}";
}

struct word14_be_t : word_base<word14_be_t> {
    explicit word14_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[1];
        m_internal |= static_cast<uint16_t>(bytes[0]) << 7;
    }
};

struct word21_be_t : word_base<word21_be_t> {
    explicit word21_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[2];
        m_internal |= static_cast<uint32_t>(bytes[1]) << 7;
        m_internal |= static_cast<uint32_t>(bytes[0]) << 14;
    }
};

struct word28_be_t : word_base<word28_be_t> {
    using word_base::word_base;

    explicit word28_be_t(uint32_t raw) {
        m_internal = raw;
    }

    explicit word28_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[3];
        m_internal |= static_cast<uint32_t>(bytes[2]) << 7;
        m_internal |= static_cast<uint32_t>(bytes[1]) << 14;
        m_internal |= static_cast<uint32_t>(bytes[0]) << 21;
    }

    uint8_t get_byte7(int idx) {
        return (m_internal >> (idx * 7)) & 0x7F;
    }
};

struct word35_be_t : word_base<word35_be_t> {
    explicit word35_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[4];
        m_internal |= static_cast<uint64_t>(bytes[3]) << 7;
        m_internal |= static_cast<uint64_t>(bytes[2]) << 14;
        m_internal |= static_cast<uint64_t>(bytes[1]) << 21;
        m_internal |= static_cast<uint64_t>(bytes[0]) << 28;
    }
};

struct word42_be_t : word_base<word42_be_t> {
    explicit word42_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[5];
        m_internal |= static_cast<uint64_t>(bytes[4]) << 7;
        m_internal |= static_cast<uint64_t>(bytes[3]) << 14;
        m_internal |= static_cast<uint64_t>(bytes[2]) << 21;
        m_internal |= static_cast<uint64_t>(bytes[1]) << 28;
        m_internal |= static_cast<uint64_t>(bytes[0]) << 35;
    }
};
} // namespace x595g