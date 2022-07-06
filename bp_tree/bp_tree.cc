//
// Created by 김태현 on 2022/06/16.
//

#include <bp_tree.h>

#include <utility>


BPTree::BPTree(SchemaShared schema, std::string key_field_name)
: head(nullptr), schema(std::move(schema)), key_field_name(std::move(key_field_name)), db_file("db_file1.bin"), record_file("record1.bin") {
  if (!std::filesystem::exists(db_file)){
    Int initial_offset(0);
    Int initial_length(0);
    auto initial_binary = initial_offset.serialize();
    initial_binary = *initial_binary + *initial_length.serialize();
    initial_binary->save(db_file).unwrap();
  }
}
bool BPTree::insert(const Key& key, const Value& value, bool to_override) {
  if (this->is_empty()){
    head = IndexNodeFactory::create(schema, key_field_name);
    auto head_info_binary = BinaryFactory::read(db_file, 0, Int::get_fixed_length() * 2);
    Int head_offset;
    BinaryIndex head_index = head_offset.deserialize(*head_info_binary, 0).unwrap();
    Int head_length;
    head_length.deserialize(*head_info_binary, head_index).unwrap();
    if (head_length.get_int() != 0) {
      auto head_binary =
          BinaryFactory::read(db_file, head_offset.get_int(), head_length.get_int());
      head->deserialize(*head_binary, 0).unwrap();
    }

    if (head->is_leaf() && head->get_pointer_count() == 0) {
      auto left_metadata = DataNodeFactory::create(schema, key_field_name, record_file)->serialize()->save(db_file).unwrap();
      DBPointer left_pointer(db_file, left_metadata.offset, left_metadata.length);
      auto right_metadata = DataNodeFactory::create(schema, key_field_name, record_file)->serialize()->save(db_file).unwrap();
      DBPointer right_pointer(db_file, right_metadata.offset, right_metadata.length);
      head->push_back_key(key);
      head->push_back_pointer(left_pointer);
      head->push_back_pointer(right_pointer);
      auto head_metadata = head->serialize()->save(db_file).unwrap();
      Int new_head_offset(head_metadata.offset);
      Int new_head_length(head_metadata.length);
      auto initial_binary = new_head_offset.serialize();
      initial_binary = *initial_binary + * new_head_length.serialize();
      initial_binary->save(db_file, 0).unwrap();
    }

    IndexNode *current = head.get();
    while (!current->is_leaf()){
      int key_index = current->search_key(key);
      current = current->get_index_child(key_index).get();
    }
    int index = current->search_key(key);
    auto data_node = current->get_data_child(index);
    int data_index = data_node->search(key);
    data_node->insert(data_index, value);

    // Split current data node when the number of data in the current data node exceeds the maximum.
    // And save all data nodes in file.
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