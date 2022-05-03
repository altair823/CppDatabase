//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include <ostream>

enum class Type{
    None = 0,
    String = 1,
    DateTime = 2,
};

unsigned char type_to_4_bits(Type type);
Type bits_to_type(unsigned char bits);
std::ostream& operator<<(std::ostream& os, const Type& type);

class FieldType{
public:
    FieldType();
    explicit FieldType(Type field_type);
    virtual std::unique_ptr<unsigned char[]> serialize() = 0;
    virtual void deserialize(std::unique_ptr<unsigned char[]>&) = 0;


protected:
    Type field_type;
};



#endif //CPPDATABASE_FIELD_INTERFACE_H
