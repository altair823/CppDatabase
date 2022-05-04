//
// Created by 김태현 on 2022/05/04.
//

#include <gtest/gtest.h>
#include <f_datetime.h>
#include "test/test_util.h"

TEST(DateTimeTest, SerializeTest) {
  DateTime d;
  d.f_year = 30;
  d.f_month = 10;
  d.f_day = 30;
  d.f_hour = 23;
  d.f_min = 58;
  d.f_sec = 34;

  // 00100001
  // 11101010
  // 00011110
  // 00010111
  // 00111010
  // 00100010

  auto a = d.serialize();
  //print_bits(a, 6);

  //std::cout<<d<<std::endl;

  auto c = DateTime();
  c.deserialize(a, 0);
  //std::cout<<c<<std::endl;

  ASSERT_EQ(d, c);
}