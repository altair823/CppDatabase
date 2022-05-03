//
// Created by 김태현 on 2022/05/04.
//

#include <gtest/gtest.h>
#include <schema.h>
#include <field_type.h>
#include <f_datetime.h>
#include <f_string.h>

TEST(SchemaTest, SetFieldTest){
  Schema schema;
  schema.set_field(std::make_shared<DateTime>(), "Created date");
  std::cout<<*schema.fields[0].type<<std::endl;

  auto origin_str = std::make_shared<String>();
  auto b = origin_str->serialize();
  auto str = std::make_shared<String>();
  str->deserialize(b);

  str->str = "this is string type variable. 이것은 문자열 타입 변수입니다.";
  schema.set_field(str, "string");
  std::cout<<*schema.fields[1].type<<std::endl;
}