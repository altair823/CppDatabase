//
// Created by 김태현 on 2022/05/02.
//
#include <mem_core.h>



int get_byte_count(unsigned long long size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}

int write_str_size_bits(BinaryUnique &binary, int size, int byte_count) {
  int b_index_after_header = 1;
  for (auto i = 0; i < byte_count; i++) {
    int data = size >> ((byte_count - (i + 1)) * 8);
    binary->set_mem(b_index_after_header, (char) data);
    b_index_after_header++;
  }
  return b_index_after_header;
}

std::vector<Byte> num_to_char_vec(unsigned long long int num){
  unsigned long long int a = (unsigned long long int)1 << 56;
  std::vector<Byte> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
  }
  return result;
}

unsigned long long byte_vec_to_num(std::vector<Byte> char_vec) {
  int start_index = -1;
  for (auto i = 0; i < (int)char_vec.size(); i++){
    if (char_vec[i] != 0){
      start_index = i;
      break;
    }
  }
  if (start_index == -1){
    return 0;
  }
  unsigned long long result = 0;
  for (auto i = start_index; i < (int)char_vec.size(); i++){
    result += char_vec[i - start_index] << ((char_vec.size() - start_index - i - 1) * 8);
  }
  return result;
}

BinaryUnique BinaryFactory::create(unsigned long long length) {
  std::unique_ptr<Binary> binary(new Binary(length));
  return binary;
}

Binary::Binary(unsigned long long int length): length(length) {
  if (length == 0){
    data = nullptr;
  } else {
    data = std::make_unique<Byte[]>(length);
  }
}

void Binary::set_mem(unsigned long long int index, Location_in_byte loc, const Byte &value) {
  verify_index(index);
  if (value > 15) {
    throw std::overflow_error("The value is 16 or higher! ");
  }
  auto v = std::bitset<4>(value);
  if (loc == Location_in_byte::FirstFourBit) {
    for (char i = 0; i < 4; i++) {
      data[index] |= v[i] << i + 4;
    }
  } else {
    for (char i = 0; i < 4; i++) {
      data[index] |= v[i] << i;
    }
  }
}
void Binary::set_mem(unsigned long long int index, const Byte &value) {
  verify_index(index);
  data[index] = value;
}
void Binary::set_mem(unsigned long long int index1, unsigned long long int index2, const Byte &value) {
  verify_index(index1);
  verify_index(index2);
  std::bitset<8> y(value);
  for (auto i = 0; i < 4; i++) {
    data[index1] |= y[i + 4] << i;
    data[index2] |= y[i] << i + 4;
  }
}
void Binary::verify_index(BinaryIndex index) const {
  if (index >= length){
    throw std::range_error("Binary index is out of range!");
  }
}
Byte Binary::read_mem(BinaryIndex index, Location_in_byte loc) const {
  Byte result = 0;
  if (loc == Location_in_byte::FirstFourBit) {
    for (auto i = 0; i < 4; i++) {
      auto b = (data[index] >> (i + 4)) & 1;
      result |= b << i;
    }
  } else {
    for (int i = 0; i < 4; i++) {
      auto b = (data[index] >> i) & 1;
      result |= b << i;
    }
  }
  return result;
}
Byte Binary::read_mem(BinaryIndex index) const {
  return data[index];
}
Byte Binary::read_mem(BinaryIndex index1, BinaryIndex index2) const {
  Byte result = 0;
  for (auto i = 0; i < 4; i++) {
    auto b = (data[index1] >> i) & 1;
    result |= b << (i + 4);
    auto c = (data[index2] >> (i + 4)) & 1;
    result |= c << i;
  }
  return result;
}
BinaryUnique Binary::operator+(const Binary &binary_ref) const {
  auto result_binary = BinaryFactory::create(this->length + binary_ref.length);
  for (BinaryIndex i = 0; i < this->length; i++){
    result_binary->data[i] = this->data[i];
  }
  for (BinaryIndex i = 0; i < binary_ref.length; i++){
    result_binary->data[this->length + i] = binary_ref.data[i];
  }
  return result_binary;
}
std::ostream &operator<<(std::ostream &os, const Binary &binary) {
  for (BinaryIndex i = 0; i < binary.length; i++){
    for (int j = 7; j >= 0; j--){
      auto a = binary.data[i];
      auto b = (a >> j) & 1;
      os << b;
      if (j == 4){
        os << " ";
      }
    }
    os << "\n";
  }
  return os;
}
