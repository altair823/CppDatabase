//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_F_STRING_H
#define CPPDATABASE_F_STRING_H

#include <type.h>
#include <mem_core.h>
#include <cmath>
#include <ostream>
#include <iostream>
#include <utility>

class String : public Type {
 public:

  String();
  explicit String(std::string str);
  ~String() override = default;
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] std::string get_string() const {return str;}
  void set_string(std::string new_str) {str = std::move(new_str);}

 private:
  std::string str;
  [[nodiscard]] bool eq(const Type &field_type) const override;
  [[nodiscard]] bool under(const Type &rhs) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_STRING_H
