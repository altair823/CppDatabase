//
// Created by 김태현 on 2022/06/08.
//

#include <bp_tree_data.h>

BPTreeData::BPTreeData(const std::string& key_name, const Record& record): value(std::make_shared<Record>(record)) {
  key = std::make_shared<Field>(record.get_field(key_name).unwrap());
}
BinaryUnique BPTreeData::serialize() const {

}
Result<BinaryIndex, DeserializeError> BPTreeData::deserialize(const Binary &binary, BinaryIndex begin) {

}
std::shared_ptr<Field> BPTreeData::get_key() const {
  return key;
}
std::shared_ptr<Record> BPTreeData::get_value() const {
  return value;
}
bool BPTreeData::eq(const Data<Field, Record> &rhs) const {
  return *key == *rhs.get_key();
}
std::ostream &BPTreeData::out(std::ostream &ostream) const {
  ostream << "<BPTreeData>\nkey: " << key << "\nvalue: " << value;
  return ostream;
}
