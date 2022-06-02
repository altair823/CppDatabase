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

using BinaryIndex = unsigned long long int ;
using Byte = std::uint8_t;


enum class Location_in_byte {
  FirstFourBit,
  SecondFourBit,
};

class Binary;

using BinaryUnique = std::unique_ptr<Binary>;

class BinaryFactory {
 public:
  static BinaryUnique create(BinaryIndex length);
};

class Binary{
 public:
  [[nodiscard]] BinaryIndex get_length() const{return length;}

  void set_mem(BinaryIndex index, Location_in_byte loc, const Byte &value);
  void set_mem(BinaryIndex index, const Byte &value);
  void set_mem(BinaryIndex index1, BinaryIndex index2, const Byte &value);

  [[nodiscard]] Byte read_mem(BinaryIndex index, Location_in_byte loc) const;
  [[nodiscard]] Byte read_mem(BinaryIndex index) const;
  [[nodiscard]] Byte read_mem(BinaryIndex index1, BinaryIndex index2) const;

  BinaryUnique operator+(const Binary& binary_ref) const;
  friend std::ostream &operator<<(std::ostream &os, const Binary &binary);

 private:
  friend class BinaryFactory;

  std::unique_ptr<Byte[]> data;
  BinaryIndex length;

  explicit Binary(BinaryIndex length);
  void verify_index(BinaryIndex index) const;
};
#define WRONG_BINARY BinaryFactory::create(0)

int byte_count_of_str(int size);
std::vector<Byte> num_to_char_vec(unsigned long long int num);
unsigned long long char_vec_to_num(std::vector<Byte> char_vec);
int write_str_size_bits(BinaryUnique &binary, int size, int byte_count);

#endif //CPPDATABASE_DATACORE_H
