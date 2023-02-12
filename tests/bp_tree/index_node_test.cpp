//
// Created by 김태현 on 2022/06/01.
//

#include <gtest/gtest.h>
#include <index_node.h>
#include <f_string.h>
#include <f_datetime.h>
#include <db_io.h>
#include <filesystem>

TEST(IndexNodeTest, SerializeTest){
//  std::vector<String> keys;
//  keys.emplace_back("key1");
//  keys.emplace_back("key2");
//  keys.emplace_back("key3");
//  std::vector<DBPointer> pointers;
//  pointers.emplace_back("db1.txt", 0, 0);
//  pointers.emplace_back("db2.txt", 3, 5);
//  pointers.emplace_back("db3.txt", 126275, 4261);
//  pointers.emplace_back("db4.txt", 35217175, 537246);
//  auto index_node = IndexNodeFactory::create(nullptr, keys, pointers);
//  index_node->set_leaf(NOT_LEAF);
//  auto binary = index_node->serialize();
//  auto new_index_node = IndexNodeFactory::create(nullptr);
//  new_index_node->deserialize(*binary, 0).unwrap();
//
//  ASSERT_EQ(*index_node, *new_index_node);
}

TEST(IndexNodeTest, IndexChildTest){
  auto test_filename = "index_node_db_test";

  auto schema = SchemaBuilder("pointer_test_schema")
      .set_field(TypeKind::STRING, "pk", KeyType::PK)
      .unwrap()->build().unwrap();

  std::vector<FieldShared> keys;
  keys.push_back(FieldFactory::create("pk", std::make_shared<String>("key1")));
  keys.push_back(FieldFactory::create("pk", std::make_shared<String>("key2")));
  keys.push_back(FieldFactory::create("pk", std::make_shared<String>("key3")));
  std::vector<DBPointer> pointers;
  pointers.emplace_back("db1.txt", 0, 0);
  pointers.emplace_back("db2.txt", 3, 5);
  pointers.emplace_back("db3.txt", 126275, 4261);
  pointers.emplace_back("db4.txt", 35217175, 537246);
  auto index_node = IndexNodeFactory::create(schema, "pk", keys, pointers);
  index_node->set_leaf(LEAF);
  auto parent_node = IndexNodeFactory::create(schema, "pk");
  parent_node->set_leaf(NOT_LEAF);
  auto index_binary = index_node->serialize();
  std::filesystem::remove(test_filename);
  index_binary->save(test_filename).unwrap();
  auto db_pointer = DBPointer(test_filename, 0, (long)index_binary->get_length());
  parent_node->push_back_pointer(db_pointer);
  auto read_node = parent_node->get_index_child(0);

  ASSERT_EQ(*index_node, *read_node);
  //std::cout << *parent_node << std::endl << *index_node << std::endl << *read_node;
}

TEST(IndexNodeTest, DataChildTest){
  auto test_filename = "data_node_db_test";
  auto test_data_node_filename = "test_data_node_file.bin";

  auto schema = SchemaBuilder("test schema").set_field(TypeKind::STRING, "pk_str", KeyType::PK).unwrap()
      ->set_field(TypeKind::STRING, "fk_str", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "Created date").unwrap()
      ->build().unwrap();

  auto data1 = RecordFactory::create(schema);
  auto data2 = RecordFactory::create(schema);
  auto data3 = RecordFactory::create(schema);
  auto data4 = RecordFactory::create(schema);
  auto data_node = DataNodeFactory::create(schema, "pk_str", test_data_node_filename);
  data_node->push_back(std::move(data1));
  data_node->push_back(std::move(data2));
  data_node->push_back(std::move(data3));
  data_node->push_back(std::move(data4));
  auto data_binary = data_node->serialize();
  std::filesystem::remove(test_filename);
  data_binary->save(test_filename).unwrap();
  auto db_pointer = DBPointer(test_filename, 0, (long)data_binary->get_length());

  auto parent_node = IndexNodeFactory::create(schema, "pk_str");
  parent_node->set_leaf(LEAF);
  parent_node->push_back_pointer(db_pointer);

  auto read_node = parent_node->get_data_child(0, test_data_node_filename);
  ASSERT_EQ(*data_node, *read_node);
  //std::cout << *parent_node << std::endl << *data_node << std::endl << *read_node;
}