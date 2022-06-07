//
// Created by 김태현 on 2022/05/02.
//

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
 * The current number of types is up to 16.
 */
enum class TypeKind {
  NONE = 0,
  STRING, // 1
  DATETIME, // 2
  DATE, // 3
  TIME, // 4
  CHAR, // 5
  SHORT, // 6
  INT, // 7
  BIGINT, // 8
  FLOAT, // 9
  DOUBLE, // 10
  BIT, // 11
  BLOB, // 12
};

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

Byte type_to_4_byte(TypeKind type);
TypeKind byte_to_type(Byte bits);
std::ostream &operator<<(std::ostream &os, const TypeKind &type);
typedef std::shared_ptr<Type> TypeShared;


#endif //CPPDATABASE_FIELD_INTERFACE_H
