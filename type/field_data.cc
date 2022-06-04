//
// Created by 김태현 on 2022/05/02.
//

#include <field_data.h>

FieldData::FieldData(Type field_type) {
  this->field_type = field_type;
}

FieldData::FieldData(): field_type(Type::NONE) { }
std::ostream &operator<<(std::ostream &os, const FieldData &field_type) {
  return field_type.out(os);
}
bool FieldData::operator==(const FieldData &rhs) const {
  return typeid(*this) == typeid(rhs) && eq(rhs);
}
FieldData::FieldData(const FieldData &field_data) {
  this->field_type = field_data.field_type;
}