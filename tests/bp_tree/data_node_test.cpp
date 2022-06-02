//
// Created by 김태현 on 2022/06/02.
//

#include <gtest/gtest.h>
#include <data.h>
#include <data_node.h>

TEST(DataNodeTest, InsertionTest){
  auto data1 = DataFactory<int, int>::create(1, 1);
  auto data2 = DataFactory<int, int>::create(2, 2);
  auto data3 = DataFactory<int, int>::create(3, 3);
  auto data4 = DataFactory<int, int>::create(4, 4);
  auto data_node = DataNodeFactory<int, int>::create();
  data_node->push_back(std::move(data1));
  data_node->insert(1, std::move(data2));
  data_node->insert(1, std::move(data3));
  ASSERT_EQ(data_node->get_key(0), 1);
  ASSERT_EQ(data_node->get_key(1), 3);
  ASSERT_EQ(data_node->get_key(2), 2);
  ASSERT_TRUE(data_node->insert(5, std::move(data4)).isErr());
}

TEST(DataNodeTest, SearchTest){
  auto data1 = DataFactory<int, int>::create(1, 1);
  auto data2 = DataFactory<int, int>::create(2, 2);
  auto data3 = DataFactory<int, int>::create(4, 4);
  auto data4 = DataFactory<int, int>::create(5, 5);
  auto data_node = DataNodeFactory<int, int>::create();
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));

  ASSERT_EQ(data_node->search(2), 1);
  ASSERT_EQ(data_node->search(3), 2);
  ASSERT_EQ(data_node->search(4), 2);
  ASSERT_EQ(data_node->search(5), 3);
  ASSERT_EQ(data_node->search(6), 4);
}