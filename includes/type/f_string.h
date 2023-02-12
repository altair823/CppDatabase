/**
 * @file f_string.h
 * @brief String type class
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_F_STRING_H
#define CPPDATABASE_F_STRING_H

#include <type.h>
#include <mem_core.h>
#include <cmath>
#include <ostream>
#include <iostream>
#include <utility>

/**
 * @brief String type.
 * @details This class is used to define the type of string fields in DB schema.
 */
class String : public Type {
 public:

  /**
   * @brief Default constructor.
   * @details This constructor is used to create a string type with no length.
   */
  String();

  /**
   * @brief Constructor.
   * @details This constructor is used to create a string type with std::string.
   * @param str String to be copied.
   */
  explicit String(std::string str);
  ~String() override = default;
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  /**
   * @brief Get string value.
   * @return std::string
   */
  [[nodiscard]] std::string get_string() const {return str;}

  /**
   * @brief Set string value.
   * @param new_str New string value.
   */
  void set_string(std::string new_str) {str = std::move(new_str);}

 private:
  std::string str;
  [[nodiscard]] bool eq(const Type &field_type) const override;
  [[nodiscard]] bool under(const Type &rhs) const override;
  std::ostream& out(std::ostream& os) const override;
};

#endif //CPPDATABASE_F_STRING_H
