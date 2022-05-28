#include <f_datetime.h>
#include <field_data.h>
#include <iostream>
#include "test/test_util.h"

void print_bits(std::unique_ptr<unsigned char[]> &data, size_t length) {
  for (size_t i = 0; i < length; i++) {
    auto a = data[i];
    std::bitset<8> x(a);
    std::cout << x << std::endl;
  }
}

int main() {
  DateTime d;
  d.set_year(30);
  d.set_month(10);
  d.set_day(30);
  d.set_hour(23);
  d.set_min(58);
  d.set_sec(34);

  // 00100001
  // 11101010
  // 00011110
  // 00010111
  // 00111010
  // 00100010

  auto a = d.serialize();
  print_bits(a);

  std::cout << d << std::endl;

  auto c = DateTime();
  c.deserialize(a, 0);
  std::cout << c << std::endl;

}

