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

class String : public FieldData {
 public:
  std::string str;

  String();
  explicit String(std::string str);
  Binary serialize() override;
  Result<int> deserialize(Binary &binary, int begin) override;
  int get_total_byte_size() const override;

 private:
  static int get_byte_count(int size);
  static int write_str_size_bits(std::unique_ptr<unsigned char[]> &binary, int size, int byte_count);
  bool eq(const FieldData &field_type) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_STRING_H
