/**
 * @file mem_core.cc
 * @brief Memory core functions and binary class.
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_DATACORE_H
#define CPPDATABASE_DATACORE_H

#include <bitset>
#include <vector>
#include <memory>
#include <stdexcept>
#include <ostream>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <error.h>
#include "result.h"

using BinaryIndex = long long int ;
using Byte = std::uint8_t;
using int8 = std::int8_t; // char
using int16 = std::int16_t; // short
using int32 = std::int32_t; // int
using int64 = std::int64_t; // long

constexpr unsigned int TWO_GB = 2147483648;
constexpr unsigned int NODE_LIMIT = 4000;

/**
 * @brief Location of 4 bits that in the byte.
 * @details This enum is used to distinguish between 4 bits in the byte.
 */
enum class Location_in_byte {
  FirstFourBit,
  SecondFourBit,
};

/**
 * @brief Metadata of the binary.
 * @details This class is used to store the metadata of the binary.
 */
struct Metadata{
  Metadata(BinaryIndex offset, BinaryIndex length): offset(offset), length(length) {}
  BinaryIndex offset; // current offset of the binary
  BinaryIndex length; // length of the binary
};

class Binary;

using BinaryUnique = std::unique_ptr<Binary>;

/**
 * @brief Binary factory class.
 * @details This class is used to create Binary object.
 * @see Binary
 * @see BinaryUnique
 */
class BinaryFactory {
 public:
  /**
   * @brief Create a Binary object.
   * @param length Length of the binary.
   * @return BinaryUnique
   */
  static BinaryUnique create(BinaryIndex length);

  /**
   * @brief Create a Binary object.
   * @param binary_data Existing binary data.
   * @param length Length of the binary.
   * @return BinaryUnique
   */
  static BinaryUnique create(std::unique_ptr<Byte[]> binary_data, BinaryIndex length);

  /**
   * @brief Read a binary from a file.
   * @param file File path.
   * @param offset Offset of the binary.
   * @param length Length of the binary.
   * @return BinaryUnique
   */
  static BinaryUnique read(const std::filesystem::path& file, BinaryIndex offset, BinaryIndex length);
};

/**
 * @brief Binary class.
 * @details This class is used to store binary data.
 * @see BinaryUnique
 */
class [[nodiscard]] Binary{
 public:

  /**
   * @brief Get the length of the binary.
   * @return BinaryIndex
   */
  [[nodiscard]] BinaryIndex get_length() const{return length;}

  /**
   * @brief Set value to the specific location of the binary.
   * @param index Index of the binary.
   * @param loc Location in the binary.
   * @param value Value to be set.
   */
  void set_mem(BinaryIndex index, Location_in_byte loc, const Byte &value);

  /**
   * @brief Set value to the binary.
   * @details This function will set the value to the whole binary(8 bits).
   * @param index Index of the binary.
   * @param value Value to be set.
   */
  void set_mem(BinaryIndex index, const Byte &value);

  /**
   * @brief Set value to the first and second binary.
   * @details This function will set the value to the first(in last 4 bits) and second binary(in first 4 bits).
   * @param index1 Index of the first binary.
   * @param index2 Index of the second binary.
   * @param value Value to be set.
   */
  void set_mem(BinaryIndex index1, BinaryIndex index2, const Byte &value);

  /**
   * @brief Get value from the specific location of the binary.
   * @param index Index of the binary.
   * @param loc Location in the binary.
   * @return Byte value.
   */
  [[nodiscard]] Byte read_mem(BinaryIndex index, Location_in_byte loc) const;

  /**
   * @brief Get value from the binary.
   * @param index Index of the binary.
   * @return Byte value.
   */
  [[nodiscard]] Byte read_mem(BinaryIndex index) const;

  /**
   * @brief Get 1 byte value from the first(in last 4 bits) and second binary(in first 4 bits).
   * @param index1 Index of the first binary.
   * @param index2 Index of the second binary.
   * @return Byte value.
   */
  [[nodiscard]] Byte read_mem(BinaryIndex index1, BinaryIndex index2) const;

  /**
   * @brief Save the binary to a file.
   * @param file File path.
   * @return Metadata of the binary if success, otherwise return NotFound result.
   */
  Result<Metadata, NotFound> save(const std::filesystem::path& file);

  /**
   * @brief Save the binary to a file.
   * @param file File path.
   * @param offset Offset of the binary.
   * @return Metadata of the binary if success, otherwise return NotFound result.
   */
  Result<Metadata, NotFound> save(const std::filesystem::path& file, BinaryIndex offset);

  BinaryUnique operator+(const Binary& binary_ref) const;
  friend std::ostream &operator<<(std::ostream &os, const Binary &binary);

 private:
  friend class BinaryFactory;

  std::unique_ptr<Byte[]> data;
  BinaryIndex length;

  explicit Binary(BinaryIndex length);
  Binary(std::unique_ptr<Byte[]> binary_data, BinaryIndex length);
  void verify_index(BinaryIndex index) const;
};

/**
 * @brief Get byte count of the specific integer saved in binary.
 * @param integer Integer to be saved.
 * @return Byte count.
 */
int get_byte_count(unsigned long long integer);

/**
 * @brief Convert 64 bits integer to vector of bytes.
 * @param num 64 bits integer to be converted.
 * @return Vector of bytes.
 */
std::vector<Byte> uint64_to_char_vec(std::uint64_t num);

/**
 * @brief Convert 32 bits integer to vector of bytes.
 * @param num 32 bits integer to be converted.
 * @return Vector of bytes.
 */
std::vector<Byte> uint32_to_char_vec(std::uint32_t num);

/**
 * @brief Convert 16 bits integer to vector of bytes.
 * @param num 16 bits integer to be converted.
 * @return Vector of bytes.
 */
std::vector<Byte> uint16_to_char_vec(std::uint16_t num);

/**
 * @brief Convert 8 bits integer to vector of bytes.
 * @param num 8 bits integer to be converted.
 * @return Vector of bytes.
 */
std::vector<Byte> uint8_to_char_vec(std::uint8_t num);

/**
 * @brief Convert vector of bytes to 64 bits integer.
 * @param char_vec Vector of bytes to be converted.
 * @return 64 bits integer.
 */
unsigned long long byte_vec_to_num(std::vector<Byte> char_vec);

#endif //CPPDATABASE_DATACORE_H
