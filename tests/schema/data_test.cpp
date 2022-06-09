//
// Created by 김태현 on 2022/06/04.
//

#include <gtest/gtest.h>
#include <data.h>
#include <f_string.h>
#include <f_datetime.h>
#include <record.h>
#include <bp_tree_data.h>
#include "type.h"

TEST(DataTest, SerializeTest){
  SchemaBuilder schema_builder("test schema");
  auto schema = schema_builder.set_field(TypeKind::STRING, "pk_str", KeyType::PK).unwrap()
      ->set_field(TypeKind::STRING, "fk_str", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "Created date").unwrap()
      ->build().unwrap();

  Record record(*schema);
  record.set_field(std::make_shared<String>("key1"), "pk_str").unwrap();
  record.set_field(std::make_shared<String>("key2"), "fk_str").unwrap();
  record.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date").unwrap();

  auto data = DBData(record);
  auto data_binary = data.serialize();
  auto new_record = Record(*schema);
  auto new_data = DBData(new_record);
  new_data.deserialize(*data_binary, 0).unwrap();
  //std::cout<<*new_data;
  ASSERT_EQ(new_data, data);
}
