//
// Created by 김태현 on 2022/05/02.
//

#include <f_datetime.h>

std::unique_ptr<unsigned char[]> DateTime::serialize() {
    auto result = std::make_unique<unsigned char[]>(6);
    auto a = schema_to_4_bits(field_type);
    set_mem(result[0], a, First);
    set_mem(result[0], result[1], f_year);
    set_mem(result[1], f_month, Second);
    set_mem(result[2], f_day);
    set_mem(result[3], f_hour);
    set_mem(result[4], f_min);
    set_mem(result[5], f_sec);
    return result;
}

DateTime::DateTime(Schema field_type): FieldType(field_type), f_year(0), f_month(0), f_day(0), f_hour(0), f_min(0), f_sec(0) {}

DateTime::DateTime(): FieldType(Schema::None), f_year(0), f_month(0), f_day(0), f_hour(0), f_min(0), f_sec(0)  {}

std::ostream &operator<<(std::ostream &os, const DateTime &time) {
    os << "field_type: " << time.field_type << " f_year: " << (int)time.f_year << " f_month: " << (int)time.f_month
       << " f_day: " << (int)time.f_day << " f_hour: " << (int)time.f_hour << " f_min: " << (int)time.f_min << " f_sec: "
       << (int)time.f_sec;
    return os;
}

void DateTime::deserialization(std::unique_ptr<unsigned char[]>& binary_array) {
    unsigned char a;
    read_mem(binary_array[0], a, First);
    this->field_type = bits_to_schema(a);
    read_mem(binary_array[0], binary_array[1], this->f_year);
    read_mem(binary_array[1], this->f_month, Second);
    read_mem(binary_array[2], this->f_day);
    read_mem(binary_array[3], this->f_hour);
    read_mem(binary_array[4], this->f_min);
    read_mem(binary_array[5], this->f_sec);
}


