#include <f_datetime.h>
#include <field_type.h>
#include <iostream>

void print_bits(std::unique_ptr<unsigned char[]> &data, size_t length){
    for (auto i = 0; i < length; i++){
        auto a = data[i];
        std::bitset<8> x(a);
        std::cout << x << std::endl;
    }
}

int main (){
    DateTime d;
    d.f_year = 30;
    d.f_month = 10;
    d.f_day = 30;
    d.f_hour = 23;
    d.f_min = 58;
    d.f_sec = 34;

    // 00100001
    // 11101010
    // 00011110
    // 00010111
    // 00111010
    // 00100010

    auto a = d.serialize();
    print_bits(a, 6);

    std::cout<<d<<std::endl;

    auto c = DateTime();
    c.deserialize(a);
    std::cout<<c<<std::endl;


}

