//
// Created by Administrator on 2022-05-02.
//

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

enum class Location_in_byte {
  FirstFourBit,
  SecondFourBit,
};

struct Metadata{
  Metadata(BinaryIndex offset, BinaryIndex length): offset(offset), length(length) {}
  BinaryIndex offset;
  BinaryIndex length;
};

class Binary;

using BinaryUnique = std::unique_ptr<Binary>;

class BinaryFactory {
 public:
  static BinaryUnique create(BinaryIndex length);
  static BinaryUnique create(std::unique_ptr<Byte[]> byte_buffer, BinaryIndex length);
  static BinaryUnique read(const std::filesystem::path& file, BinaryIndex offset, BinaryIndex length);
};

class [[nodiscard]] Binary{
 public:
  [[nodiscard]] BinaryIndex get_length() const{return length;}

  void set_mem(BinaryIndex index, Location_in_byte loc, const Byte &value);
  void set_mem(BinaryIndex index, const Byte &value);
  void set_mem(BinaryIndex index1, BinaryIndex index2, const Byte &value);

  [[nodiscard]] Byte read_mem(BinaryIndex index, Location_in_byte loc) const;
  [[nodiscard]] Byte read_mem(BinaryIndex index) const;
  [[nodiscard]] Byte read_mem(BinaryIndex index1, BinaryIndex index2) const;

  Result<Metadata, NotFound> save(const std::filesystem::path& file);

  BinaryUnique operator+(const Binary& binary_ref) const;
  friend std::ostream &operator<<(std::ostream &os, const Binary &binary);

 private:
  friend class BinaryFactory;

  std::unique_ptr<Byte[]> data;
  BinaryIndex length;

  explicit Binary(BinaryIndex length);
  Binary(std::unique_ptr<Byte[]> byte_buffer, BinaryIndex length);
  void verify_index(BinaryIndex index) const;
};

int get_byte_count(unsigned long long size);
std::vector<Byte> num_to_char_vec(unsigned long long int num);
unsigned long long byte_vec_to_num(std::vector<Byte> char_vec);
int write_str_size_bits(BinaryUnique &binary, int size, int byte_count);

#endif //CPPDATABASE_DATACORE_H
