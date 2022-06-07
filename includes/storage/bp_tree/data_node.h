//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_

#include <memory>
#include <algorithm>
#include <error.h>
#include <result.h>
#include <serializable.h>
#include <data.h>
#include <db_io.h>

template <typename Key, typename Value>
class DataNode;

template <typename Key, typename Value>
using DataNodeShared = std::shared_ptr<DataNode<Key, Value>>;

template <typename Key, typename Value>
class DataNodeFactory {
 public:
  static DataNodeShared<Key, Value> create();
};

template <typename Key, typename Value>
class DataNode : public Serializable{
 public:
  Key get_key(int index) const {return data[index]->get_key();}
  Value get_value(int index) const {return data[index]->get_value();}
  DataUnique<Key, Value> pop_data(int index) {return data[index];}
  [[nodiscard]] int get_data_count() const {return data.size();}

  void set_left_sibling(DataNodeShared<Key, Value> data_node) {left = data_node;};
  void set_right_sibling(DataNodeShared<Key, Value> data_node) {right = data_node;};
  DataNodeShared<Key, Value> get_left_sibling() const {return left;}
  DataNodeShared<Key, Value> get_right_sibling() const {return right;}

  int in_begin() {return 0;}
  int in_end() {return data.size() - 1;}

  Result<void, InsertionError> insert(int index, DataUnique<Key, Value> new_data);
  Result<void, InsertionError> push_back(DataUnique<Key, Value> new_data);
  int search(Key key) const;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  bool operator==(const DataNode &rhs) const;
  bool operator!=(const DataNode &rhs) const;

 private:
  friend class DataNodeFactory<Key, Value>;

  DBPointer left, right;
  std::vector<DataUnique<Key, Value>> data;
};
template<typename Key, typename Value>
DataNodeShared<Key, Value> DataNodeFactory<Key, Value>::create() {
  DataNodeShared<Key, Value> new_data_node(new DataNode<Key, Value>());
  return new_data_node;
}
template<typename Key, typename Value>
Result<void, InsertionError>  DataNode<Key, Value>::insert(int index, DataUnique<Key, Value> new_data) {
  if (index > data.size() || index < 0){
    return Err(InsertionError("Wrong index!"));
  }
  data.insert(data.begin() + index, std::move(new_data));
  return Ok();
}
template<typename Key, typename Value>
Result<void, InsertionError> DataNode<Key, Value>::push_back(DataUnique<Key, Value> new_data) {
  data.push_back(std::move(new_data));
  return Ok();
}
template<typename Key, typename Value>
int DataNode<Key, Value>::search(Key key) const {
  auto iterator = std::lower_bound(data.begin(), data.end(), key, [&](const DataUnique<Key, Value> &data_it, const Key &key){
    return data_it->get_key() < key;
  });
  return std::distance(data.begin(), iterator);
}
template<typename Key, typename Value>
BinaryUnique DataNode<Key, Value>::serialize() const {
  Byte node_type = static_cast<Byte>(NodeType::DataNode);
  if (data.size() >= 4096){
    throw std::range_error("Too many data(over 4096)!");
  }
  int data_count = data.size();
  std::vector<BinaryUnique> binaries;
  BinaryIndex total_size = 2;

  for (const auto &d: data){
    binaries.push_back(d->serialize());
    total_size += binaries.back()->get_length();
  }

  auto binary = BinaryFactory::create(total_size);
  binary->set_mem(0, Location_in_byte::FirstFourBit, node_type);
  binary->set_mem(0, Location_in_byte::SecondFourBit, data_count >> 8);\
  binary->set_mem(1, (Byte)data_count);

  BinaryIndex index = 2;
  for (auto &b: binaries){
    for (int i = 0; i < b->get_length(); i++){
      binary->set_mem(index, b->read_mem(i));
      index++;
    }
  }
  return binary;
}
template<typename Key, typename Value>
Result<BinaryIndex, DeserializeError> DataNode<Key, Value>::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  if (byte_to_node_type(binary.read_mem(index, Location_in_byte::FirstFourBit)) != NodeType::DataNode){
    return Err(DeserializeError("Wrong binary(this binary may not be for DataNode)!"));
  }
  std::vector<unsigned char> data_count_vec;
  data_count_vec.push_back(binary.read_mem(index, Location_in_byte::SecondFourBit));
  index++;
  data_count_vec.push_back(binary.read_mem(index));
  index++;
  int data_count = (int)byte_vec_to_num(data_count_vec);
  for (int i = 0; i < data_count; i++){
    //data.push_back(DataFactory<Key, Value>::create());
  }
}
template<typename Key, typename Value>
bool DataNode<Key, Value>::operator==(const DataNode &rhs) const {
  if (left != rhs.left || right != rhs.right || data.size() != rhs.data.size()){
    return false;
  } else {
    for (int i = 0; i < data.size(); i++){
      if (*data[i] != *rhs.data[i]){
        return false;
      }
    }
  }
  return left == rhs.left  &&
      data == rhs.data;
}
template<typename Key, typename Value>
bool DataNode<Key, Value>::operator!=(const DataNode &rhs) const {
  return !(rhs == *this);
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
