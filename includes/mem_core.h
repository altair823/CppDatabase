//
// Created by Administrator on 2022-05-02.
//

#ifndef CPPDATABASE_DATACORE_H
#define CPPDATABASE_DATACORE_H

#include <bitset>
#include <vector>
#include <memory>

#define BINARY_INDEX unsigned long long int

struct Binary{
  std::unique_ptr<unsigned char[]> data;
  BINARY_INDEX length;

  Binary(std::unique_ptr<unsigned char[]> data, BINARY_INDEX length): data(std::move(data)), length(length) {}
};
#define WRONG_BINARY Binary(nullptr, 0)

typedef Binary& BinaryRef;

Binary create_binary(BINARY_INDEX length);

enum class Location_in_byte {
  First,
  Second,
};


bool set_mem(unsigned char &dest, const unsigned char &value, Location_in_byte loc);
void set_mem(unsigned char &dest, const unsigned char &value);
void set_mem(unsigned char &dest1, unsigned char &dest2, const unsigned char &value);

void read_mem(const unsigned char &origin, unsigned char &value, Location_in_byte loc);
void read_mem(const unsigned char &origin, unsigned char &value);
void read_mem(const unsigned char &origin1, const unsigned char &origin2, unsigned char &value);

int byte_count_of_str(int size);
std::vector<unsigned char> num_to_char_vec(unsigned long long int num);
int write_str_size_bits(std::unique_ptr<unsigned char[]> &binary, int size, int byte_count);

#endif //CPPDATABASE_DATACORE_H
