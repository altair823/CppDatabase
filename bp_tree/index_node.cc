//
// Created by 김태현 on 2022/06/15.
//
#include <index_node.h>

#include <utility>


IndexNodeUnique IndexNodeFactory::create(const SchemaShared& schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers) {
  IndexNodeUnique index_node(new IndexNode(schema, std::move(key_field_name), std::move(keys), std::move(pointers)));
  return index_node;
}
IndexNodeUnique IndexNodeFactory::create(const SchemaShared& schema, std::string key_field_name) {
  IndexNodeUnique index_node(new IndexNode(schema, std::move(key_field_name)));
  return index_node;
}

IndexNode::IndexNode(SchemaShared schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers)
: keys(std::move(keys)), pointers(std::move(pointers)), is_node_leaf(true), schema(std::move(schema)), key_field_name(std::move(key_field_name)) {

}
BinaryUnique IndexNode::serialize() const {
  Byte node_type = static_cast<Byte>(NodeType::IndexNode);
  if (is_node_leaf == LEAF){
    node_type += (1 << 3);
  }
  if (pointers.size() >= 4096){
    throw std::range_error("Too many childs(over 4096)!");
  }
  int degree = (int)pointers.size();
  std::vector<BinaryUnique> binaries;
  BinaryIndex total_size = 2;
  for (const auto &key: keys){
    binaries.push_back(key->serialize());
    total_size += binaries.back()->get_length();
  }
  for (const auto &pointer: pointers){
    binaries.push_back(pointer.serialize());
    total_size += binaries.back()->get_length();
  }

  auto binary = BinaryFactory::create(total_size);
  binary->set_mem(0, Location_in_byte::FirstFourBit, node_type);
  binary->set_mem(0, Location_in_byte::SecondFourBit, degree >> 8);\
  binary->set_mem(1, (Byte)degree);

  BinaryIndex index = 2;
  for (auto &b: binaries){
    for (int i = 0; i < b->get_length(); i++){
      binary->set_mem(index, b->read_mem(i));
      index++;
    }
  }
  return binary;
}
Result<BinaryIndex, DeserializeError> IndexNode::deserialize(const Binary &binary, BinaryIndex begin) {
  auto node_type = binary.read_mem(0, Location_in_byte::FirstFourBit);
  if (node_type & 8){
    is_node_leaf = LEAF;
    node_type -= 8;
  } else {
    is_node_leaf = NOT_LEAF;
  }
  if (node_type != static_cast<Byte>(NodeType::IndexNode)){
    return Err(DeserializeError("The binary data is not for index node!"));
  }
  int degree = (binary.read_mem(0, Location_in_byte::SecondFourBit) << 4) + binary.read_mem(1);
  BinaryIndex index = 2;
  for (int i = 0; i < degree - 1; i++){
    Key new_key = FieldFactory::create(schema->get_field(key_field_name).unwrap());
    index = new_key->deserialize(binary, index).unwrap();
    keys.push_back(new_key);
  }
  for (int i = 0; i < degree; i++){
    DBPointer pointer;
    index = pointer.deserialize(binary, index).unwrap();
    pointers.push_back(pointer);
  }
  return Ok(index);
}
bool IndexNode::operator==(const IndexNode &rhs) const {
  if (is_node_leaf != rhs.is_node_leaf || keys.size() != rhs.keys.size() || pointers != rhs.pointers) {
    return false;
  }
  if (!std::equal(keys.begin(), keys.end(), rhs.keys.begin(), field_comparator)){
    return false;
  }
  return true;
}
IndexNodeUnique IndexNode::get_index_child(int index) const {
  if (is_node_leaf){
    throw NotFound("The Node is leaf! Does not have any pointers.");
  }
  auto binary = BinaryFactory::read(pointers[index].get_file_name(), pointers[index].get_offset(), pointers[index].get_length());
  auto new_index_node = IndexNodeFactory::create(schema, key_field_name);
  new_index_node->deserialize(*binary, 0).unwrap();
  return new_index_node;
}
DataNodeShared IndexNode::get_data_child(int index) const {
  if (!is_node_leaf){
    throw NotFound("The Node is not leaf! Does not have any data nodes.");
  }
  auto binary = BinaryFactory::read(pointers[index].get_file_name(), pointers[index].get_offset(), pointers[index].get_length());
//  auto new_data_node = DataNodeFactory::create(schema, key_field_name, );
//  new_data_node->deserialize(*binary, 0).unwrap();
//  return new_data_node;
}
void IndexNode::insert_key(int index, const Key& key) {
  keys.insert(keys.begin() + index, key);
}
void IndexNode::insert_pointer(int index, const DBPointer& pointer) {
  pointers.insert(pointers.begin() + index, pointer);
}
void IndexNode::push_back_key(const Key &key) {
  keys.push_back(key);
}
void IndexNode::push_back_pointer(const DBPointer &pointer) {
  pointers.push_back(pointer);
}
int IndexNode::search_key(const Key& key) {
  int index = 0;
  for (auto &k: keys){
    if (*k > *key){
      break;
    }
    index++;
  }
  return index;
}
void IndexNode::erase_key(int index) {

}
void IndexNode::erase_pointer(int index) {

}
