#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>

constexpr uint8_t flip7(uint8_t x) {
    uint8_t res = 0;
    for (int i = 0; i < 8; ++i) {
        auto mask = ((x & 0x80) >> (7 - i));
        res |= mask;
        x <<= 1;
    }
    return res >> 1;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1], std::ios::binary);

    auto read_8_by7e = [&]() -> std::array<uint8_t, 8> {
        uint64_t large;
        file.read(reinterpret_cast<char*>(&large), 7);
        std::reverse(reinterpret_cast<char*>(&large) + 0,
                     reinterpret_cast<char*>(&large) + 7);
        large <<= 8;

        std::array<uint8_t, 8> res;
        for (auto& r : res) {
            r = ((large & 0xFE'00'00'00'00'00'00'00) >> 56);
            r >>= 1;
            r &= 0x7F;
            large <<= 7;
            //r = flip7(r);
        }
        return res;
    };

    while (!file.eof()) {
        auto bytes = read_8_by7e();
        std::cout.write(reinterpret_cast<const char*>(bytes.data()), 8);
    }
}
