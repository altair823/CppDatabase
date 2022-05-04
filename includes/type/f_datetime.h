//
// Created by Administrator on 2022-05-01.
//

#ifndef CPPDATABASE_F_DATETIME_H
#define CPPDATABASE_F_DATETIME_H

#include <bitset>
#include <mem_core.h>
#include <field_data.h>
#include <ostream>
#include <iostream>

class DateTime : public FieldData {
 public:
  unsigned char f_year; // 8bits
  unsigned char f_month; // 4
  unsigned char f_day; // 8
  unsigned char f_hour; // 8
  unsigned char f_min; // 8
  int get_total_byte_size() const override;
  unsigned char f_sec; // 8

  DateTime();
  DateTime(int year, char month, char day, char hour, char min, char sec);

  Binary serialize() override;
  Result<int> deserialize(BinaryRef binary, int begin) override;

 private:
  bool eq(const FieldData &rhs) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_DATETIME_H
