//
// Created by 김태현 on 2022/06/05.
//

#include <field.h>

#include <utility>

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
bool Field::operator<(const Field &rhs) const {
  if (name < rhs.name) {
    return true;
  } else {
    return false;
  }
}
bool Field::operator>(const Field &rhs) const {
  return rhs < *this;
}
bool Field::operator<=(const Field &rhs) const {
  return !(rhs < *this);
}
bool Field::operator>=(const Field &rhs) const {
  return !(*this < rhs);
}

Result<TypeShared, CannotConvert> type_to_field(TypeKind type){
  switch (type) {
    case TypeKind::NONE:return Err(CannotConvert("None type error!"));
    case TypeKind::STRING:return Ok(TypeShared(new String()));
    case TypeKind::DATETIME:return Ok(TypeShared(new DateTime()));
    case TypeKind::DATE:break;
    case TypeKind::TIME:break;
    case TypeKind::CHAR:break;
    case TypeKind::SHORT:break;
    case TypeKind::INT:return Ok(TypeShared(new Int));
    case TypeKind::BIGINT:break;
    case TypeKind::FLOAT:break;
    case TypeKind::DOUBLE:break;
    case TypeKind::BIT:break;
    case TypeKind::BLOB:break;
    default: return Err(CannotConvert("Wrong type error!"));
  }
  return Err(CannotConvert("Wrong type error!"));
}
FieldShared FieldFactory::create() {
  std::shared_ptr<Field> new_field(new Field());
  return new_field;
}
FieldShared FieldFactory::create(const FieldSchema &field_schema) {
  std::shared_ptr<Field> new_field(new Field(field_schema));
  return new_field;
}
FieldShared FieldFactory::create(std::string name) {
  std::shared_ptr<Field> new_field(new Field(std::move(name)));
  return new_field;
}
FieldShared FieldFactory::create(std::string name, TypeShared data) {
  std::shared_ptr<Field> new_field(new Field(std::move(name), std::move(data)));
  return new_field;
}

bool field_shared_comparator(const FieldShared& field1, const FieldShared& field2){
  return *field1 > *field2;
}