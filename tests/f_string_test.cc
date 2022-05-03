//
// Created by 김태현 on 2022/05/03.
//

#include <gtest/gtest.h>
#include <field_type.h>
#include <mem_core.h>
#include <f_string.h>
#include <test_util.h>

int get_byte_count(int size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}
TEST(f_string_test, string_bit_test) {
  std::string str =
      "달아 달아 밝은 달아 이태백이 놀던 달아 저기 저기 저달 속에 계수 나무 박혔으니 옥도끼로 찍어내어 금도끼로 다듬어서 초가삼간 집을 짓고 양친 부모 모셔다가 천년 만년 살고지고 천년 만년 살고지고";
  auto v = std::vector<unsigned char>();
  for (char i : str) {
    v.push_back(i);
  }

  std::string a;
  std::unique_ptr<unsigned char[]> p(new unsigned char[a.size()]);
  for (int i = 0; i < str.size(); i++) {
    a.push_back((char) v[i]);
    set_mem(p[i], v[i]);
  }

  //print_bits(p, a.size());

  ASSERT_EQ(str, a);

  auto str_bit_count = str.size();
  auto size_bit_count = get_byte_count((int) str_bit_count);
  //std::cout<<str_bit_count<<" "<<size_bit_count<<std::endl;

  ASSERT_EQ(str_bit_count, 269);
  ASSERT_EQ(size_bit_count, 2);
}

TEST(f_string_test, SerializeTest) {
  String a;
  a.str =
      "달아 달아 밝은 달아 이태백이 놀던 달아 저기 저기 저달 속에 계수 나무 박혔으니 옥도끼로 찍어내어 금도끼로 다듬어서 초가삼간 집을 짓고 양친 부모 모셔다가 천년 만년 살고지고 천년 만년 살고지고";

  //std::cout<<a<<std::endl;

  auto b = a.serialize();

  //print_bits(b, 272);

  String t;
  t.deserialize(b);
  //std::cout<<t<<std::endl;

  ASSERT_EQ(a, t);
}