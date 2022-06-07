//
// Created by 김태현 on 2022/06/04.
//

#include <f_int.h>
#include "type.h"
BinaryUnique Int::serialize() const {
  auto int_byte_vec = num_to_char_vec(value);
  std::vector<Byte> valid_bytes;
  bool flag = false;
  for (auto &i: int_byte_vec){
    if (flag || i != 0){
      valid_bytes.push_back(i);
      flag = true;
    }
  }
  if (valid_bytes.size() > 4){
    throw SerializeError("Too big integer");
  }
  auto binary_unique = BinaryFactory::create(1 + valid_bytes.size());
  binary_unique->set_mem(0, Location_in_byte::FirstFourBit, type_to_4_byte(TypeKind::INT));
  binary_unique->set_mem(0, Location_in_byte::SecondFourBit, valid_bytes.size());
  int index = 1;
  for (auto &i: valid_bytes){
    binary_unique->set_mem(index, i);
    index++;
  }
  return binary_unique;
}
Result<BinaryIndex, DeserializeError> Int::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  auto type = byte_to_type(binary.read_mem(index, Location_in_byte::FirstFourBit));
  if (type != field_type){
    return Err(DeserializeError("Wrong type of binary!"));
  }
  int int_bytes_count = binary.read_mem(index, Location_in_byte::SecondFourBit);
  index++;
  std::vector<Byte> int_bytes_vec;
  for (int i = 0; i < int_bytes_count; i++, index++){
    int_bytes_vec.push_back(binary.read_mem(index));
  }
  value = (int32)byte_vec_to_num(int_bytes_vec);
  return Ok(index);
}
bool Int::eq(const Type &rhs) const {
  return this->field_type == rhs.field_type && value == dynamic_cast<const Int&>(rhs).value;
}
std::ostream &Int::out(std::ostream &ostream) const {
  ostream << "Int{field_type: " << this->field_type << ", value: " << this->value << "}";
  return ostream;
}
bool Int::under(const Type &rhs) const {
  if (rhs.field_type != TypeKind::INT){
    throw CannotConvert("rhs is not Int!");
  }
  return value < dynamic_cast<const Int&>(rhs).value;
}