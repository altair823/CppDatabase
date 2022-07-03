//
// Created by 김태현 on 2022/05/27.
//

#include <gtest/gtest.h>
#include <bp_tree/bp_tree.h>
#include "record.h"
TEST(BPTreeTest, InstantiateTest){
  if (std::filesystem::exists("db_file1.bin")){
    std::filesystem::remove("db_file1.bin");
  }
  SchemaShared schema = SchemaBuilder("db_test_schema")
      .set_field(TypeKind::STRING, "string", KeyType::PK).unwrap()
      ->set_field(TypeKind::INT, "int", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "datetime").unwrap()
      ->build().unwrap();
  BPTree bp_tree(schema, "string");
}

TEST(BPTreeTest, InsertionTest){
  if (std::filesystem::exists("db_file1.bin")){
    std::filesystem::remove("db_file1.bin");
  }
  SchemaShared schema = SchemaBuilder("db_test_schema")
      .set_field(TypeKind::STRING, "string", KeyType::PK).unwrap()
      ->build().unwrap();
  BPTree bp_tree(schema, "string");
  RecordShared record = RecordFactory::create(schema);
  bp_tree.insert(record->get_pk_field().unwrap(), record);
}