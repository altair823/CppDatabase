//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_F_STRING_H
#define CPPDATABASE_F_STRING_H

#include <field_type.h>
#include <mem_core.h>
#include <cmath>
#include <ostream>
#include <iostream>

class String : public FieldType {
 public:
  std::string str;

  String();
  std::unique_ptr<unsigned char[]> serialize() override;
  void deserialize(std::unique_ptr<unsigned char[]> &) override;

  bool operator==(const String &rhs) const;

 private:
  static int get_byte_count(int size);
  static int write_str_size_bits(std::unique_ptr<unsigned char[]> &binary, int size, int byte_count);
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_STRING_H
