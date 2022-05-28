//
// Created by 김태현 on 2022/05/02.
//
#include <mem_core.h>

Binary create_binary(BINARY_INDEX length) {
  return {std::make_unique<unsigned char []>(length), length};
}

bool set_mem(unsigned char& dest, const unsigned char& value, Location_in_byte loc){
    if (value > 15){
        return false;
    }
    auto v = std::bitset<4>(value);
    if (loc == Location_in_byte::First){
        for (char i = 0; i < 4; i++){
            dest |= v[i] << i + 4;
        }
    } else{
        for (char i = 0; i < 4; i++){
            dest |= v[i] << i;
        }
    }
    return true;
}

void set_mem(unsigned char& dest, const unsigned char& value){
    dest = value;
}

void set_mem(unsigned char& dest1, unsigned char& dest2, const unsigned char& value){
    std::bitset<8> y(value);
    for (int i = 0; i < 4; i++){
        dest1 |= y[i+4] << i;
        dest2 |= y[i] << i + 4;
    }
}

void read_mem(const unsigned char &origin, unsigned char &value, Location_in_byte loc) {
    if (loc == Location_in_byte::First){
        for (int i = 0; i < 4; i++){
            auto b = (origin >> (i + 4)) & 1;
            value |= b << i;
        }
    } else {
        for (int i = 0; i < 4; i++){
            auto b = (origin >> i) & 1;
            value |= b << i;
        }
    }
}

void read_mem(const unsigned char &origin, unsigned char &value) {
    value = origin;
}

void read_mem(const unsigned char &origin1, const unsigned char &origin2, unsigned char &value) {
    for (int i = 0; i < 4; i++){
        auto b = (origin1 >> i) & 1;
        value |= b << (i + 4);
        auto c = (origin2 >> (i + 4)) & 1;
        value |= c << i;
    }
}

int byte_count_of_str(int size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}

int write_str_size_bits(std::unique_ptr<unsigned char[]> &binary, int size, int byte_count) {
  int b_index_after_header = 1;
  for (int i = 0; i < byte_count; i++) {
    int data = size >> ((byte_count - (i + 1)) * 8);
    set_mem(binary[b_index_after_header], (char) data);
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