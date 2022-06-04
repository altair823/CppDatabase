//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_

#include <memory>
#include <serializable.h>
#include <type_traits>

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

struct can_serialize_impl {
  template <typename Key, typename Value,
  typename k_s = decltype(std::declval<Key>().serialize()),
  typename v_s = decltype(std::declval<Value>().serialize())>
      static std::true_type test(int);
  template<typename ...>
    static std::false_type test(...);
  };

template <typename Key, typename Value>
struct can_serialize : decltype(can_serialize_impl::test<Key, Value>(0)) {};

template <typename Key, typename Value>
class Data : public Serializable{
  static_assert(can_serialize<Key, Value>::value, "Key and Value should be serializable!");
 public:
  Key get_key() {return key;}
  Value get_value() {return value;}

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
 private:
  friend class DataFactory<Key, Value>;
  Data(Key key, Value value): key(key), value(value) {}
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
}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
