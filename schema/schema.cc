//
// Created by 김태현 on 2022/05/04.
//

#include <schema.h>

Schema::Schema() = default;
Result Schema::set_field(std::shared_ptr<FieldType> type, const std::string& field_name) {
  for (const auto& f: fields){
    if (f.name == field_name){
      return {Err, "Cannot set a new variable. Same variable name is already exist! : " + field_name};
    }
  }
  fields.push_back(Field{std::move(type), field_name});
  return {Ok};
}
std::shared_ptr<FieldType> Schema::get_field(std::string field_name) {
  return std::shared_ptr<FieldType>();
}
