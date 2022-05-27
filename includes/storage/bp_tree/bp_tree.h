//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_

#include "../storage_interface.h"
#include "index_node.h"
#include "data_node.h"

template <typename Key, typename Value>
class BPTree : public Storage<Key, Value> {
 public:
 private:
 public:
  bool insert(Key key, Value value, bool to_override) override;
  Result<Value, std::string> search(Key key) override;
  Result<std::vector<Value>, std::string> search(Key begin, Key end) override;
  bool remove(Key key) override;

};
template<typename Key, typename Value>
bool BPTree<Key, Value>::insert(Key key, Value value, bool to_override) {
  return false;
}
template<typename Key, typename Value>
Result<Value, std::string> BPTree<Key, Value>::search(Key key) {
  return Ok(nullptr);
}
template<typename Key, typename Value>
Result<std::vector<Value>, std::string> BPTree<Key, Value>::search(Key begin, Key end) {
  return Ok(std::vector<Value>());
}
template<typename Key, typename Value>
bool BPTree<Key, Value>::remove(Key key) {
  return false;
}

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
