//
// Created by 김태현 on 2022/05/04.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
#define CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_

#include <utility>
#include <vector>
#include <utility>
#include <type.h>
#include <mem_core.h>
#include <ostream>
#include <serializable.h>

#include <schema.h>

#include <field.h>

#include <error.h>



class Record : public Serializable{
 public:
  explicit Record(Schema schema);

  Result<bool, AlreadyExist> set_field(const std::shared_ptr<Type>& data, const std::string& field_name);
  [[nodiscard]] Result<FieldShared, NotFound> get_field(const std::string& field_name) const;
  [[nodiscard]] Result<FieldShared, NotFound> get_pk_field() const;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] BinaryUnique serialize() const override;

  [[nodiscard]] Schema get_schema() const {return schema;}
  friend std::ostream &operator<<(std::ostream &os, const Record &schema);
  bool operator==(const Record &rhs) const;

  std::vector<Field> fields;
  Schema schema;
};

typedef std::unique_ptr<Record> RecordUnique;

Result<TypeShared, CannotConvert> type_to_field(TypeKind type);

#endif //CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
