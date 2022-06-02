//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_TYPE_TYPES_H_
#define CPPDATABASE_INCLUDES_TYPE_TYPES_H_

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
  BIT, // 11
  BLOB, // 12
};




Byte type_to_4_byte(Type type);
Type byte_to_type(Byte bits);
std::ostream &operator<<(std::ostream &os, const Type &type);

#endif //CPPDATABASE_INCLUDES_TYPE_TYPES_H_
