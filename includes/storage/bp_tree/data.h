//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_

#include <memory>
#include <serializable.h>
#include <type_traits>
#include <ostream>

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
class Data : public Serializable {
  static_assert(can_serialize<Key>::value, "Key should be serializable!");
  static_assert(can_serialize<Value>::value, "Value should be serializable!");
  static_assert(can_deserialize<Key>::value, "Kay should be deserializable!");
  static_assert(can_deserialize<Value>::value, "Value should be deserializable!");
 public:
  Key get_key() { return key; }
  Value get_value() { return value; }
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  bool operator==(const Data &rhs) const;
  bool operator!=(const Data &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const Data<Key, Value> &data) {
    os << "<Data>\nkey: " << data.key << "\nvalue: " << data.value;
    return os;
  }
 private:
  friend class DataFactory<Key, Value>;
  Data(Key key, Value value) : key(key), value(value) {}
  Key key;
  Value value;
};

template<typename Key, typename Value>
BinaryUnique Data<Key, Value>::serialize() const {
  auto key_binary = key.serialize();
  auto value_binary = value.serialize();
  return *key_binary + *value_binary;
}
template<typename Key, typename Value>
Result<BinaryIndex, DeserializeError> Data<Key, Value>::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = key.deserialize(binary, begin).unwrap();
  index = value.deserialize(binary, index).unwrap();
  return Ok(index);
}
template<typename Key, typename Value>
bool Data<Key, Value>::operator==(const Data &rhs) const {
  return key == rhs.key && value == rhs.value;
}
template<typename Key, typename Value>
bool Data<Key, Value>::operator!=(const Data &rhs) const {
  return !(rhs == *this);
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
