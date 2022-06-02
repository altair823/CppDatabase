//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_

#include <memory>

template <typename Key, typename Value>
class Data;

template <typename Key, typename Value>
using DataUnique = std::unique_ptr<Data<Key, Value>>;

template <typename Key, typename Value>
class DataFactory {
 public:
  static DataUnique<Key, Value> create(Key key, Value value);
};
template<typename Key, typename Value>
DataUnique<Key, Value> DataFactory<Key, Value>::create(Key key, Value value) {
  DataUnique<Key, Value> new_data(new Data<Key, Value>(key, value));
  return new_data;
}

template <typename Key, typename Value>
class Data {
 public:
  Key get_key() {return key;}
  Value get_value() {return value;}
 private:
  friend class DataFactory<Key, Value>;
  Data(Key key, Value value): key(key), value(value) {}
  Key key;
  Value value;
};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
