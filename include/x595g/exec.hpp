//
// Created by fatih on 2/7/20.
//

#pragma once

#include <array>
#include <cstdint>
#include <x595g/stream_utils.hpp>
#include <x595g/memory.hpp>

namespace x595g {
class machine;
class interpreter {
public:
    interpreter(memory mem);

    void run(word28_le_t entry);

    ~interpreter();
private:
    std::unique_ptr<machine> m_machine;
};
}