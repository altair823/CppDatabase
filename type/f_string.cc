//
// Created by 김태현 on 2022/05/02.
//

#include <f_string.h>

#include <utility>

String::String() : FieldData(Type::STRING) {}

Binary String::serialize() {
  auto binary = std::make_unique<unsigned char[]>(str.size() + 2);
  set_mem(binary[0], type_to_4_bits(Type::STRING), Location_in_byte::First);
  int str_bits = (int) str.size();
  auto bit_count = get_byte_count(str_bits);
  if (bit_count > 15) {
    return {nullptr, 0};
  }
  set_mem(binary[0], bit_count, Location_in_byte::Second);

  auto b_index = write_str_size_bits(binary, str_bits, bit_count);

  for (auto& c : str) {
    set_mem(binary[b_index], c);
    b_index++;
  }

  return {std::move(binary), b_index};
}

Result<int> String::deserialize(BinaryRef binary, int begin) {
  unsigned char type = 0;
  read_mem(binary.data[begin], type, Location_in_byte::First);
  unsigned char size_char_count = 0;
  read_mem(binary.data[begin], size_char_count, Location_in_byte::Second);
  int size = 0;
  int index = begin + 1;
  for (int i = 0; i < size_char_count; i++) {
    size += binary.data[index] << 8 * (size_char_count - (i + 1));
    index++;
  }

  if (size > binary.length - 1 - size_char_count){
    return Err(0, "Binary size error!");
  }

  for (int i = 0; i < size; i++) {
    str.push_back((char) binary.data[index]);
    index++;
  }
  return Ok(index);
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

std::ostream &String::out(std::ostream &os) const {
  os << "field_type: " << this->field_type << " str: " << this->str;
  return os;
}
bool String::eq(const FieldData &rhs) const {
  auto rh = dynamic_cast<const String&>(rhs);
  return this->field_type == rh.field_type && str == rh.str;
}

String::String(std::string str): FieldData(Type::STRING), str(std::move(str)) {}

int String::get_total_byte_size() const {
  int str_size = (int)str.size();
  auto size_byte_count = get_byte_count(str_size);
  return str_size + size_byte_count + 1;
}

