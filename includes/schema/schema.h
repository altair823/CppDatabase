//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_RECORD_SCHEMA_H_
#define CPPDATABASE_RECORD_SCHEMA_H_

#include <field_data.h>
#include <utility>
#include <vector>
#include <memory>
#include <ostream>
#include <algorithm>
#include "record.h"

enum class KeyType{
  PK,
  FK,
  NONE,
};

struct FieldSchema{
  Type type;
  std::string name;
  friend std::ostream &operator<<(std::ostream &os, const FieldSchema &schema) {
    os << "type: " << schema.type << " name: " << schema.name;
    return os;
  }

  FieldSchema(Type type, std::string name): type(type), name(std::move(name)) {}
};
class Schema;
typedef std::shared_ptr<Schema> SchemaShared;
typedef Schema* SchemaPtr;

class Schema {
 public:
  std::string schema_name;
  std::string pk_name;
  std::vector<std::string> fk_names;
  std::vector<FieldSchema> fields;

  explicit Schema(std::string schema_name);
  bool verify_record(Record* record);

  friend std::ostream &operator<<(std::ostream &os, const Schema &schema);
};

class SchemaBuilder;
typedef SchemaBuilder* SchemaBuilderPtr;

class SchemaBuilder {
 public:
  std::string schema_name;
  std::string pk_name;
  std::vector<std::string> fk_names;
  std::vector<FieldSchema> fields;

  explicit SchemaBuilder(std::string schema_name);
  Result<SchemaBuilder*, AlreadyExist> set_field(Type type, const std::string& name, KeyType key_type = KeyType::NONE);
  Result<SchemaShared, NotFound> build();
};


#endif //CPPDATABASE_RECORD_SCHEMA_H_
