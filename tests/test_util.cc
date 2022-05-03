//
// Created by 김태현 on 2022/05/04.
//
#include <test_util.h>

void print_bits(std::unique_ptr<unsigned char[]> &data, size_t length) {
  for (auto i = 0; i < length; i++) {
    auto a = data[i];
    std::bitset<8> x(a);
    std::cout << x << std::endl;
  }
}