//
// Created by Administrator on 2022-05-02.
//

#ifndef CPPDATABASE_DATACORE_H
#define CPPDATABASE_DATACORE_H

#include <bitset>
#include <type/field_data.h>

enum class Location_in_byte {
  First,
  Second,
};

void set_field_type(unsigned char &dest, Type &field_type);

bool set_mem(unsigned char &dest, const unsigned char &value, Location_in_byte loc);
void set_mem(unsigned char &dest, const unsigned char &value);
void set_mem(unsigned char &dest1, unsigned char &dest2, const unsigned char &value);

void read_mem(const unsigned char &origin, unsigned char &value, Location_in_byte loc);
void read_mem(const unsigned char &origin, unsigned char &value);
void read_mem(const unsigned char &origin1, const unsigned char &origin2, unsigned char &value);

#endif //CPPDATABASE_DATACORE_H
