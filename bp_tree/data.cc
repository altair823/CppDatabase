//
// Created by 김태현 on 2022/06/08.
//

#include <data.h>

#include <utility>

Data::Data(const Record& record): value(std::make_shared<Record>(record)) {
  key = std::make_shared<Field>(record.get_pk_field().unwrap());
}
BinaryUnique Data::serialize() const {
  auto binary = value->serialize();
  // Don't serialize and deserialize the key because it is just a pointer of PK in record.
  return binary;
}
Result<BinaryIndex, DeserializeError> Data::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  index = value->deserialize(binary, index).unwrap();
  return Ok(index);
}
std::shared_ptr<Field> Data::get_key() const {
  return key;
}
std::shared_ptr<Record> Data::get_value() const {
  return value;
}
bool Data::eq(const Data &rhs) const {
  return *key == *rhs.get_key() && *value == *rhs.get_value();
}
std::ostream &operator<<(std::ostream &os, const Data &data) {
  os << "BPTreeData{\nkey: " << *data.key << "\nvalue: " << *data.value << "\n}";
  return os;
}
bool Data::operator==(const Data &rhs) const {
  return *key == *rhs.get_key() && *value == *rhs.get_value();
}
bool Data::operator!=(const Data &rhs) const {
  return !(rhs == *this);
}
DataFactory::DataFactory(SchemaShared schema_shared) : schema_shared(std::move(schema_shared)) {

}
std::unique_ptr<Data> DataFactory::create_data() {
  auto pk_name = schema_shared->get_pk().get_name();
  auto record = Record(*schema_shared);
  return std::make_unique<Data>(record);
}
