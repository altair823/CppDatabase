//
// Created by 김태현 on 2022/06/01.
//

#ifndef CPPDATABASE_INCLUDES_SERIALIZABLE_H_
#define CPPDATABASE_INCLUDES_SERIALIZABLE_H_

#include "mem_core.h"
#include "error.h"
#include "result.h"

class Serializable {
 public:
  [[nodiscard]] virtual BinaryUnique serialize() const = 0;
  virtual Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) = 0;
  virtual Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary) {return deserialize(binary, 0);};
  void _can_serialize() {};
  void _can_deserialize() {};
};


struct can_serialize_impl {
  template<typename T,
      typename = decltype(std::declval<T>()._can_serialize())>
  static std::true_type test(int);
  template<typename ...>
  static std::false_type test(...);
};
template <typename T>
struct can_serialize : decltype(can_serialize_impl::test<T>(0)) {};

struct can_deserialize_impl {
  template<typename T,
      typename = decltype(std::declval<T>()._can_deserialize())>
  static std::true_type test(int);
  template<typename ...>
  static std::false_type test(...);
};
template <typename T>
struct can_deserialize : decltype(can_deserialize_impl::test<T>(0)) {};


#endif //CPPDATABASE_INCLUDES_SERIALIZABLE_H_
