/**
 * @file data_node.h
 * @brief Serializable abstract class.
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_INCLUDES_SERIALIZABLE_H_
#define CPPDATABASE_INCLUDES_SERIALIZABLE_H_

#include "mem_core.h"
#include "error.h"
#include "result.h"

/**
 * @brief Serializable class
 * @details This class is an interface for serializing and deserializing.
 * @see Binary
 * @see BinaryUnique
 * @see BinaryIndex
 * @see SerializeError
 * @see DeserializeError
 * @see Result
 */
class Serializable {
 public:
  /**
   * @brief Serialize this object to Binary
   * @return BinaryUnique
   */
  [[nodiscard]] virtual BinaryUnique serialize() const = 0;

  /**
   * @brief Deserialize this object from Binary
   * @param binary Binary to deserialize
   * @param begin Begin index of binary
   * @return if success, return BinaryIndex of end index of binary. if fail, return DeserializeError
   */
  virtual Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) = 0;

  /**
   * @brief Deserialize this object from Binary
   * @param binary Binary to deserialize
   * @return if success, return BinaryIndex of end index of binary. if fail, return DeserializeError
   */
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
