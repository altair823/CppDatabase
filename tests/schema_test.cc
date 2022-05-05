//
// Created by 김태현 on 2022/05/05.
//

#include <gtest/gtest.h>
#include <memory>
#include <schema.h>

TEST(SchemaTest, SetFieldTest){
  SchemaBuilder builder("test_schema");

  auto schema = builder.set_field(Type::STRING, "string1", KeyType::PK).unwrap()
      ->set_field(Type::DATETIME, "datetime1", KeyType::FK).unwrap()
      ->set_field(Type::DATETIME, "datetime2").unwrap()->build().unwrap();

  std::cout<< *schema << std::endl;
}

TEST(SchemaTest, VerifyRecordTest){
  SchemaBuilder builder("test_schema");

  auto schema = builder.set_field(Type::STRING, "string1", KeyType::PK).unwrap()
      ->set_field(Type::DATETIME, "datetime1", KeyType::FK).unwrap()
      ->set_field(Type::DATETIME, "datetime2").unwrap()->build().unwrap();

  auto record = std::make_unique<Record>();
  record->set_field(std::make_shared<String>("stringggg"), "string1");
  record->set_field(std::make_shared<DateTime>(20, 3, 14, 23, 25, 5), "datetime1");
  record->set_field(std::make_shared<DateTime>(12, 2, 23, 2, 35, 55), "datetime2");

  ASSERT_TRUE(schema->verify_record(record.get()));
}