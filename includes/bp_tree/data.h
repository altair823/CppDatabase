//
// Created by 김태현 on 2022/06/08.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_DB_DATA_H_
#define CPPDATABASE_INCLUDES_SCHEMA_DB_DATA_H_

#include "data.h"
#include <ostream>
#include "schema/field.h"
#include "schema/record.h"


class Data : public Serializable {
 public:
  Data(const Record& record);
  [[nodiscard]] BinaryUnique serialize() const;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin);
  [[nodiscard]] std::shared_ptr<Field> get_key() const;
  [[nodiscard]] std::shared_ptr<Record> get_value() const;
  friend std::ostream &operator<<(std::ostream &os, const Data &data);
  bool operator==(const Data &rhs) const;
  bool operator!=(const Data &rhs) const;

 private:
  [[nodiscard]] bool eq(const Data &rhs) const;

  FieldShared key;
  RecordShared value;
};

class DataFactory{
 public:
  explicit DataFactory(SchemaShared schema_shared);
  std::unique_ptr<Data> create_data();
 private:
  SchemaShared schema_shared;
};

using Key = FieldShared;
using Value = RecordShared;

#endif //CPPDATABASE_INCLUDES_SCHEMA_DB_DATA_H_
