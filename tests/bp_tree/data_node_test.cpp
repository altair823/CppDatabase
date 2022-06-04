//
// Created by 김태현 on 2022/06/02.
//

#include <gtest/gtest.h>
#include <data.h>
#include <data_node.h>
#include <f_string.h>
#include <f_int.h>

TEST(DataNodeTest, InsertionTest){
  auto data1 = DataFactory<Int, Int>::create(Int(1), Int(1));
  auto data2 = DataFactory<Int, Int>::create(Int(2), Int(2));
  auto data3 = DataFactory<Int, Int>::create(Int(3), Int(3));
  auto data4 = DataFactory<Int, Int>::create(Int(4), Int(4));
  auto data_node = DataNodeFactory<Int, Int>::create();
  data_node->push_back(std::move(data1));
  data_node->insert(1, std::move(data2));
  data_node->insert(1, std::move(data3));
  ASSERT_EQ(data_node->get_key(0), Int(1));
  ASSERT_EQ(data_node->get_key(1), Int(3));
  ASSERT_EQ(data_node->get_key(2), Int(2));
  ASSERT_TRUE(data_node->insert(5, std::move(data4)).isErr());
}

TEST(DataNodeTest, SearchTest){
  auto data1 = DataFactory<Int, Int>::create(Int(1), Int(1));
  auto data2 = DataFactory<Int, Int>::create(Int(2), Int(2));
  auto data3 = DataFactory<Int, Int>::create(Int(4), Int(4));
  auto data4 = DataFactory<Int, Int>::create(Int(5), Int(5));
  auto data_node = DataNodeFactory<Int, Int>::create();
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));

  ASSERT_EQ(data_node->search(Int(2)), 1);
  ASSERT_EQ(data_node->search(Int(3)), 2);
  ASSERT_EQ(data_node->search(Int(4)), 2);
  ASSERT_EQ(data_node->search(Int(5)), 3);
  ASSERT_EQ(data_node->search(Int(6)), 4);
}