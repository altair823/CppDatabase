//
// Created by 김태현 on 2022/05/28.
//
#include "db_io.h"

Result<BinaryIndex, DeserializeError> DBPointer::deserialize(const Binary &binary, unsigned long long int start_index) {
  BinaryIndex index = start_index;
  auto file_name_byte_count = binary.read_mem(index);
  index++;
  for (int i = 0; i < file_name_byte_count; i++){
    char b = (char)binary.read_mem(index);
    file_name.push_back((char)b);
    index++;
  }
  auto offset_byte_count = binary.read_mem(index);
  index++;
  std::vector<Byte> valid_offset_bytes;
  for (int i = 0; i < offset_byte_count; i++){
    valid_offset_bytes.push_back(binary.read_mem(index));
    index++;
  }
  this->offset = byte_vec_to_num(valid_offset_bytes);
  return Ok(index);
}
BinaryUnique DBPointer::serialize() const {
  auto file_name_byte_count = (int) file_name.size();
  if (file_name_byte_count > 225){
    throw SerializeError("Too long file name!");
  }
  auto offset_to_chars = num_to_char_vec(offset);
  bool flag = false;
  std::vector<Byte> valid_offset_bytes;
  for (auto &f: offset_to_chars){
    if (flag || f != 0){
      valid_offset_bytes.push_back(f);
      flag = true;
    }
  }
  if (valid_offset_bytes.size() >= 256){
    throw SerializeError("File is too large!");
  }
  auto offset_byte_count = (Byte)valid_offset_bytes.size();
  auto binary = BinaryFactory::create(1 + file_name_byte_count + 1 + offset_byte_count);
  int index = 0;
  binary->set_mem(index, file_name_byte_count);
  index++;
  for (int i = 0; i < file_name_byte_count; i++){
    binary->set_mem(index, file_name[i]);
    index++;
  }
  binary->set_mem(index, offset_byte_count);
  index++;
  for (int i = 0; i < offset_byte_count; i++){
    binary->set_mem(index, valid_offset_bytes[i]);
    index++;
  }
  return binary;
}
std::ostream &operator<<(std::ostream &os, const DBPointer &pointer) {
  os << "file_name: " << pointer.file_name << ", offset: " << pointer.offset;
  return os;
}
bool DBPointer::operator==(const DBPointer &rhs) const {
  return file_name == rhs.file_name &&
      offset == rhs.offset;
}

