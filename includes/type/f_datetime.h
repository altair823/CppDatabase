//
// Created by Administrator on 2022-05-01.
//

#ifndef CPPDATABASE_F_DATETIME_H
#define CPPDATABASE_F_DATETIME_H


#include <bitset>
#include <mem_core.h>
#include <field_type.h>
#include <ostream>
#include <iostream>


class DateTime : public FieldType{
public:
    unsigned char f_year; // 8bits
    unsigned char f_month; // 4
    unsigned char f_day; // 8
    unsigned char f_hour; // 8
    unsigned char f_min; // 8
    unsigned char f_sec; // 8

    DateTime();

    bool operator==(const DateTime &rhs) const;

    std::unique_ptr<unsigned char[]> serialize() override;
    void deserialize(std::unique_ptr<unsigned char[]>&) override;

    friend std::ostream &operator<<(std::ostream &os, const DateTime &time);

};


#endif //CPPDATABASE_F_DATETIME_H
