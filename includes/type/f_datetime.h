/**
 * @file f_datetime.h
 * @brief Datetime type class
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_F_DATETIME_H
#define CPPDATABASE_F_DATETIME_H

#include <bitset>
#include <mem_core.h>
#include <type.h>
#include <ostream>
#include <iostream>

/**
 * @brief Date type.
 * @details This class is used to define the type of date fields in DB schema.
 * @see Type
 */
class DateTime : public Type {
 public:

  /**
   * @brief Default constructor.
   * @details This constructor is used to create a date type with 0 value.
   * All datetime values are set to 0.
   */
  DateTime();

  /**
   * @brief Constructor.
   * @details This constructor is used to create a date type with date value.
   * @param year Year value.
   * @param month Month value.
   * @param day Day value.
   * @param hour Hour value.
   * @param minute Minute value.
   * @param second Second value.
   */
  DateTime(int year, char month, char day, char hour, char min, char sec);
  ~DateTime() override = default;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  /**
   * @brief Get year value.
   * @return Byte
   */
  [[nodiscard]] Byte get_year() const {return f_year;}

  /**
   * @brief Get month value.
   * @return Byte
   */
  [[nodiscard]] Byte get_month() const {return f_month;}

  /**
   * @brief Get day value.
   * @return Byte
   */
  [[nodiscard]] Byte get_day() const {return f_day;}

  /**
   * @brief Get hour value.
   * @return Byte
   */
  [[nodiscard]] Byte get_hour() const {return f_hour;}

/**
   * @brief Get minute value.
   * @return Byte
   */
  [[nodiscard]] Byte get_min() const {return f_min;}

  /**
   * @brief Get second value.
   * @return Byte
   */
  [[nodiscard]] Byte get_sec() const {return f_sec;}

  /**
   * @brief Set year value.
   * @param year New year value.
   */
  void set_year(Byte year) {
    DateTime::f_year = year;
  }

  /**
   * @brief Set month value.
   * @param month New month value.
   */
  void set_month(Byte month) {
    DateTime::f_month = month;
  }

  /**
   * @brief Set day value.
   * @param day New day value.
   */
  void set_day(Byte day) {
    DateTime::f_day = day;
  }

  /**
   * @brief Set hour value.
   * @param hour New hour value.
   */
  void set_hour(Byte hour) {
    DateTime::f_hour = hour;
  }

  /**
   * @brief Set minute value.
   * @param min New minute value.
   */
  void set_min(Byte min) {
    DateTime::f_min = min;
  }

  /**
   * @brief Set second value.
   * @param sec New second value.
   */
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
