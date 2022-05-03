//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_F_STRING_H
#define CPPDATABASE_F_STRING_H

#include <field_type.h>
#include <mem_core.h>
#include <cmath>
#include <ostream>
#include <iostream>

class String : public FieldType{
public:
    String();
    std::unique_ptr<unsigned char[]> serialize() override;
    void deserialize(std::unique_ptr<unsigned char[]>&) override;

    friend std::ostream &operator<<(std::ostream &os, const String &string);

    bool operator==(const String &rhs) const;

    std::string str;

private:
    static int get_byte_count(int size);
    static int write_str_size_bits(std::unique_ptr<unsigned char[]>& binary, int size, int byte_count);
};


#endif //CPPDATABASE_F_STRING_H
