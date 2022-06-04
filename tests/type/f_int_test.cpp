//
// Created by 김태현 on 2022/06/04.
//

#include <gtest/gtest.h>
#include <f_int.h>

TEST(IntTest, SerializeTest){
  Int i(541377157);
  auto binary = i.serialize();
  Int j(541377157);
  ASSERT_EQ(i, j);
}

TEST(IntTest, DeserializeTest){
  Int i(541377157);
  auto binary = i.serialize();
  Int j;
  j.deserialize(*binary, 0);
  //std::cout<<j;
  ASSERT_EQ(i, j);
}