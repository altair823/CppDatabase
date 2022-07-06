//
// Created by 김태현 on 2022/06/16.
//

#include <data_node.h>

#include <utility>


DataNodeShared DataNodeFactory::create(const SchemaShared& schema, std::string key_field_name, std::filesystem::path record_file) {
  DataNodeShared new_data_node(new DataNode(schema, std::move(key_field_name), std::move(record_file)));
  return new_data_node;
}
DataNode::DataNode(SchemaShared schema, std::string key_field_name, std::filesystem::path record_file)
: schema(std::move(schema)), key_field_name(std::move(key_field_name)), record_file(DBFile(std::move(record_file), TWO_GB)) {

}
Value DataNode::get_value(int index) const {
  auto value_binary = BinaryFactory::read(data[index].get_file_name(), data[index].get_offset(), data[index].get_length());
  auto value = RecordFactory::create(schema);
  value->Serializable::deserialize(*value_binary).unwrap();
  return value;
}
void DataNode::insert(int index, Value new_data) {
  auto record_filename = record_file.get_free_file();
  auto metadata = new_data->serialize()->save(record_filename).unwrap();
  data.insert(data.begin() + index, DBPointer(record_filename, metadata.offset, metadata.length));
}
void DataNode::push_back(Value new_data) {
  auto record_filename = record_file.get_free_file();
  auto metadata = new_data->serialize()->save(record_filename).unwrap();
  data.emplace_back(record_filename, metadata.offset, metadata.length);
}
int DataNode::search(const Key& key) const {
  auto iterator = std::lower_bound(keys.begin(), keys.end(), key, [&](const Key &key_it, const Key &key){
    return key_it < key;
  });
  return (int)std::distance(keys.begin(), iterator);
}
void DataNode::remove(int index) {
  data.erase(data.begin() + index);
}
BinaryUnique DataNode::serialize() const {
  Byte node_type = static_cast<Byte>(NodeType::DataNode);
  if (data.size() >= 4096){
    throw std::range_error("Too many data(over 4096)!");
  }
  int data_count = (int)data.size();
  std::vector<BinaryUnique> binaries;
  BinaryIndex total_size = 2;
  auto key_binary = String(key_field_name).serialize();
  total_size += key_binary->get_length();
  binaries.push_back(std::move(key_binary));

  for (const auto &d: data){
    binaries.push_back(d.serialize());
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
  auto temp_str = String();
  index = temp_str.deserialize(binary, index).unwrap();
  key_field_name = temp_str.get_string();
  for (int i = 0; i < data_count; i++){
    auto new_data = DBPointer();
    index = new_data.deserialize(binary, index).unwrap();
    data.push_back(std::move(new_data));
  }
  return Ok(index);
}
bool DataNode::operator==(const DataNode &rhs) const {
  if (left != rhs.left || right != rhs.right || data.size() != rhs.data.size() || key_field_name != rhs.key_field_name){
    return false;
  } else {
    for (int i = 0; i < data.size(); i++){
      if (data[i] != rhs.data[i]){
        return false;
      }
    }
  }
  return true;
}
bool DataNode::operator!=(const DataNode &rhs) const {
  return !(rhs == *this);
}
