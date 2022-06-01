//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_

#include <memory>
#include <vector>
#include <ostream>
#include <serializable.h>
#include <error.h>

#include "db_io.h"

#define LEAF true
#define NOT_LEAF false

template <typename Key>
class RawIndexNode;

template <typename Key>
using IndexNodeUnique = std::unique_ptr<RawIndexNode<Key>>;

template <typename Key>
class IndexNode {
 public:
  static IndexNodeUnique<Key> create(std::vector<Key> keys, std::vector<DBPointer> pointers);
  static IndexNodeUnique<Key> create();
};

template <typename Key>
class RawIndexNode : public Serializable {
 public:
  [[nodiscard]] Key get_key(int index) const { return keys[index]; }
  [[nodiscard]] IndexNodeUnique<Key> get_pointer(int index) const;
  [[nodiscard]] bool is_leaf() const { return is_node_leaf; }
  void set_leaf(bool is_leaf) { is_node_leaf = is_leaf; }
  [[nodiscard]] int get_key_count() const { return keys.size(); }
  [[nodiscard]] int get_pointer_count() const { return pointers.size(); }

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BINARY_INDEX, DeserializeError> deserialize(const Binary &binary, BINARY_INDEX begin) override;
  bool operator==(const RawIndexNode &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const RawIndexNode<Key> &node) {
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
  friend class IndexNode<Key>;
  RawIndexNode() : is_node_leaf(true) {};
  RawIndexNode(std::vector<Key> keys, std::vector<DBPointer> pointers);
  std::vector<Key> keys;
  std::vector<DBPointer> pointers;
  bool is_node_leaf;
};

template<typename Key>
IndexNodeUnique<Key> IndexNode<Key>::create(std::vector<Key> keys, std::vector<DBPointer> pointers) {
  IndexNodeUnique<Key> index_node(new RawIndexNode<Key>(keys, pointers));
  return index_node;
}
template<typename Key>
IndexNodeUnique<Key> IndexNode<Key>::create() {
  IndexNodeUnique<Key> index_node(new RawIndexNode<Key>());
  return index_node;
}

template<typename Key>
RawIndexNode<Key>::RawIndexNode(std::vector<Key> keys, std::vector<DBPointer> pointers)
: keys(std::move(keys)), pointers(std::move(pointers)), is_node_leaf(true) {

}
template<typename Key>
BinaryUnique RawIndexNode<Key>::serialize() const {
  char node_type = static_cast<char>(NodeType::IndexNode);
  if (pointers.size() >= 4096){
    throw std::range_error("Too many childs(over 4096)!");
  }
  int degree = pointers.size();
  std::vector<BinaryUnique> binaries;
  BINARY_INDEX total_size = 2;
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
  binary->set_mem(1, (char)degree);

  BINARY_INDEX index = 2;
  for (auto &b: binaries){
    for (int i = 0; i < b->get_length(); i++){
      binary->set_mem(index, b->read_mem(i));
      index++;
    }
  }
  return binary;
}
template<typename Key>
Result<BINARY_INDEX, DeserializeError> RawIndexNode<Key>::deserialize(const Binary &binary, BINARY_INDEX begin) {
  if (binary.read_mem(0, Location_in_byte::FirstFourBit) != static_cast<unsigned char>(NodeType::IndexNode)){
    return Err(DeserializeError("The binary data is not for index node!", begin));
  }
  int degree = (binary.read_mem(0, Location_in_byte::SecondFourBit) << 4) + binary.read_mem(1);
  BINARY_INDEX index = 2;
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
bool RawIndexNode<Key>::operator==(const RawIndexNode &rhs) const {
  return keys == rhs.keys &&
      pointers == rhs.pointers &&
      is_node_leaf == rhs.is_node_leaf;
}
template<typename Key>
IndexNodeUnique<Key> RawIndexNode<Key>::get_pointer(int index) const {
  // To find DBPointer's file and open it,
  // and read the another index node in a certain offset.

  return IndexNodeUnique<Key>();
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
