//
// Created by 김태현 on 2022/05/02.
//

#include <field_type.h>

FieldType::FieldType(Schema field_type) {
    this->field_type = field_type;
}

FieldType::FieldType() {
    this->field_type = Schema::None;
}
