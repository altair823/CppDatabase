//
// Created by 김태현 on 2022/05/02.
//

#include <type.h>

Type::Type(TypeKind field_type) {
  this->field_type = field_type;
}

Type::Type(): field_type(TypeKind::NONE) { }
std::ostream &operator<<(std::ostream &os, const Type &field_type) {
  return field_type.out(os);
}
bool Type::operator==(const Type &rhs) const {
  return typeid(*this) == typeid(rhs) && eq(rhs);
}
Type::Type(const Type &field_data) {
  this->field_type = field_data.field_type;
}
bool Type::operator<(const Type &rhs) const {
  return under(rhs);
}
bool Type::operator>(const Type &rhs) const {
  return rhs.under(*this);
}
bool Type::operator<=(const Type &rhs) const {
  return !(rhs < *this);
}
bool Type::operator>=(const Type &rhs) const {
  return !(*this < rhs);
}
Byte type_to_4_byte(TypeKind type) {
  switch (type) {
    case TypeKind::NONE:return 0;
    case TypeKind::STRING:return 1;
    case TypeKind::DATETIME:return 2;
    case TypeKind::DATE:return 3;
    case TypeKind::TIME:return 4;
    case TypeKind::CHAR:return 5;
    case TypeKind::SHORT:return 6;
    case TypeKind::INT:return 7;
    case TypeKind::BIGINT:return 8;
    case TypeKind::FLOAT:return 9;
    case TypeKind::DOUBLE:return 10;
    case TypeKind::BIT:return 11;
    case TypeKind::BLOB:return 12;
  }
  return 0;
}
TypeKind byte_to_type(Byte bits) {
  return static_cast<TypeKind>(bits);
}
std::ostream &operator<<(std::ostream &os, const TypeKind &type) {
  switch (type) {
    case TypeKind::NONE:
      os << "NONE";
      break;
    case TypeKind::STRING:
      os << "STRING";
      break;
    case TypeKind::DATETIME:
      os << "DATETIME";
      break;
    case TypeKind::INT:
      os << "INT";
      break;
    default:os << "UNKNOWN";
      break;
  }
  return os;
}