//
// Created by 김태현 on 2022/05/02.
//

#include <field_data.h>

FieldData::FieldData(Type field_type) {
  this->field_type = field_type;
}

FieldData::FieldData() {
  this->field_type = Type::NONE;
}
std::ostream &operator<<(std::ostream &os, const FieldData &field_type) {
  return field_type.out(os);
}
bool FieldData::operator==(const FieldData &rhs) const {
  return typeid(*this) == typeid(rhs) && eq(rhs);
}

unsigned char type_to_4_bits(Type type) {
  switch (type) {
    case Type::NONE:return 0;
    case Type::STRING:return 1;
    case Type::DATETIME:return 2;
    case Type::DATE:return 3;
    case Type::TIME:return 4;
    case Type::CHAR:return 5;
    case Type::SHORT:return 6;
    case Type::INT:return 7;
    case Type::BIGINT:return 8;
    case Type::FLOAT:return 9;
    case Type::DOUBLE:return 10;
    case Type::BIT:return 11;
    case Type::BLOB:return 12;
  }
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