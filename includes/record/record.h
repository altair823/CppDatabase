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

#include <f_string.h>
#include <f_datetime.h>




struct Field{
  Field(std::shared_ptr<FieldData> data, std::string name): data(std::move(data)), name(std::move(name)){}
  std::shared_ptr<FieldData> data;
  std::string name;
};

typedef std::shared_ptr<Field> FieldShared;

class Record{
 public:
  Record();

  Result<bool> set_field(std::shared_ptr<FieldData> data, const std::string& field_name);
  Result<FieldShared> get_field(const std::string& field_name) const;

  Binary serialize();

  int get_total_byte_size() const;
  friend std::ostream &operator<<(std::ostream &os, const Record &schema);
  bool operator==(const Record &rhs) const;

  std::vector<Field> fields;
};

typedef std::unique_ptr<Record> RecordUnique;

Result<FieldDataShared> type_to_field(Type type);
RecordUnique deserialize(BinaryRef binary, std::vector<std::string> &field_names);

#endif //CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
