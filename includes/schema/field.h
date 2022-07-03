//
// Created by 김태현 on 2022/06/05.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_
#define CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_

#include <serializable.h>

#include <schema.h>
#include <f_string.h>
#include <f_datetime.h>
#include <f_int.h>
#include <ostream>
#include <utility>

class Field;

typedef std::shared_ptr<Field> FieldShared;

class FieldFactory {
 public:
  static FieldShared create();
  static FieldShared create(const FieldSchema& field_schema);
  static FieldShared create(std::string name);
  static FieldShared create(std::string name, TypeShared data);
};

Result<TypeShared, CannotConvert> type_to_field(TypeKind type);

class [[nodiscard]] Field : public Serializable{
 public:
  [[nodiscard]] TypeShared get_data() const {return data;}
  [[nodiscard]] std::string get_name() const {return name;}
  void set_data(TypeShared new_data) {data = std::move(new_data);}
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  friend std::ostream &operator<<(std::ostream &os, const Field &field);
  bool operator==(const Field &rhs) const;
  bool operator!=(const Field &rhs) const;
  bool operator<(const Field &rhs) const;
  bool operator>(const Field &rhs) const;
  bool operator<=(const Field &rhs) const;
  bool operator>=(const Field &rhs) const;
 private:
  friend class FieldFactory;
  Field(): data(nullptr) {}
  explicit Field(const FieldSchema& field_schema): name(field_schema.get_name()), data(type_to_field(field_schema.get_type()).unwrap()) {}
  explicit Field(std::string name): data(nullptr), name(std::move(name)) {}
  Field(std::string name, TypeShared data): data(std::move(data)), name(std::move(name)){}
  TypeShared data;
  std::string name;
};

bool field_shared_comparator(const FieldShared& field1, const FieldShared& field2);

#endif //CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_
