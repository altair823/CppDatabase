//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_RECORD_SCHEMA_H_
#define CPPDATABASE_RECORD_SCHEMA_H_

#include <type.h>
#include <utility>
#include <vector>
#include <memory>
#include <ostream>
#include <algorithm>

enum class KeyType{
  NONE,
  PK,
  FK,
};

std::ostream &operator<<(std::ostream &os, const KeyType &key_type);

class FieldSchema{
 public:
  FieldSchema(TypeKind type, std::string name): type(type), name(std::move(name)) {}

  friend std::ostream &operator<<(std::ostream &os, const FieldSchema &schema) {
    os << "type: " << schema.type << " name: " << schema.name;
    return os;
  }
  bool operator==(const FieldSchema &rhs) const {return type == rhs.type && name == rhs.name;}
  bool operator!=(const FieldSchema &rhs) const {return !(rhs == *this);}

  [[nodiscard]] TypeKind get_type() const {return type;}
  [[nodiscard]] std::string get_name() const {return name;}

 private:
  TypeKind type;
  std::string name;
};

class SchemaBuilder;

class Schema;
typedef std::shared_ptr<Schema> SchemaShared;

constexpr unsigned int UNDEFINED_INDEX = -1;
class Schema {
 public:
  explicit Schema(std::string schema_name);

  [[nodiscard]] Result<FieldSchema, NotFound> get_field(const std::string& field_name) const;
  template<typename Predicate>
  Result<FieldSchema, NotFound> get_field(const std::string& field_name, Predicate predicate) const;
  [[nodiscard]] Result<FieldSchema, NotFound> get_field(unsigned int index) const;
  [[nodiscard]] Result<FieldSchema, NotFound> get_field(KeyType key_type, const std::string& field_name = "") const;
  [[nodiscard]] Result<KeyType, NotFound> get_field_key_type(const std::string& field_name) const;

  friend std::ostream &operator<<(std::ostream &os, const Schema &schema);
  friend class SchemaBuilder;
  bool operator==(const Schema &rhs) const;
  bool operator!=(const Schema &rhs) const;

  using iterator = std::vector<FieldSchema>::iterator;
  using const_iterator = std::vector<FieldSchema>::const_iterator;

  iterator begin() {return fields.begin();}
  iterator end() {return fields.end();}
  [[nodiscard]] const_iterator cbegin() const {return fields.cbegin();}
  [[nodiscard]] const_iterator cend() const {return fields.cend();}

 private:
  std::string schema_name;
  unsigned int pk_index;
  std::vector<unsigned int> fk_indexes;
  std::vector<FieldSchema> fields;
};

class SchemaBuilder {
 public:
  explicit SchemaBuilder(std::string schema_name);
  Result<SchemaBuilder*, AlreadyExist> set_field(TypeKind type, const std::string& name, KeyType key_type = KeyType::NONE);
  Result<SchemaShared, NotFound> build();

 private:
  std::string schema_name;
  unsigned int pk_index;
  std::vector<unsigned int> fk_indexes;
  std::vector<FieldSchema> fields;
};


#endif //CPPDATABASE_RECORD_SCHEMA_H_
