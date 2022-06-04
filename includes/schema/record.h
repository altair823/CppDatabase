//
// Created by 김태현 on 2022/05/04.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
#define CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_

#include <utility>
#include <vector>
#include <utility>
#include <field_data.h>
#include <mem_core.h>
#include <ostream>
#include <serializable.h>

#include <schema.h>

#include <f_string.h>
#include <f_datetime.h>

#include <error.h>


struct Field{
  Field(FieldDataShared data, std::string name): data(std::move(data)), name(std::move(name)){}
  std::shared_ptr<FieldData> data;
  std::string name;
};

typedef std::shared_ptr<Field> FieldShared;

class Record : public Serializable{
 public:
  explicit Record(Schema schema);

  Result<bool, AlreadyExist> set_field(std::shared_ptr<FieldData> data, const std::string& field_name);
  [[nodiscard]] Result<FieldShared, NotFound> get_field(const std::string& field_name) const;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] BinaryUnique serialize() const;

  [[nodiscard]] int get_total_byte_size() const;
  [[nodiscard]] Schema get_schema() const {return schema;}
  friend std::ostream &operator<<(std::ostream &os, const Record &schema);
  bool operator==(const Record &rhs) const;

  std::vector<Field> fields;
  Schema schema;
};

typedef std::unique_ptr<Record> RecordUnique;

Result<FieldDataShared, CannotConvert> type_to_field(Type type);

#endif //CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
