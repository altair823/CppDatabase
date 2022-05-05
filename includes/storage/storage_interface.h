//
// Created by 김태현 on 2022/05/05.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_STORAGE_INTERFACE_H_
#define CPPDATABASE_INCLUDES_STORAGE_STORAGE_INTERFACE_H_

#include <result.h>

template<typename Key, typename Value>
class Storage{
 public:
  virtual Result<bool> insert(Key key, Value value) = 0;
  virtual Result<Value> search(Key key) = 0;
  virtual Result<Value> pop(Key key) = 0;
  virtual Result<bool> del(Key key) = 0;
};

template<typename Key, typename Value>
using StorageShared = std::shared_ptr<Storage<Key, Value>>;

#endif //CPPDATABASE_INCLUDES_STORAGE_STORAGE_INTERFACE_H_
