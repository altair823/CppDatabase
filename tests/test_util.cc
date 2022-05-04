//
// Created by 김태현 on 2022/05/04.
//
#include <test_util.h>

void print_bits(BinaryRef binary) {
  for (auto i = 0; i < binary.length; i++) {
    auto a = binary.data[i];
    std::bitset<8> x(a);
    std::cout << x << std::endl;
  }
}