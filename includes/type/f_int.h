/**
 * @file f_int.h
 * @brief Integer type class
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_INCLUDES_TYPE_F_INT_H_
#define CPPDATABASE_INCLUDES_TYPE_F_INT_H_

#include <type.h>
#include <mem_core.h>

/**
 * @brief 32 bits integer type.
 * @details This class is used to define the type of integer fields in DB schema.
 * @see Type
 */
class Int : public Type {
 public:
  /**
   * @brief Default constructor.
   * @details This constructor is used to create an integer type with 0 value.
   */
  Int(): value(0), Type(TypeKind::INT) {}

  /**
   * @brief Constructor.
   * @details This constructor is used to create an integer type with int value.
   * @param i Integer value.
   */
  explicit Int(int i): value(i), Type(TypeKind::INT) {}
  ~Int() override = default;
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  /**
   * @brief Get integer value.
   * @return int32 value.
   */
  [[nodiscard]] int32 get_int() const {return value;}

  /**
   * @brief Get length of binary.
   * @return Length of binary.
   */
  [[nodiscard]] static int get_fixed_length() {return 32;}
 private:
  [[nodiscard]] bool eq(const Type &rhs) const override;
  [[nodiscard]] bool under(const Type &rhs) const override;
  std::ostream &out(std::ostream &ostream) const override;

  int32 value;
};

#endif //CPPDATABASE_INCLUDES_TYPE_F_INT_H_
