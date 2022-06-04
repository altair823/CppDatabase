//
// Created by 김태현 on 2022/06/04.
//

#ifndef CPPDATABASE_INCLUDES_TYPE_F_INT_H_
#define CPPDATABASE_INCLUDES_TYPE_F_INT_H_

#include <field_data.h>
#include <mem_core.h>

class Int : public FieldData {
 public:
  Int(): value(0), FieldData(Type::INT) {}
  explicit Int(int i): value(i), FieldData(Type::INT) {}
  ~Int() override = default;
  [[nodiscard]] BinaryUnique serialize() const override;
 private:
  bool under(const FieldData &rhs) const override;
 public:
  [[nodiscard]] BinaryIndex get_total_byte_size() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
 private:
  [[nodiscard]] bool eq(const FieldData &rhs) const override;
  std::ostream &out(std::ostream &ostream) const override;

  int32 value;
};

#endif //CPPDATABASE_INCLUDES_TYPE_F_INT_H_
