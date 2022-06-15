//
// Created by 김태현 on 2022/06/16.
//

#include <data_node.h>


DataNodeShared DataNodeFactory::create(std::unique_ptr<DataFactory> data_factory) {
  DataNodeShared new_data_node(new DataNode(std::move(data_factory)));
  return new_data_node;
}
void DataNode::insert(int index, DataUnique new_data) {
  data.insert(data.begin() + index, std::move(new_data));
}
void DataNode::push_back(DataUnique new_data) {
  data.push_back(std::move(new_data));
}
int DataNode::search(Key key) const {
  auto iterator = std::lower_bound(data.begin(), data.end(), key, [&](const DataUnique &data_it, const Key &key){
    return *(data_it->get_key()) < *key;
  });
  return std::distance(data.begin(), iterator);
}
BinaryUnique DataNode::serialize() const {
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
Result<BinaryIndex, DeserializeError> DataNode::deserialize(const Binary &binary, BinaryIndex begin) {
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
bool DataNode::operator==(const DataNode &rhs) const {
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
bool DataNode::operator!=(const DataNode &rhs) const {
  return !(rhs == *this);
}