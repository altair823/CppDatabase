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
class Data : public Serializable {
  static_assert(can_serialize<Key>::value, "Key should be serializable!");
  static_assert(can_serialize<Value>::value, "Value should be serializable!");
  static_assert(can_deserialize<Key>::value, "Kay should be deserializable!");
  static_assert(can_deserialize<Value>::value, "Value should be deserializable!");
 public:
  virtual std::shared_ptr<Key> get_key() const = 0;
  virtual std::shared_ptr<Value> get_value() const = 0;
  bool operator==(const Data &rhs) const;
  bool operator!=(const Data &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const Data<Key, Value> &data) {
    return data.out(os);
  }

 private:
  virtual bool eq(const Data &rhs) const = 0;
  virtual std::ostream& out(std::ostream&) const = 0;
};

template<typename Key, typename Value>
bool Data<Key, Value>::operator==(const Data &rhs) const {
  return eq(rhs);
}
template<typename Key, typename Value>
bool Data<Key, Value>::operator!=(const Data &rhs) const {
  return !eq(rhs);
}




#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_H_
