//
// Created by 김태현 on 2022/05/04.
//
#include <test_util.h>

void print_bits(Binary &binary) {
  for (auto i = 0; i < binary.get_length(); i++) {
    auto a = binary.read_mem(i);
    std::bitset<8> x(a);
    std::cout << x << std::endl;
  }
}