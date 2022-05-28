//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include <ostream>
#include <utility>
#include <result.h>
#include "../error.h"
#include "types.h"
#include <mem_core.h>


class FieldData {
 public:
  FieldData();
  FieldData(const FieldData& field_data);
  virtual ~FieldData() = default;
  explicit FieldData(Type field_type);
  virtual Binary serialize() = 0;
  virtual Result<BINARY_INDEX, DeserializeError> deserialize(BinaryRef binary, BINARY_INDEX begin) = 0;
  [[nodiscard]] virtual int get_total_byte_size() const = 0;

  bool operator==(const FieldData &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const FieldData &field_type);

  Type field_type;
 private:
  [[nodiscard]] virtual bool eq(const FieldData &rhs) const = 0;
  virtual std::ostream& out(std::ostream&) const = 0;
};

typedef std::shared_ptr<FieldData> FieldDataShared;



#endif //CPPDATABASE_FIELD_INTERFACE_H
