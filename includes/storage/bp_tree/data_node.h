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
#include <ostream>

template <typename Key, typename Value, typename Data>
class DataNode;

template <typename Key, typename Value, typename Data>
using DataNodeShared = std::shared_ptr<DataNode<Key, Value, Data>>;

template <typename Key, typename Value, typename Data>
class DataNodeFactory {
 public:
  static DataNodeShared<Key, Value, Data> create(std::unique_ptr<DataFactory<Key, Value, Data>> data_factory);
};

template <typename Key, typename Value, typename Data>
class DataNode : public Serializable{
 public:
using DataUnique = std::unique_ptr<Data>;
using KeyShared = std::shared_ptr<Key>;
using ValueShared = std::shared_ptr<Value>;
  KeyShared get_key(int index) const {return data[index]->get_key();}
  ValueShared get_value(int index) const {return data[index]->get_value();}
  Data pop_data(int index) {return data[index];}
  [[nodiscard]] int get_data_count() const {return data.size();}

  void set_left_sibling(DataNodeShared<Key, Value, Data> data_node) {left = data_node;};
  void set_right_sibling(DataNodeShared<Key, Value, Data> data_node) {right = data_node;};
  DataNodeShared<Key, Value, Data> get_left_sibling() const {return left;}
  DataNodeShared<Key, Value, Data> get_right_sibling() const {return right;}

  int in_begin() {return 0;}
  int in_end() {return data.size() - 1;}

  void insert(int index, DataUnique new_data);
  void push_back(DataUnique new_data);
  int search(Key key) const;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  bool operator==(const DataNode &rhs) const;
  bool operator!=(const DataNode &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const DataNode &node) {
    os << "DataNode{\nleft: " << node.left << ",\nright: " << node.right << ",\ndata: ";
    for (auto& d: node.data){
      os << "\n" << *d;
    }
    return os;
  }
 private:
  friend class DataNodeFactory<Key, Value, Data>;
  explicit DataNode(std::unique_ptr<DataFactory<Key, Value, Data>> data_factory): data_factory(std::move(data_factory)) {}

  DBPointer left, right;
  std::vector<DataUnique> data;

  std::unique_ptr<DataFactory<Key, Value, Data>> data_factory;
};
template<typename Key, typename Value, typename Data>
DataNodeShared<Key, Value, Data> DataNodeFactory<Key, Value, Data>::create(std::unique_ptr<DataFactory<Key, Value, Data>> data_factory) {
  DataNodeShared<Key, Value, Data> new_data_node(new DataNode<Key, Value, Data>(std::move(data_factory)));
  return new_data_node;
}
template<typename Key, typename Value, typename Data>
void  DataNode<Key, Value, Data>::insert(int index, DataUnique new_data) {
  data.insert(data.begin() + index, std::move(new_data));
}
template<typename Key, typename Value, typename Data>
void DataNode<Key, Value, Data>::push_back(DataUnique new_data) {
  data.push_back(std::move(new_data));
}
template<typename Key, typename Value, typename Data>
int DataNode<Key, Value, Data>::search(Key key) const {
  auto iterator = std::lower_bound(data.begin(), data.end(), key, [&](const DataUnique &data_it, const Key &key){
    return *(data_it->get_key()) < key;
  });
  return std::distance(data.begin(), iterator);
}
template<typename Key, typename Value, typename Data>
BinaryUnique DataNode<Key, Value, Data>::serialize() const {
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
template<typename Key, typename Value, typename Data>
Result<BinaryIndex, DeserializeError> DataNode<Key, Value, Data>::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  if (byte_to_node_type(binary.read_mem(index, Location_in_byte::FirstFourBit)) != NodeType::DataNode){
    return Err(DeserializeError("Wrong binary(this binary may not be for DataNode)!"));
  }
  int data_count = binary.read_mem(index, Location_in_byte::SecondFourBit) << 8;
  index++;
  data_count += binary.read_mem(index);
  index++;
  for (int i = 0; i < data_count; i++){
    auto new_data = data_factory->create_data();
    index = new_data->deserialize(binary, index).unwrap();
    data.push_back(std::move(new_data));
  }
  return Ok(index);
}
template<typename Key, typename Value, typename Data>
bool DataNode<Key, Value, Data>::operator==(const DataNode<Key, Value, Data> &rhs) const {
  if (left != rhs.left || right != rhs.right || data.size() != rhs.data.size()){
    return false;
  } else {
    for (int i = 0; i < data.size(); i++){
      if (*data[i] != *rhs.data[i]){
        return false;
      }
    }
  }
  return true;
}
template<typename Key, typename Value, typename Data>
bool DataNode<Key, Value, Data>::operator!=(const DataNode &rhs) const {
  return !(rhs == *this);
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
