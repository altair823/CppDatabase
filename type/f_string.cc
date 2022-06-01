//
// Created by 김태현 on 2022/05/02.
//

#include <f_string.h>

#include <utility>

String::String() : FieldData(Type::STRING) {}

BinaryUnique String::serialize() const {
  int str_bits = (int) str.size();
  auto byte_count = byte_count_of_str(str_bits);
  if (byte_count > 15) {
    return WRONG_BINARY;
  }
  auto binary = BinaryFactory::create(str.size() + 1 + byte_count);
  binary->set_mem(0, Location_in_byte::FirstFourBit, type_to_4_bits(Type::STRING));
  binary->set_mem(0, Location_in_byte::SecondFourBit, byte_count);

  BINARY_INDEX b_index = 1;
  for (int i = 0; i < byte_count; i++) {
    int data = str_bits >> ((byte_count - (i + 1)) * 8);
    binary->set_mem(b_index, (char) data);
    b_index++;
  }

  for (char i : str) {
    binary->set_mem(b_index, i);
    b_index++;
  }

  return binary;
}

Result<BINARY_INDEX, DeserializeError> String::deserialize(const Binary &binary, BINARY_INDEX begin) {
  unsigned char type = binary.read_mem(begin, Location_in_byte::FirstFourBit);
  unsigned char size_char_count = binary.read_mem(begin, Location_in_byte::SecondFourBit);
  int size = 0;
  BINARY_INDEX index = begin + 1;
  for (int i = 0; i < size_char_count; i++) {
    size += binary.read_mem(index) << 8 * (size_char_count - (i + 1));
    index++;
  }

  for (int i = 0; i < size; i++) {
    str.push_back((char) binary.read_mem(index));
    index++;
  }
  return Ok(index);
}

std::ostream &operator<<(std::ostream &os, const String &string) {
  os << "field_type: " << string.field_type << " str: " << string.get_string();
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
  auto size_byte_count = byte_count_of_str(str_size);
  return str_size + size_byte_count + 1;
}

