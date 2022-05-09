//
// Created by 김태현 on 2022/05/10.
//

#include <node.h>

IndexNode::IndexNode(const FieldData& new_key) {
  auto key = type_to_field(new_key.field_type).unwrap();
  keys.push_back(std::move(key));
}

Result<IndexNode *> IndexNode::push_back_key(const FieldData &key) {
  auto _key = type_to_field(key.field_type).unwrap();
  keys.push_back(std::move(_key));
  return Ok(this);
}
Result<IndexNode *> IndexNode::push_back_pointer(const unsigned long long int pointer) {
  pointers.push_back(pointer);
  return Ok(this);
}
Result<Binary> IndexNode::serialize() {
  int b_length = 0;
  for (auto& k: keys){
    b_length += k.get_total_byte_size();
  }
  for (auto& p: pointers){

  }
  auto binary_data = std::make_unique<unsigned char[]>(4);
}