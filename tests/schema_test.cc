//
// Created by 김태현 on 2022/05/04.
//

#include <gtest/gtest.h>
#include <schema.h>
#include <field_data.h>
#include <f_datetime.h>
#include <f_string.h>

#include "test_util.h"

TEST(SchemaTest, SetFieldTest){
  Schema schema;
  schema.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  // std::cout<<*schema.fields[0].type<<std::endl;

  auto str1 = std::make_shared<String>();

  str1->str = "this is string type variable. 이것은 문자열 타입 변수입니다.";
  schema.set_field(str1, "string");
  // std::cout<<*schema.fields[1].type<<std::endl;

  DateTime exp_dt(2022, 5, 4, 17, 5, 20);
  String exp_str("this is string type variable. 이것은 문자열 타입 변수입니다.");

  ASSERT_EQ(exp_dt, *schema.fields[0].data);
  ASSERT_EQ(exp_str, *schema.fields[1].data);
}

TEST(SchemaTest, GetFieldTest){
  Schema schema;
  schema.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  schema.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");

  auto f1 = schema.get_field("Created date").ok->data;
  auto f2 = schema.get_field("File name").ok->data;

  //std::cout<<*f1<<std::endl<<*f2<<std::endl;

  DateTime exp_dt(2022, 5, 4, 17, 5, 20);
  String exp_str("this is string type variable. 이것은 문자열 타입 변수입니다.");

  ASSERT_EQ(exp_dt, *f1);
  ASSERT_EQ(exp_str, *f2);
}

TEST(SchemaTest, VectoredSchemaTest){
  std::vector<Schema> schemas(3);
  for (auto& s: schemas){
    s.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
    s.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), "Edited date");
    s.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");
  }
//  for (auto &s: schemas){
//    std::cout<<s<<std::endl;
//  }
  Schema exp_schema;
  exp_schema.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  exp_schema.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), "Edited date");
  exp_schema.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");

  ASSERT_EQ(schemas[0], exp_schema);
}

TEST(SchemaTest, DeserializeTest){

  std::vector<std::string> field_names;
  field_names.emplace_back("Created date");
  field_names.emplace_back("Edited date");
  field_names.emplace_back("File name");

  Schema schema;
  schema.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), field_names[0]);
  schema.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), field_names[1]);
  schema.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), field_names[2]);

  //std::cout<<schema<<std::endl;
  auto b = serialize(schema);


  //print_bits(b);


  SchemaUnique schema_unique = deserialize(b, field_names);
  //std::cout<<*schema_unique<<std::endl;

  ASSERT_EQ(schema, *schema_unique);
}