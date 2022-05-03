//
// Created by 김태현 on 2022/05/02.
//

#include <f_string.h>

String::String() : FieldType(Type::STRING) {}

std::unique_ptr<unsigned char[]> String::serialize() {
  auto binary = std::make_unique<unsigned char[]>(str.size() + 2);
  set_mem(binary[0], type_to_4_bits(Type::STRING), Location_in_byte::First);
  int str_bits = (int) str.size();
  auto bit_count = get_byte_count(str_bits);
  if (bit_count > 15) {
    return nullptr;
  }
  set_mem(binary[0], bit_count, Location_in_byte::Second);

  auto b_index = write_str_size_bits(binary, str_bits, bit_count);

  for (auto& c : str) {
    set_mem(binary[b_index], c);
    b_index++;
  }

  return binary;
}

void String::deserialize(std::unique_ptr<unsigned char[]> &binary) {
  unsigned char type = 0;
  read_mem(binary[0], type, Location_in_byte::First);
  unsigned char size_char_count = 0;
  read_mem(binary[0], size_char_count, Location_in_byte::Second);
  int size = 0;
  int index = 1;
  for (int i = 0; i < size_char_count; i++) {
    size += binary[index] << 8 * (size_char_count - (i + 1));
    index++;
  }

  for (int i = 0; i < size; i++) {
    str.push_back((char) binary[index]);
    index++;
  }
}

int String::get_byte_count(int size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}

int String::write_str_size_bits(std::unique_ptr<unsigned char[]> &binary, int size, int byte_count) {
  int b_index_after_header = 1;
  for (int i = 0; i < byte_count; i++) {
    int data = size >> ((byte_count - (i + 1)) * 8);
    set_mem(binary[b_index_after_header], (char) data);
    b_index_after_header++;
  }
  return b_index_after_header;
}

std::ostream &operator<<(std::ostream &os, const String &string) {
  os << "field_type: " << string.field_type << " str: " << string.str;
  return os;
}

bool String::operator==(const String &rhs) const {
  return this->field_type == rhs.field_type && str == rhs.str;
}
std::ostream &String::out(std::ostream &os) const {
  os << "field_type: " << this->field_type << " str: " << this->str;
  return os;
}

