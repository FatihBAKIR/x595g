//
// Created by fatih on 3/5/20.
//

#include <boost/multiprecision/cpp_int.hpp>
#include <thread>

namespace mp = boost::multiprecision;
template<size_t Bits>
using num =
    mp::number<mp::cpp_int_backend<Bits, Bits, mp::signed_magnitude, mp::checked, void>>;

using int28_t = num<28>;
using int7_t = num<7>;
using int14_t = num<14>;

int28_t A(const int7_t* idx, int28_t iter) noexcept {
    int28_t accum = 0;

    while (iter != 0) {
        int14_t multiplied = int14_t(*idx) * 7;
        int7_t cur(multiplied & 127);
        cur |= 0b1001001;
        cur /= 3;
        accum ^= cur;
        ++idx;
        --iter;
    }

    return accum;
}

std::array<int7_t, 32> key{1, 0, 0, 89, 67, 3, 0,  0,  49, 40, 1,  0, 0, 0, 89, 45,
                           1, 0, 0, 40, 51, 0, 39, 83, 0,  48, 48, 8, 0, 0, 0,  43};

bool tryit(const std::array<int7_t, 32>& input) {
    for (int i = 7; i <= 7; ++i) {
        if (key[i] != A(input.data(), i)) {
            return false;
        }
    }
    return true;
}

int main() {
    std::vector<std::thread> threads(std::thread::hardware_concurrency());
    const auto top_level_iters_per_thread = 128 / threads.size();

    int thread_id = 0;
    for (auto& t : threads) {
        t = std::thread([=]{
          std::array<int7_t, 32> input{};
          auto end = (thread_id + 1) * top_level_iters_per_thread;
          for (int i0 = thread_id * top_level_iters_per_thread; i0 < end; ++i0) {
              input[0] = i0;
              for (int i1 = 0; i1 < 128; ++i1) {
                  input[1] = i1;
                  for (int i2 = 0; i2 < 128; ++i2) {
                      std::cerr << "2nd bit: " << i2 << '\n';
                      input[2] = i2;
                      for (int i3 = 0; i3 < 128; ++i3) {
                          input[3] = i3;
                          for (int i4 = 0; i4 < 128; ++i4) {
                              input[4] = i4;
                              for (int i5 = 0; i5 < 128; ++i5) {
                                  input[5] = i5;
                                  for (int i6 = 0; i6 < 128; ++i6) {
                                      input[6] = i6;
                                      if (tryit(input)) {
                                          std::cerr << "found it!\n";
                                          for (int i = 0; i < 7; ++i) {
                                              std::cout << input[i] << ", ";
                                          }
                                          std::cout << '\n';
                                      }
                                  }
                              }
                          }
                      }
                  }
              }
          }
        });
        ++thread_id;
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}