//
// Created by 김태현 on 2022/05/05.
//

#include <gtest/gtest.h>
#include <memory>
#include <schema.h>
#include "type.h"

TEST(SchemaTest, SetFieldTest){
  SchemaBuilder builder("test_schema");

  auto schema = builder.set_field(TypeKind::STRING, "string1", KeyType::PK).unwrap()
      ->set_field(TypeKind::DATETIME, "datetime1", KeyType::FK).unwrap()
      ->set_field(TypeKind::DATETIME, "datetime2").unwrap()->build().unwrap();

  // std::cout<< *schema << std::endl;
}
