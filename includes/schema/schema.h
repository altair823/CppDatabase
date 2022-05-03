//
// Created by 김태현 on 2022/05/04.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_SCHEMA_H_
#define CPPDATABASE_INCLUDES_SCHEMA_SCHEMA_H_

#include <vector>
#include <utility>
#include <field_type.h>

#define Ok true
#define Err false

struct Result{
  bool is_ok;
  std::string err_message;
};

struct Field{
  std::shared_ptr<FieldType> type;
  std::string name;
};

class Schema{
 public:
  Schema();

  Result set_field(std::shared_ptr<FieldType> type, const std::string& field_name);
  std::shared_ptr<FieldType> get_field(std::string field_name);

  std::vector<Field> fields;
};

#endif //CPPDATABASE_INCLUDES_SCHEMA_SCHEMA_H_
