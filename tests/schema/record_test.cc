//
// Created by 김태현 on 2022/05/04.
//

#include <gtest/gtest.h>
#include <record.h>
#include <field_data.h>
#include <f_datetime.h>
#include <f_string.h>

#include "test_util.h"

TEST(RecordTest, SetFieldTest){
  Record record;
  record.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  // std::cout<<*schema.fields[0].type<<std::endl;

  auto str1 = std::make_shared<String>();

  str1->set_string("this is string type variable. 이것은 문자열 타입 변수입니다.");
  record.set_field(str1, "string");
  // std::cout<<*schema.fields[1].type<<std::endl;

  DateTime exp_dt(2022, 5, 4, 17, 5, 20);
  String exp_str("this is string type variable. 이것은 문자열 타입 변수입니다.");

  ASSERT_EQ(exp_dt, *record.fields[0].data);
  ASSERT_EQ(exp_str, *record.fields[1].data);
}

TEST(RecordTest, GetFieldTest){
  Record record;
  record.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  record.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");

  auto f1 = record.get_field("Created date").unwrap()->data;
  auto f2 = record.get_field("File name").unwrap()->data;

  //std::cout<<*f1<<std::endl<<*f2<<std::endl;

  DateTime exp_dt(2022, 5, 4, 17, 5, 20);
  String exp_str("this is string type variable. 이것은 문자열 타입 변수입니다.");

  ASSERT_EQ(exp_dt, *f1);
  ASSERT_EQ(exp_str, *f2);
}

TEST(RecordTest, VectoredTest){
  std::vector<Record> records(3);
  for (auto& s: records){
    s.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
    s.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), "Edited date");
    s.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");
  }
//  for (auto &s: schemas){
//    std::cout<<s<<std::endl;
//  }
  Record exp_record;
  exp_record.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), "Created date");
  exp_record.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), "Edited date");
  exp_record.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), "File name");

  ASSERT_EQ(records[0], exp_record);
}

TEST(RecordTest, DeserializeTest){

  std::vector<std::string> field_names;
  field_names.emplace_back("Created date");
  field_names.emplace_back("Edited date");
  field_names.emplace_back("File name");

  Record record;
  record.set_field(std::make_shared<DateTime>(2022, 5, 4, 17, 5, 20), field_names[0]);
  record.set_field(std::make_shared<DateTime>(2022, 6, 2, 20, 39, 10), field_names[1]);
  record.set_field(std::make_shared<String>("this is string type variable. 이것은 문자열 타입 변수입니다."), field_names[2]);

  //std::cout<<schema<<std::endl;
  auto b = record.serialize();


  //print_bits(b);


  RecordUnique record_unique = deserialize(*b, field_names);
  //std::cout<<*schema_unique<<std::endl;

  ASSERT_EQ(record, *record_unique);
}