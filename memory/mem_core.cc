//
// Created by 김태현 on 2022/05/02.
//
#include <mem_core.h>



int byte_count_of_str(int size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}

int write_str_size_bits(BinaryUnique &binary, int size, int byte_count) {
  int b_index_after_header = 1;
  for (int i = 0; i < byte_count; i++) {
    int data = size >> ((byte_count - (i + 1)) * 8);
    binary->set_mem(b_index_after_header, (char) data);
    b_index_after_header++;
  }
  return b_index_after_header;
}

std::vector<unsigned char> num_to_char_vec(unsigned long long int num){
  unsigned long long int a = (unsigned long long int)1 << 56;
  std::vector<unsigned char> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
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
    data = std::make_unique<unsigned char[]>(length);
  }
}

void Binary::set_mem(unsigned long long int index, Location_in_byte loc, const unsigned char &value) {
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
void Binary::set_mem(unsigned long long int index, const unsigned char &value) {
  verify_index(index);
  data[index] = value;
}
void Binary::set_mem(unsigned long long int index1, unsigned long long int index2, const unsigned char &value) {
  verify_index(index1);
  verify_index(index2);
  std::bitset<8> y(value);
  for (int i = 0; i < 4; i++) {
    data[index1] |= y[i + 4] << i;
    data[index2] |= y[i] << i + 4;
  }
}
void Binary::verify_index(BINARY_INDEX index) const {
  if (index >= length){
    throw std::range_error("Binary index is out of range!");
  } else if (data == nullptr){
    assert("Wrong binary!");
  }
}
unsigned char Binary::read_mem(BINARY_INDEX index, Location_in_byte loc) {
  unsigned char result = 0;
  if (loc == Location_in_byte::FirstFourBit) {
    for (int i = 0; i < 4; i++) {
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
unsigned char Binary::read_mem(BINARY_INDEX index) {
  return data[index];
}
unsigned char Binary::read_mem(BINARY_INDEX index1, BINARY_INDEX index2) {
  unsigned char result = 0;
  for (int i = 0; i < 4; i++) {
    auto b = (data[index1] >> i) & 1;
    result |= b << (i + 4);
    auto c = (data[index2] >> (i + 4)) & 1;
    result |= c << i;
  }
  return result;
}