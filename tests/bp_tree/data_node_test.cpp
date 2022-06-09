//
// Created by 김태현 on 2022/06/02.
//

#include <gtest/gtest.h>
#include <data_node.h>
#include <data.h>
#include <bp_tree_data.h>
#include <f_int.h>
#include "test_data.h"



TEST(DataNodeTest, InsertionTest){
  auto data1 = std::make_unique<TestData>(TestData(Int(1), Int(1)));
  auto data2 = std::make_unique<TestData>(TestData(Int(2), Int(2)));
  auto data3 = std::make_unique<TestData>(TestData(Int(3), Int(3)));
  auto data4 = std::make_unique<TestData>(TestData(Int(4), Int(4)));
  auto data_node = DataNodeFactory<Int, Int, TestData>::create(std::make_unique<TestDataFactory>());
  data_node->push_back(std::move(data1));
  data_node->insert(1, std::move(data2));
  data_node->insert(1, std::move(data3));
  ASSERT_EQ(*data_node->get_key(0), Int(1));
  ASSERT_EQ(*data_node->get_key(1), Int(3));
  ASSERT_EQ(*data_node->get_key(2), Int(2));
}

TEST(DataNodeTest, SearchTest){
  auto data1 = std::make_unique<TestData>(TestData(Int(1), Int(1)));
  auto data2 = std::make_unique<TestData>(TestData(Int(2), Int(2)));
  auto data3 = std::make_unique<TestData>(TestData(Int(4), Int(3)));
  auto data4 = std::make_unique<TestData>(TestData(Int(5), Int(4)));
  auto data_node = DataNodeFactory<Int, Int, TestData>::create(std::make_unique<TestDataFactory>());
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

TEST(DataNodeTest, SerializeTest){
  auto schema = SchemaBuilder("test schema").set_field(TypeKind::STRING, "pk_str", KeyType::PK).unwrap()
      ->set_field(TypeKind::STRING, "fk_str", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "Created date").unwrap()
      ->build().unwrap();

  auto data1 = std::make_unique<DBData>(DBData(Record(*schema)));
  auto data2 = std::make_unique<DBData>(DBData(Record(*schema)));
  auto data3 = std::make_unique<DBData>(DBData(Record(*schema)));
  auto data4 = std::make_unique<DBData>(DBData(Record(*schema)));
  auto data_node = DataNodeFactory<Field, Record, DBData>::create(std::make_unique<DBDataFactory>(schema));
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));

  auto binary = data_node->serialize();

  auto new_data_node = DataNodeFactory<Field, Record, DBData>::create(std::make_unique<DBDataFactory>(schema));
  new_data_node->deserialize(*binary, 0).unwrap();

  ASSERT_EQ(*data_node, *new_data_node);
}