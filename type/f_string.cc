//
// Created by 김태현 on 2022/05/02.
//

#include <f_string.h>

#include <utility>
#include "type.h"

String::String() : Type(TypeKind::STRING) {}

BinaryUnique String::serialize() const {
  auto str_size = str.size();
  auto byte_count = get_byte_count(str_size);
  if (byte_count > 15) {
    throw SerializeError("Too long string!");
  }
  auto binary = BinaryFactory::create(str.size() + 1 + byte_count);
  binary->set_mem(0, Location_in_byte::FirstFourBit, type_to_byte(TypeKind::STRING));
  binary->set_mem(0, Location_in_byte::SecondFourBit, byte_count);

  BinaryIndex b_index = 1;
  for (int i = 0; i < byte_count; i++) {
    auto data = str_size >> ((byte_count - (i + 1)) * 8);
    binary->set_mem(b_index, (char) data);
    b_index++;
  }

  for (char i : str) {
    binary->set_mem(b_index, i);
    b_index++;
  }

  return binary;
}

Result<BinaryIndex, DeserializeError> String::deserialize(const Binary &binary, BinaryIndex begin) {
  auto type = byte_to_type(binary.read_mem(begin, Location_in_byte::FirstFourBit));
  if (type != field_type){
    return Err(DeserializeError("Wrong type of binary!"));
  }
  Byte size_char_count = binary.read_mem(begin, Location_in_byte::SecondFourBit);
  int size = 0;
  BinaryIndex index = begin + 1;
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

std::ostream &String::out(std::ostream &os) const {
  os << "String{field_type: " << this->field_type << ", str: " << this->str << "}";
  return os;
}
bool String::eq(const Type &rhs) const {
  auto rh = dynamic_cast<const String&>(rhs);
  return this->field_type == rh.field_type && str == rh.str;
}

String::String(std::string str): Type(TypeKind::STRING), str(std::move(str)) {}

bool String::under(const Type &rhs) const {
  if (rhs.field_type != TypeKind::STRING){
    throw CannotConvert("rhs is not String!");
  }
  return str < dynamic_cast<const String&>(rhs).str;
}

