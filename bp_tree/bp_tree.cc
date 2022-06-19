//
// Created by 김태현 on 2022/06/16.
//

#include <bp_tree.h>

#include <utility>


BPTree::BPTree(SchemaShared schema, std::string key_field_name): head(nullptr), schema(std::move(schema)), key_field_name(std::move(key_field_name)) {
}
bool BPTree::insert(Key key, Value value, bool to_override) {
  if (this->is_empty()){
    head = IndexNodeFactory::create(schema, key_field_name);
    auto left_data = DataNodeFactory::create(schema, key_field_name);
    auto right_data = DataNodeFactory::create(schema, key_field_name);
    head->push_back_key(key);
  }
  return true;
}
Result<Value, NotFound> BPTree::search(Key key) {
  if (this->is_empty()) {
    return Err(NotFound("The tree is empty!"));
  }

}
Result<std::vector<Value>, NotFound> BPTree::search(Key begin, Key end) {
  if (this->is_empty()) {
    return Err(NotFound("The tree is empty!"));
  }
}
bool BPTree::remove(Key key) {
  return false;
}
bool BPTree::is_empty() {
  if (head == nullptr) {
    return true;
  } else{
    return false;
  }
}