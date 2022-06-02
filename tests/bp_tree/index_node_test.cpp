//
// Created by 김태현 on 2022/06/01.
//

#include <gtest/gtest.h>
#include <index_node.h>
#include <f_string.h>
#include <f_datetime.h>
#include <db_io.h>

TEST(IndexNodeTest, SerializeTest){
  std::vector<String> keys;
  keys.emplace_back("key1");
  keys.emplace_back("key2");
  keys.emplace_back("key3");
  std::vector<DBPointer> pointers;
  pointers.emplace_back("db1.txt", 0);
  pointers.emplace_back("db2.txt", 3);
  pointers.emplace_back("db3.txt", 126275);
  pointers.emplace_back("db4.txt", 35217175);
  auto index_node = IndexNodeFactory<String>::create(keys, pointers);
  auto binary = index_node->serialize();
  auto new_index_node = IndexNodeFactory<String>::create();
  new_index_node->deserialize(*binary, 0);

  ASSERT_EQ(*index_node, *new_index_node);
}