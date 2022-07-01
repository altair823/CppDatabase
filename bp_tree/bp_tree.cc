//
// Created by 김태현 on 2022/06/16.
//

#include <bp_tree.h>

#include <utility>


BPTree::BPTree(SchemaShared schema, std::string key_field_name)
: head(nullptr), schema(std::move(schema)), key_field_name(std::move(key_field_name)), current_file("db_file1.bin") {
}
bool BPTree::insert(const Key& key, const Value& value, bool to_override) {
  if (this->is_empty()){
    auto head_info_binary = BinaryFactory::read(current_file, 0, 32);
    Int head_length;
    head_length.deserialize(*head_info_binary, 0).unwrap();
    auto head_binary = BinaryFactory::read(current_file, (long long)head_info_binary->get_length(), head_length.get_int());
    head = IndexNodeFactory::create(schema, key_field_name);
    head->deserialize(*head_binary, 0).unwrap();

    if (head->is_leaf() && head->get_pointer_count() == 0) {
      auto left_data = DataNodeFactory::create(schema, key_field_name);
      auto left_binary = left_data->serialize();
      auto metadata = left_binary->save(current_file).unwrap();
      DBPointer left_pointer(current_file, metadata.offset, metadata.length);
      auto right_data = DataNodeFactory::create(schema, key_field_name);
      auto right_binary = right_data->serialize();
      head->push_back_key(key);
    //head->push_back_pointer()
    }

    IndexNode *current = head.get();
    while (current->is_leaf()){
      int key_index = current->search_key(key);
      current = current->get_index_child(key_index).get();
    }



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