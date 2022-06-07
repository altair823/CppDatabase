//
// Created by 김태현 on 2022/06/05.
//

#include <field.h>

BinaryUnique Field::serialize() const {
  return data->serialize();
}
Result<BinaryIndex, DeserializeError> Field::deserialize(const Binary &binary, BinaryIndex begin) {
  return data->deserialize(binary, begin);
}
std::ostream &operator<<(std::ostream &os, const Field &field) {
  os << "Field{name: " << field.name <<", data: " << *field.data << "}";
  return os;
}
bool Field::operator==(const Field &rhs) const {
  return name == rhs.name && *data == *rhs.data;
}
bool Field::operator!=(const Field &rhs) const {
  return !(rhs == *this);
}
