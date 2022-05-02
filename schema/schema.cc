//
// Created by 김태현 on 2022/05/03.
//

#include <schema.h>

unsigned char schema_to_4_bits(Schema schema){
    return static_cast<unsigned char>(schema);
}

Schema bits_to_schema(unsigned char bits){
    return static_cast<Schema>(bits);
}

std::ostream& operator<<(std::ostream& os, const Schema& schema){
    switch (schema) {
        case Schema::None:
            os << "None";
            break;
        case Schema::Name:
            os << "Name";
            break;
        case Schema::CreatedDate:
            os << "CreatedDate";
            break;
        case Schema::EditedDate:
            os << "EditedDate";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}