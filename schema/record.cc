//
// Created by 김태현 on 2022/05/04.
//

#include <record.h>

#include <utility>
#include "type.h"

Record::Record(const Schema& schema) {
  pk = Field(schema.get_pk().get_name(), type_to_field(schema.get_pk().get_type()).unwrap());
  for (auto& fk: schema.get_fks()){
    fks.emplace_back(fk.get_name(), type_to_field(fk.get_type()).unwrap());
  }
  for (auto& fk: schema.get_fks()){
    fks.emplace_back(fk.get_name(), type_to_field(fk.get_type()).unwrap());
  }
  for (auto& f: schema.get_other_fields()){
    other_fields.emplace_back(f.get_name(), type_to_field(f.get_type()).unwrap());
  }
}

Result<bool, AlreadyExist> Record::set_field(const TypeShared& data, const std::string& field_name) {
  if (field_name == pk.get_name()){
    pk = Field(field_name, data);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const Field& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    fk_it->set_data(data);
  }

  auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const Field& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it != other_fields.end()){
    it->set_data(data);
  } else {
    Err(NotFound("Cannot find the field name {" + field_name + "} in record!"));
  }
  return Ok(true);
}

Result<Field, NotFound> Record::get_field(const std::string& field_name) const {
  auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const Field& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it != other_fields.end()){
    return Ok(*it);
  }
  if (pk.get_name() == field_name){
    return Ok(pk);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const Field& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    return Ok(*fk_it);
  }
  return Err(NotFound("Cannot find the field name {" + field_name + "} in record!"));
}

Result<Field, NotFound> Record::get_pk_field() const {
  return Ok(pk);
}

std::ostream &operator<<(std::ostream &os, const Record &record) {
  os << "Record{";
  os << "\n    type: pk, name: " << record.pk.get_name() << ", field: " << *record.pk.get_data();
  for (auto& fk: record.fks){
    os << "\n    type: fk, name: " << fk.get_name() << ", field: " << *fk.get_data();
  }
  for (auto& f: record.other_fields){
    os << "\n    type: none, name: " << f.get_name() << ", field: " << *f.get_data();
  }
  os << "\n}";
  return os;
}

bool Record::operator==(const Record &rhs) const {
  if (pk != rhs.pk || this->other_fields != rhs.other_fields || fks != rhs.fks){
    return false;
  } else {
    return true;
  }
}

BinaryUnique Record::serialize() const {
  if (fks.size() > 256){
    throw SerializeError("Too many FKs(over 256)!");
  }
  auto binary = BinaryFactory::create(0);
  binary = *binary + *(pk.serialize());
  for (auto& fk: fks){
    binary = *binary + *(fk.serialize());
  }
  for (auto& f: other_fields){
    auto b = f.get_data()->serialize();
    binary = *binary + *b;
  }
  return binary;
}

Result<BinaryIndex, DeserializeError> Record::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  index = pk.deserialize(binary, index).unwrap();
  for (auto& fk: fks){
    index = fk.deserialize(binary, index).unwrap();
  }
  for (auto& f: other_fields){
    index = f.deserialize(binary, index).unwrap();
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
