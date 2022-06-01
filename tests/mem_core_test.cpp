//
// Created by 김태현 on 2022/06/01.
//

#include <gtest/gtest.h>
#include <mem_core.h>
#include <f_datetime.h>
#include <f_string.h>

TEST(BinaryTest, OperatorTest){
  auto binary1 = DateTime(20, 12, 30, 9, 32, 54).serialize();
  auto binary2 = String("testtesttesttesttesttesttesttest").serialize();
  auto binary3 = *binary1 + *binary2;
}