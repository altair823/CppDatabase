//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include "schema.h"
#include <ostream>

class FieldType{
public:
    FieldType();
    explicit FieldType(Schema field_type);
    virtual std::unique_ptr<unsigned char[]> serialize() = 0;
    virtual void deserialization(std::unique_ptr<unsigned char[]>&) = 0;

protected:
    Schema field_type;
};



#endif //CPPDATABASE_FIELD_INTERFACE_H
