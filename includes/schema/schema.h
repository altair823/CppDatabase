//
// Created by Administrator on 2022-05-01.
//

#ifndef CPPDATABASE_SCHEMA_H
#define CPPDATABASE_SCHEMA_H

#include <ostream>

enum class Schema{
    None = 0,
    Name = 1,
    CreatedDate = 2,
    EditedDate = 3,
};

unsigned char schema_to_4_bits(Schema schema);
Schema bits_to_schema(unsigned char bits);
std::ostream& operator<<(std::ostream& os, const Schema& schema);

#endif //CPPDATABASE_SCHEMA_H
