//
// Created by 김태현 on 2022/05/04.
//

#include <record.h>

#include <utility>
#include "type.h"

Record::Record(SchemaShared _schema) : schema(std::move(_schema)) {
  pk = FieldFactory::create(schema->get_pk());
  for (auto& fk: schema->get_fks()){
    fks.insert(FieldFactory::create(fk));
  }
  for (auto& f: schema->get_other_fields()){
    other_fields.insert(FieldFactory::create(f));
  }
}

bool Record::set_field(const TypeShared& data, const std::string& field_name) {
  if (field_name == schema->get_pk().get_name()){
    pk->set_data(data);
    return true;
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(),
                            [&](const FieldShared& field){
    if (field->get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    (*fk_it)->set_data(data);
    return true;
  }

  auto it = std::find_if(other_fields.begin(), other_fields.end(),
                         [&](const FieldShared& field){
    if (field->get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it != other_fields.end()){
    (*it)->set_data(data);
    return true;
  } else {
    return false;
  }
}

Result<FieldShared, NotFound> Record::get_field(const std::string& field_name) const {
  auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const FieldShared& field){
    if (field->get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it != other_fields.end()){
    return Ok(*it);
  }
  if (pk->get_name() == field_name){
    return Ok(pk);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const FieldShared& field){
    if (field->get_name() == field_name){
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

Result<FieldShared, NotFound> Record::get_pk_field() const {
  return Ok(pk);
}

std::ostream &operator<<(std::ostream &os, const Record &record) {
  os << "Record{";
  os << "\n    type: pk, name: " << record.pk->get_name() << ", field: " << *record.pk->get_data();
  for (auto& fk: record.fks){
    os << "\n    type: fk, name: " << fk->get_name() << ", field: " << *fk->get_data();
  }
  for (auto& f: record.other_fields){
    os << "\n    type: none, name: " << f->get_name() << ", field: " << *f->get_data();
  }
  os << "\n}";
  return os;
}

bool Record::operator==(const Record &rhs) const {
  if (*pk != *rhs.pk
  || this->other_fields.size() != rhs.other_fields.size()
  || this->fks.size() != rhs.fks.size()
  || !std::equal(this->other_fields.begin(), this->other_fields.end(), rhs.other_fields.begin(), field_comparator)
  || !std::equal(fks.begin(), fks.end(), rhs.fks.begin(), field_comparator)){
    return false;
  }
  return true;
}

BinaryUnique Record::serialize() const {
  if (fks.size() > 256){
    throw SerializeError("Too many FKs(over 256)!");
  }
  auto binary = BinaryFactory::create(0);
  binary = *binary + *(pk->serialize());
  for (auto& fk: fks){
    binary = *binary + *(fk->serialize());
  }
  for (auto& f: other_fields){
    auto b = f->get_data()->serialize();
    binary = *binary + *b;
  }
  return binary;
}

Result<BinaryIndex, DeserializeError> Record::deserialize(const Binary &binary, BinaryIndex begin) {
  BinaryIndex index = begin;
  index = pk->deserialize(binary, index).unwrap();
  for (auto& fk: fks){
    index = fk->deserialize(binary, index).unwrap();
  }
  for (auto& f: other_fields){
    index = f->deserialize(binary, index).unwrap();
  }
  return Ok(index);
}
bool field_comparator(const FieldShared& fk1, const FieldShared& fk2) {
  return *fk1 == *fk2;
}
