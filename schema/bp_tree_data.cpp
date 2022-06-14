//
// Created by 김태현 on 2022/06/08.
//

#include <bp_tree_data.h>

#include <utility>

DBData::DBData(const Record& record): value(std::make_shared<Record>(record)) {
  key = std::make_shared<Field>(record.get_pk_field().unwrap());
}
BinaryUnique DBData::serialize() const {
  auto binary = value->serialize();
  // Don't serialize and deserialize the key because it is just a pointer of PK in record.
  return binary;
}
Result<BinaryIndex, DeserializeError> DBData::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  index = value->deserialize(binary, index).unwrap();
  return Ok(index);
}
std::shared_ptr<Field> DBData::get_key() const {
  return key;
}
std::shared_ptr<Record> DBData::get_value() const {
  return value;
}
bool DBData::eq(const Data<Field, Record> &rhs) const {
  return *key == *rhs.get_key() && *value == *rhs.get_value();
}
std::ostream &DBData::out(std::ostream &ostream) const {
  ostream << "BPTreeData{\nkey: " << *key << "\nvalue: " << *value << "\n}";
  return ostream;
}
DBDataFactory::DBDataFactory(SchemaShared schema_shared) : schema_shared(std::move(schema_shared)) {

}
std::unique_ptr<DBData> DBDataFactory::create_data() {
  auto pk_name = schema_shared->get_pk().get_name();
  auto record = Record(*schema_shared);
  return std::make_unique<DBData>(record);
}
