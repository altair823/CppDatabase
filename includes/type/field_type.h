//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include <ostream>

/**
 * Type of fields in DB schema.
 * The current number of types is up to 16.
 */
enum class Type {
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
  BIT,
  BLOB,
};

unsigned char type_to_4_bits(Type type);
Type bits_to_type(unsigned char bits);
std::ostream &operator<<(std::ostream &os, const Type &type);

class FieldType {
 public:
  FieldType();
  explicit FieldType(Type field_type);
  virtual std::unique_ptr<unsigned char[]> serialize() = 0;
  virtual void deserialize(std::unique_ptr<unsigned char[]> &) = 0;
  friend std::ostream &operator<<(std::ostream &os, const FieldType &field_type);

  Type field_type;
 private:
  virtual std::ostream& out(std::ostream&) const = 0;
};

#endif //CPPDATABASE_FIELD_INTERFACE_H
