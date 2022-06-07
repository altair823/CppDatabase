//
// Created by Administrator on 2022-05-01.
//

#ifndef CPPDATABASE_F_DATETIME_H
#define CPPDATABASE_F_DATETIME_H

#include <bitset>
#include <mem_core.h>
#include <type.h>
#include <ostream>
#include <iostream>

class DateTime : public Type {
 public:

  DateTime();
  DateTime(int year, char month, char day, char hour, char min, char sec);
  ~DateTime() override = default;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  [[nodiscard]] Byte get_year() const {return f_year;}
  [[nodiscard]] Byte get_month() const {return f_month;}
  [[nodiscard]] Byte get_day() const {return f_day;}
  [[nodiscard]] Byte get_hour() const {return f_hour;}
  [[nodiscard]] Byte get_min() const {return f_min;}
  [[nodiscard]] Byte get_sec() const {return f_sec;}
  void set_year(Byte year) {
    DateTime::f_year = year;
  }
  void set_month(Byte month) {
    DateTime::f_month = month;
  }
  void set_day(Byte day) {
    DateTime::f_day = day;
  }
  void set_hour(Byte hour) {
    DateTime::f_hour = hour;
  }
  void set_min(Byte min) {
    DateTime::f_min = min;
  }
  void set_sec(Byte sec) {
    DateTime::f_sec = sec;
  }
 private:
  Byte f_year; // 8bits
  Byte f_month; // 4
  Byte f_day; // 8
  Byte f_hour; // 8
  Byte f_min; // 8
  Byte f_sec; // 8
  [[nodiscard]] bool eq(const Type &rhs) const override;
  [[nodiscard]] bool under(const Type &rhs) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_DATETIME_H
