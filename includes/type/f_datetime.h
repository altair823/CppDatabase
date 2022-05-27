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
  int get_total_byte_size() const override;

  DateTime();
  DateTime(int year, char month, char day, char hour, char min, char sec);

  Binary serialize() override;
  Result<BINARY_INDEX, DeserializeError> deserialize(BinaryRef binary, BINARY_INDEX begin) override;

  [[nodiscard]] unsigned char get_year() const {return f_year;}
  [[nodiscard]] unsigned char get_month() const {return f_month;}
  [[nodiscard]] unsigned char get_day() const {return f_day;}
  [[nodiscard]] unsigned char get_hour() const {return f_hour;}
  [[nodiscard]] unsigned char get_min() const {return f_min;}
  [[nodiscard]] unsigned char get_sec() const {return f_sec;}
  void set_year(unsigned char f_year) {
    DateTime::f_year = f_year;
  }
  void set_month(unsigned char f_month) {
    DateTime::f_month = f_month;
  }
  void set_day(unsigned char f_day) {
    DateTime::f_day = f_day;
  }
  void set_hour(unsigned char f_hour) {
    DateTime::f_hour = f_hour;
  }
  void set_min(unsigned char f_min) {
    DateTime::f_min = f_min;
  }
  void set_sec(unsigned char f_sec) {
    DateTime::f_sec = f_sec;
  }
 private:
  unsigned char f_year; // 8bits
  unsigned char f_month; // 4
  unsigned char f_day; // 8
  unsigned char f_hour; // 8
  unsigned char f_min; // 8
  unsigned char f_sec; // 8
  [[nodiscard]] bool eq(const FieldData &rhs) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_DATETIME_H
