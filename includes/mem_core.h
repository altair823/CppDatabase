//
// Created by Administrator on 2022-05-02.
//

#ifndef CPPDATABASE_DATACORE_H
#define CPPDATABASE_DATACORE_H

#include "schema/schema.h"
#include <bitset>

typedef bool Location_in_byte;
#define First true
#define Second false

void set_field_type(unsigned char& dest, Schema& field_type);

bool set_mem(unsigned char& dest, unsigned char& value, Location_in_byte loc);
void set_mem(unsigned char& dest, unsigned char& value);
void set_mem(unsigned char& dest1, unsigned char& dest2, unsigned char& value);

void read_mem(unsigned char& origin, unsigned char& value, Location_in_byte loc);
void read_mem(unsigned char& origin, unsigned char& value);
void read_mem(unsigned char& origin1, unsigned char& origin2, unsigned char& value);

#endif //CPPDATABASE_DATACORE_H
