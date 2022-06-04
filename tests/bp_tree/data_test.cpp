//
// Created by 김태현 on 2022/06/04.
//

#include <gtest/gtest.h>
#include <data.h>
#include <f_string.h>
#include <f_datetime.h>
#include <record.h>

TEST(DataTest, SerializeTest){
  SchemaBuilder schema_builder("test schema");
  auto schema = schema_builder.set_field(Type::STRING, "pk", KeyType::PK).unwrap()
      ->set_field(Type::STRING, "fk", KeyType::FK).unwrap()
      ->set_field(Type::DATETIME, "Created date").unwrap()
      ->build().unwrap();
  Record value(*schema);
  value.set_field(std::make_shared<String>("key1"), "pk");
  value.set_field(std::make_shared<String>("key2"), "fk");
  value.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  //auto data = DataFactory<Field, Record>::create(value.get_field("pk").unwrap()->data, value);
}
