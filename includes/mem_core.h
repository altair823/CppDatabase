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

#define BINARY_INDEX unsigned long long int

enum class Location_in_byte {
  FirstFourBit,
  SecondFourBit,
};

class Binary;

using BinaryUnique = std::unique_ptr<Binary>;

class BinaryFactory {
 public:
  static BinaryUnique create(BINARY_INDEX length);
};

class Binary{
 public:
  [[nodiscard]] BINARY_INDEX get_length() const{return length;}

  void set_mem(BINARY_INDEX index, Location_in_byte loc, const unsigned char &value);
  void set_mem(BINARY_INDEX index, const unsigned char &value);
  void set_mem(BINARY_INDEX index1, BINARY_INDEX index2, const unsigned char &value);

  [[nodiscard]] unsigned char read_mem(BINARY_INDEX index, Location_in_byte loc) const;
  [[nodiscard]] unsigned char read_mem(BINARY_INDEX index) const;
  [[nodiscard]] unsigned char read_mem(BINARY_INDEX index1, BINARY_INDEX index2) const;

  BinaryUnique operator+(const Binary& binary_ref) const;
  friend std::ostream &operator<<(std::ostream &os, const Binary &binary);

 private:
  friend class BinaryFactory;

  std::unique_ptr<unsigned char[]> data;
  BINARY_INDEX length;

  explicit Binary(BINARY_INDEX length);
  void verify_index(BINARY_INDEX index) const;
};
#define WRONG_BINARY BinaryFactory::create(0)

int byte_count_of_str(int size);
std::vector<unsigned char> num_to_char_vec(unsigned long long int num);
unsigned long long char_vec_to_num(std::vector<unsigned char> char_vec);
int write_str_size_bits(BinaryUnique &binary, int size, int byte_count);

#endif //CPPDATABASE_DATACORE_H
