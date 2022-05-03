//
// Created by 김태현 on 2022/05/02.
//
#include <mem_core.h>

void set_field_type(unsigned char& dest, Type& field_type) {
    auto type = static_cast<unsigned char>(field_type);
    if (type < 16) {
        std::bitset<4> t(type);
        for (int i = 0; i < 4; i++) {
            dest |= t[i] << i + 4;
        }
    }
    // Assume the number of fields is less than 256.
    else {
        dest = type;
    }
}

bool set_mem(unsigned char& dest, const unsigned char& value, Location_in_byte loc){
    if (value > 15){
        return false;
    }
    auto v = std::bitset<4>(value);
    if (loc == Location_in_byte::First){
        for (char i = 0; i < 4; i++){
            dest |= v[i] << i + 4;
        }
    } else{
        for (char i = 0; i < 4; i++){
            dest |= v[i] << i;
        }
    }
    return true;
}

void set_mem(unsigned char& dest, const unsigned char& value){
    dest = value;
}

void set_mem(unsigned char& dest1, unsigned char& dest2, const unsigned char& value){
    std::bitset<8> y(value);
    for (int i = 0; i < 4; i++){
        dest1 |= y[i+4] << i;
        dest2 |= y[i] << i + 4;
    }
}

void read_mem(const unsigned char &origin, unsigned char &value, Location_in_byte loc) {
    if (loc == Location_in_byte::First){
        for (int i = 0; i < 4; i++){
            auto b = (origin >> (i + 4)) & 1;
            value |= b << i;
        }
    } else {
        for (int i = 0; i < 4; i++){
            auto b = (origin >> i) & 1;
            value |= b << i;
        }
    }
}

void read_mem(const unsigned char &origin, unsigned char &value) {
    value = origin;
}

void read_mem(const unsigned char &origin1, const unsigned char &origin2, unsigned char &value) {
    for (int i = 0; i < 4; i++){
        auto b = (origin1 >> i) & 1;
        value |= b << (i + 4);
        auto c = (origin2 >> (i + 4)) & 1;
        value |= c << i;
    }
}
