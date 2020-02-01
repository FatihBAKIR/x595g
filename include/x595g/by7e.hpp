#pragma once

#include <cstdint>
#include <ostream>
#include <tos/span.hpp>

namespace x595g {
constexpr uint8_t flip7(uint8_t x) {
    uint8_t res = 0;
    for (int i = 0; i < 8; ++i) {
        auto mask = ((x & 0x80) >> (7 - i));
        res |= mask;
        x <<= 1;
    }
    return res >> 1;
}

constexpr uint16_t flip7(uint16_t x) {
    uint16_t res = 0;
    for (int i = 0; i < 16; ++i) {
        auto mask = ((x & 0x8000) >> (15 - i));
        res |= mask;
        x <<= 1;
    }
    return res >> 1;
}

constexpr uint32_t flip7(uint32_t x) {
    uint16_t res = 0;
    for (int i = 0; i < 32; ++i) {
        auto mask = ((x & 0x80'00'00'00) >> (31 - i));
        res |= mask;
        x <<= 1;
    }
    return res >> 4;
}


struct word56_le_t {
    word56_le_t()
        : m_internal{0} {
    }
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
    uint64_t m_internal;
};

inline std::ostream& operator<<(std::ostream& os, word56_le_t word) {
    return os << "word56_t{" << word.m_internal << "}";
}

struct word28_le_t {
    word28_le_t()
        : m_internal{0} {
    }
    explicit word28_le_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[0];
        m_internal |= static_cast<uint32_t>(bytes[1]) << 7;
        m_internal |= static_cast<uint32_t>(bytes[2]) << 14;
        m_internal |= static_cast<uint32_t>(bytes[3]) << 21;
    }
    uint32_t m_internal;
};

inline std::ostream& operator<<(std::ostream& os, word28_le_t word) {
    return os << "word28_t{" << word.m_internal << "}";
}

struct word14_le_t {
    word14_le_t()
        : m_internal{0} {
    }

    explicit word14_le_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[0];
        m_internal |= static_cast<uint16_t>(bytes[1]) << 7;
    }

    uint16_t m_internal;
};

inline std::ostream& operator<<(std::ostream& os, word14_le_t word) {
    return os << "word14_t{" << word.m_internal << "}";
}

struct word21_be_t {
    word21_be_t()
        : m_internal{0} {
    }

    explicit word21_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[2];
        m_internal |= static_cast<uint32_t>(bytes[1]) << 7;
        m_internal |= static_cast<uint32_t>(bytes[0]) << 14;
    }

    uint32_t m_internal;
};

struct word35_be_t {
    word35_be_t()
        : m_internal{0} {
    }

    explicit word35_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[4];
        m_internal |= static_cast<uint64_t>(bytes[3]) << 7;
        m_internal |= static_cast<uint64_t>(bytes[2]) << 14;
        m_internal |= static_cast<uint64_t>(bytes[1]) << 21;
        m_internal |= static_cast<uint64_t>(bytes[0]) << 28;
    }

    uint64_t m_internal;
};

struct word42_be_t {
    word42_be_t()
        : m_internal{0} {
    }

    explicit word42_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[5];
        m_internal |= static_cast<uint64_t>(bytes[4]) << 7;
        m_internal |= static_cast<uint64_t>(bytes[3]) << 14;
        m_internal |= static_cast<uint64_t>(bytes[2]) << 21;
        m_internal |= static_cast<uint64_t>(bytes[1]) << 28;
        m_internal |= static_cast<uint64_t>(bytes[0]) << 35;
    }

    uint64_t m_internal;
};

struct word14_be_t {
    word14_be_t()
        : m_internal{0} {
    }

    explicit word14_be_t(tos::span<const uint8_t> bytes) {
        m_internal = bytes[1];
        m_internal |= static_cast<uint16_t>(bytes[0]) << 7;
    }

    uint16_t m_internal;
};

struct byte7_t {
    byte7_t()
        : m_internal{0} {
    }

    byte7_t(uint8_t byte) {
        m_internal = flip7(byte);
    }

    uint8_t m_internal;
};
} // namespace x595g