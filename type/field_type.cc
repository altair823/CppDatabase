//
// Created by 김태현 on 2022/05/02.
//

#include <field_type.h>

FieldType::FieldType(Type field_type) {
  this->field_type = field_type;
}

FieldType::FieldType() {
  this->field_type = Type::NONE;
}
std::ostream &operator<<(std::ostream &os, const FieldType &field_type) {
  return field_type.out(os);
}

unsigned char type_to_4_bits(Type type) {
  return static_cast<unsigned char>(type);
}

Type bits_to_type(unsigned char bits) {
  return static_cast<Type>(bits);
}

std::ostream &operator<<(std::ostream &os, const Type &type) {
  switch (type) {
    case Type::NONE:
      os << "NONE";
      break;
    case Type::STRING:
      os << "STRING";
      break;
    case Type::DATETIME:
      os << "DATETIME";
      break;
    default:os << "UNKNOWN";
      break;
  }
  return os;
}