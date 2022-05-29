//
// Created by 김태현 on 2022/05/28.
//
#include "db_io.h"

Result<BINARY_INDEX, DeserializeError> DBPointer::deserialize(Binary &binary, unsigned long long int start_index) {

}
BinaryUnique DBPointer::serialize() {
  auto file_name_byte_count = byte_count_of_str((int) file_name.size());
  if (file_name_byte_count > 225){
    return WRONG_BINARY;
  }
  auto offset_to_chars = num_to_char_vec(offset);
  bool flag = false;
  std::vector<unsigned char> valid_offset_bytes;
  for (auto &f: offset_to_chars){
    if (flag || f != 0){
      valid_offset_bytes.push_back(f);
      flag = true;
    }
  }
  if (valid_offset_bytes.size() >= 256){
    return WRONG_BINARY;
  }
  char offset_byte_count = (char)valid_offset_bytes.size();
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

