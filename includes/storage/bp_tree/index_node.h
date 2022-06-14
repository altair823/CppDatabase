//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_

#include <memory>
#include <utility>
#include <vector>
#include <ostream>
#include <serializable.h>
#include <error.h>
#include <db_io.h>
#include <data_node.h>

#define LEAF true
#define NOT_LEAF false

template <typename Key>
class IndexNode;

template <typename Key>
using IndexNodeUnique = std::unique_ptr<IndexNode<Key>>;

template <typename Key>
class IndexNodeFactory {
 public:
  static IndexNodeUnique<Key> create(const SchemaShared& schema, std::vector<Key> keys, std::vector<DBPointer> pointers);
  static IndexNodeUnique<Key> create(const SchemaShared& schema);
};

template <typename Key>
class IndexNode : public Serializable {
 public:
  [[nodiscard]] Key get_key(int index) const { return keys[index]; }
  [[nodiscard]] IndexNodeUnique<Key> get_index_child(int index) const;
  template<typename Value, typename DataType, typename DataFactory>
  [[nodiscard]] DataNodeShared<Key, Value, DataType> get_data_child(int index) const;
  [[nodiscard]] bool is_leaf() const { return is_node_leaf; }
  void set_leaf(bool is_leaf) { is_node_leaf = is_leaf; }
  [[nodiscard]] int get_key_count() const { return keys.size(); }
  [[nodiscard]] int get_pointer_count() const { return pointers.size(); }
  void insert_key(int index, const Key& key);
  void insert_pointer(int index, const DBPointer& pointer);
  void push_back_key(const Key& key);
  void push_back_pointer(const DBPointer& pointer);

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  bool operator==(const IndexNode &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const IndexNode<Key> &node) {
    os << "keys: ";
    for (auto &k : node.keys) {
      os << k << " ";
    }
    os << "\n pointers: ";
    for (auto &p : node.pointers) {
      os << p << " | ";
    }
    os << "\nis_node_leaf: " << node.is_node_leaf << "\n";
    return os;
  }

 private:
  friend class IndexNodeFactory<Key>;
  SchemaShared data_schema;
  IndexNode(SchemaShared schema) : is_node_leaf(true), data_schema(std::move(schema)) {};
  IndexNode(SchemaShared schema, std::vector<Key> keys, std::vector<DBPointer> pointers);
  std::vector<Key> keys;
  std::vector<DBPointer> pointers;
  bool is_node_leaf;
};

template<typename Key>
IndexNodeUnique<Key> IndexNodeFactory<Key>::create(const SchemaShared& schema, std::vector<Key> keys, std::vector<DBPointer> pointers) {
  IndexNodeUnique<Key> index_node(new IndexNode<Key>(schema, keys, pointers));
  return index_node;
}
template<typename Key>
IndexNodeUnique<Key> IndexNodeFactory<Key>::create(const SchemaShared& schema) {
  IndexNodeUnique<Key> index_node(new IndexNode<Key>(schema));
  return index_node;
}

template<typename Key>
IndexNode<Key>::IndexNode(SchemaShared schema, std::vector<Key> keys, std::vector<DBPointer> pointers)
: keys(std::move(keys)), pointers(std::move(pointers)), is_node_leaf(true), data_schema(std::move(schema)) {

}
template<typename Key>
BinaryUnique IndexNode<Key>::serialize() const {
  Byte node_type = static_cast<Byte>(NodeType::IndexNode);
  if (is_node_leaf == LEAF){
    node_type += (1 << 3);
  }
  if (pointers.size() >= 4096){
    throw std::range_error("Too many childs(over 4096)!");
  }
  int degree = pointers.size();
  std::vector<BinaryUnique> binaries;
  BinaryIndex total_size = 2;
  for (const auto &key: keys){
    binaries.push_back(key.serialize());
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
template<typename Key>
Result<BinaryIndex, DeserializeError> IndexNode<Key>::deserialize(const Binary &binary, BinaryIndex begin) {
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
    Key new_key;
    index = new_key.deserialize(binary, index).unwrap();
    keys.push_back(new_key);
  }
  for (int i = 0; i < degree; i++){
    DBPointer pointer;
    index = pointer.deserialize(binary, index).unwrap();
    pointers.push_back(pointer);
  }
  return Ok(index);
}
template<typename Key>
bool IndexNode<Key>::operator==(const IndexNode &rhs) const {
  return keys == rhs.keys &&
      pointers == rhs.pointers &&
      is_node_leaf == rhs.is_node_leaf;
}
template<typename Key>
IndexNodeUnique<Key> IndexNode<Key>::get_index_child(int index) const {
  if (is_node_leaf){
    throw NotFound("The Node is leaf! Does not have any pointers.");
  }
  auto binary = BinaryFactory::read(pointers[index].get_file_name(), pointers[index].get_offset(), pointers[index].get_length());
  auto new_index_node = IndexNodeFactory<Key>::create(data_schema);
  new_index_node->deserialize(*binary, 0).unwrap();
  return new_index_node;
}
template<typename Key>
template<typename Value, typename DataType, typename DataFactory>
DataNodeShared<Key, Value, DataType> IndexNode<Key>::get_data_child(int index) const {
  if (!is_node_leaf){
    throw NotFound("The Node is not leaf! Does not have any data nodes.");
  }
  auto binary = BinaryFactory::read(pointers[index].get_file_name(), pointers[index].get_offset(), pointers[index].get_length());
  auto new_data_node = DataNodeFactory<Key, Value, DataType>::create(std::make_unique<DataFactory>(data_schema));
  new_data_node->deserialize(*binary, 0).unwrap();
  return new_data_node;
}
template<typename Key>
void IndexNode<Key>::insert_key(int index, const Key& key) {
  keys.insert(keys.begin() + index, keys);
}
template<typename Key>
void IndexNode<Key>::insert_pointer(int index, const DBPointer& pointer) {
  pointers.insert(pointers.begin() + index, pointer);
}
template<typename Key>
void IndexNode<Key>::push_back_key(const Key &key) {
  keys.push_back(key);
}
template<typename Key>
void IndexNode<Key>::push_back_pointer(const DBPointer &pointer) {
  pointers.push_back(pointer);
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
