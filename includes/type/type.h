/**
 * @file type.h
 * @brief Type definitions
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include <ostream>
#include <utility>
#include <result.h>
#include "../error.h"
#include <mem_core.h>
#include "serializable.h"

/**
 * Type of fields in DB schema.
 */
enum class TypeKind {
  NONE = 0, // Not defined
  STRING, // String
  DATETIME, // Datetime
  DATE, // Date
  TIME, // Time
  CHAR, // Character
  SHORT, // Short
  INT, // Integer
  BIGINT, // Long
  FLOAT, // Float
  DOUBLE, // Double
  BIT, // Bit
  BLOB, // Blob
};

/**
 * @brief Field type interface.
 * @details This interface is used to define the type of fields in DB schema.
 * Must overload equals, under, and out when inherit this class.
 * @see TypeKind
 * @see Field
 * @see Schema
 * @see Table
 */
class [[nodiscard]] Type : public Serializable{
 public:
  Type();
  Type(const Type& field_data);
  virtual ~Type() = default;
  explicit Type(TypeKind field_type);

  bool operator==(const Type &rhs) const;
  bool operator<(const Type &rhs) const;
  bool operator>(const Type &rhs) const;
  bool operator<=(const Type &rhs) const;
  bool operator>=(const Type &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const Type &field_type);

  TypeKind field_type;
 private:
  [[nodiscard]] virtual bool eq(const Type &rhs) const = 0;
  [[nodiscard]] virtual bool under(const Type &rhs) const = 0;
  virtual std::ostream& out(std::ostream&) const = 0;
};

/**
 * @brief Convert TypeKind to a byte.
 * @param type TypeKind to convert.
 * @return Byte representation of TypeKind.
 */
Byte type_to_byte(TypeKind type);

/**
 * @brief Convert a byte to TypeKind.
 * @param byte Byte to convert.
 * @return TypeKind representation of byte.
 */
TypeKind byte_to_type(Byte byte);

std::ostream &operator<<(std::ostream &os, const TypeKind &type);
typedef std::shared_ptr<Type> TypeShared;


#endif //CPPDATABASE_FIELD_INTERFACE_H
