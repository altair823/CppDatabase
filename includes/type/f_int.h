//
// Created by 김태현 on 2022/06/04.
//

#ifndef CPPDATABASE_INCLUDES_TYPE_F_INT_H_
#define CPPDATABASE_INCLUDES_TYPE_F_INT_H_

#include <type.h>
#include <mem_core.h>

class Int : public Type {
 public:
  Int(): value(0), Type(TypeKind::INT) {}
  explicit Int(int i): value(i), Type(TypeKind::INT) {}
  ~Int() override = default;
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  int32 get_int() const {return value;}
 private:
  [[nodiscard]] bool eq(const Type &rhs) const override;
  [[nodiscard]] bool under(const Type &rhs) const override;
  std::ostream &out(std::ostream &ostream) const override;

  int32 value;
};

#endif //CPPDATABASE_INCLUDES_TYPE_F_INT_H_
