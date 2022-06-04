//
// Created by 김태현 on 2022/05/06.
//

#include <gtest/gtest.h>
#include <table.h>
#include "../storage/bp_tree/bp_tree.h"

TEST(TableTest, InsertTest){
  SchemaBuilder builder("test_schema");

  auto schema = builder.set_field(Type::STRING, "string1", KeyType::PK).unwrap()
      ->set_field(Type::DATETIME, "datetime1", KeyType::FK).unwrap()
      ->set_field(Type::DATETIME, "datetime2").unwrap()->build().unwrap();

  Table table(schema, std::make_shared<BPTree<Field, Record>>("test_db_file"));

  auto record = std::make_unique<Record>(*schema);
  record->set_field(std::make_shared<String>("stringggg"), "string1");
  record->set_field(std::make_shared<DateTime>(20, 3, 14, 23, 25, 5), "datetime1");
  record->set_field(std::make_shared<DateTime>(12, 2, 23, 2, 35, 55), "datetime2");

  table.add_record(*record);

}