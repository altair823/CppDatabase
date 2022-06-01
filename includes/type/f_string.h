//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_F_STRING_H
#define CPPDATABASE_F_STRING_H

#include <field_data.h>
#include <mem_core.h>
#include <cmath>
#include <ostream>
#include <iostream>
#include <utility>

class String : public FieldData {
 public:

  String();
  explicit String(std::string str);
  BinaryUnique serialize() const override;
  Result<BINARY_INDEX, DeserializeError> deserialize(const Binary &binary, BINARY_INDEX begin) override;
  [[nodiscard]] int get_total_byte_size() const override;
  [[nodiscard]] std::string get_string() const {return str;}
  void set_string(std::string new_str) {str = std::move(new_str);}

 private:
  std::string str;
  [[nodiscard]] bool eq(const FieldData &field_type) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_STRING_H
