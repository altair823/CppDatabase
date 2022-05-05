//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_B_TREE_H_
#define CPPDATABASE_INCLUDES_STORAGE_B_TREE_H_

#include <storage_interface.h>

template <typename Key, typename Value>
class BTree: public Storage<Key, Value>{
 public:
  BTree() = default;;
  Result<bool> insert(Key key, Value value) override {
    return Err(false, "there is no storage.");
  }
  Result<Value> search(Key key) override {
    return Err((Value) nullptr, "there is no storage.");
  }
  Result<Value> pop(Key key) override {
    return Err((Value) nullptr, "there is no storage.");
  }
  Result<bool> del(Key key) override {
    return Err(false, "there is no storage.");
  }
};

#endif //CPPDATABASE_INCLUDES_STORAGE_B_TREE_H_
