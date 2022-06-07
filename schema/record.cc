//
// Created by 김태현 on 2022/05/04.
//

#include <record.h>

#include <utility>
#include "type.h"

Record::Record(Schema schema): schema(std::move(schema)) {

}

Result<bool, AlreadyExist> Record::set_field(const std::shared_ptr<Type>& data, const std::string& field_name) {
  for (const auto& f: fields){
    if (f.get_name() == field_name){
      return Err(AlreadyExist("Cannot set a new variable. Same variable name is already exist! : " + field_name));
    }
  }
  fields.emplace_back(field_name, data);
  return Ok(true);
}

Result<FieldShared, NotFound> Record::get_field(const std::string& field_name) const {
  for (auto& field: fields){
    if (field.get_name() == field_name){
      return Ok(std::make_shared<Field>(field.get_name(), field.get_data()));
    }
  }
  return Err(NotFound("Cannot find the field name {" + field_name + "} in record!"));
}

Result<FieldShared, NotFound> Record::get_pk_field() const {
  auto pk_name = schema.get_field(KeyType::PK).unwrap().get_name();
  return Ok(get_field(pk_name).unwrap());
}

std::ostream &operator<<(std::ostream &os, const Record &record) {
  os << "Record{";
  for (auto& f: record.fields){
    os << "\n    type: " << record.get_schema().get_field_key_type(f.get_name()).unwrap() << ", domain: " << f.get_name() << ", field: " << *f.get_data();
  }
  os << "\n}";
  return os;
}

bool Record::operator==(const Record &rhs) const {
  if (this->fields.size() != rhs.fields.size()){
    return false;
  }

  return (std::all_of(fields.begin(), fields.end(), [&rhs](const Field& field){
    if (!rhs.get_field(field.get_name()).unwrap() || *field.get_data() != *rhs.get_field(field.get_name()).unwrap()->get_data()){
      return false;
    } else{
      return true;
    }
  }));
}

BinaryUnique Record::serialize() const {
  auto binary = BinaryFactory::create(0);
  for (auto& f: fields){
    auto b = f.get_data()->serialize();
    binary = *binary + *b;
  }
  return binary;
}

Result<BinaryIndex, DeserializeError> Record::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  for (auto &field_schema: schema){
    auto type = byte_to_type(binary.read_mem(index, Location_in_byte::FirstFourBit));
    if (type != field_schema.get_type()){
      throw DeserializeError("Wrong binary! (maybe mismatched with schema)");
    }
    auto field = type_to_field(type).unwrap();
    index = field->deserialize(binary, index).unwrap();
    fields.emplace_back(field_schema.get_name(), field);
  }
  return Ok(index);
}

Result<TypeShared, CannotConvert> type_to_field(TypeKind type){
  switch (type) {
    case TypeKind::NONE:return Err(CannotConvert("None type error!"));
    case TypeKind::STRING:return Ok(TypeShared(new String()));
    case TypeKind::DATETIME:return Ok(TypeShared(new DateTime()));
    case TypeKind::DATE:break;
    case TypeKind::TIME:break;
    case TypeKind::CHAR:break;
    case TypeKind::SHORT:break;
    case TypeKind::INT:return Ok(TypeShared(new Int));
    case TypeKind::BIGINT:break;
    case TypeKind::FLOAT:break;
    case TypeKind::DOUBLE:break;
    case TypeKind::BIT:break;
    case TypeKind::BLOB:break;
    default: return Err(CannotConvert("Wrong type error!"));
  }
  return Err(CannotConvert("Wrong type error!"));
}
