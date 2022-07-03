//
// Created by 김태현 on 2022/05/05.
//

#include "schema.h"
#include "type.h"

#include <utility>


std::ostream &operator<<(std::ostream &os, const KeyType &key_type){
  switch (key_type) {
    case KeyType::NONE:os<<"none";
      break;
    case KeyType::PK:os<<"PK";
      break;
    case KeyType::FK:os<<"FK";
      break;
  }
  return os;
}

Schema::Schema(std::string name): schema_name(std::move(name)){ }

std::ostream &operator<<(std::ostream &os, const Schema &schema) {
  os << "schema_name: " << schema.schema_name << "\npk: " << schema.pk.get_name() << "(" <<schema.pk.get_type()<< ")" << "\n";
  if (!schema.fks.empty()) {
    os << "fk_names: \n";
    for (auto &n : schema.fks) {
      os << n.get_name() << "(" << n.get_type() << ")"  << "\n";
    }
  }
  if (!schema.other_fields.empty()) {
    os << "fields: \n";
    for (auto &f : schema.other_fields) {
      os << f << "(" << f.get_type() << ")" << "\n";
    }
  }
  return os;
}
bool Schema::operator==(const Schema &rhs) const {
  return schema_name == rhs.schema_name &&
      pk == rhs.pk &&
      fks == rhs.fks &&
      other_fields == rhs.other_fields;
}
bool Schema::operator!=(const Schema &rhs) const {
  return !(rhs == *this);
}
Result<FieldSchema, NotFound> Schema::get_fk(const std::string& field_name) const {
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const FieldSchema& field_schema){
    if (field_schema.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    return Ok(*fk_it);
  } else {
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  }
}
Result<FieldSchema, NotFound> Schema::get_field(const std::string &field_name) const {
  if (field_name == pk.get_name()){
    return Ok(pk);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const FieldSchema& field_schema){
    if (field_schema.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    return Ok(*fk_it);
  }
  auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const FieldSchema &field_schema) {
    if (field_schema.get_name() == field_name) {
      return true;
    } else {
      return false;
    }
  });
  if (it == other_fields.end()) {
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    return Ok(*it);
  }
}
template<typename Predicate>
Result<FieldSchema, NotFound> Schema::get_field(const std::string &field_name, Predicate predicate) const {
  if (field_name == pk.get_name()){
    return Ok(pk);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const FieldSchema& field_schema){
    if (field_schema.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    return Ok(*fk_it);
  }
  auto it = std::find_if(other_fields.begin(), other_fields.end(), predicate);
  if (it == other_fields.end()) {
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    return Ok(*it);
  }
}
Result<FieldSchema, NotFound> Schema::get_field(KeyType key_type, const std::string &field_name) const {
  if (KeyType::PK == key_type) {
    return Ok(pk);
  } else if (KeyType::FK == key_type && !field_name.empty()) {
    auto it = std::find_if(fks.begin(), fks.end(), [&](const FieldSchema& field_schema){
      if (field_schema.get_name() == field_name){
        return true;
      } else{
        return false;
      }
    });
    if (it == fks.end()){
      return Err(NotFound("Cannot found field " + field_name + " in schema!"));
    } else {
      return Ok(*it);
    }
  } else if (KeyType::NONE == key_type && !field_name.empty()) {
    auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const FieldSchema &field_schema) {
      if (field_schema.get_name() == field_name) {
        return true;
      } else {
        return false;
      }
    });
    if (it == other_fields.end()){
      return Err(NotFound("Cannot found field " + field_name + " in schema!"));
    } else {
      return Ok(*it);
    }
  }
  return Err(NotFound("Cannot found field " + field_name + " in schema!"));
}
Result<KeyType, NotFound> Schema::get_field_key_type(const std::string &field_name) const {
  if (pk.get_name() == field_name){
    return Ok(KeyType::PK);
  }
  auto fk_it = std::find_if(fks.begin(), fks.end(), [&](const FieldSchema& field_schema){
    if (field_schema.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (fk_it != fks.end()){
    return Ok(KeyType::FK);
  }
  auto it = std::find_if(other_fields.begin(), other_fields.end(), [&](const FieldSchema& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it == other_fields.end()){
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    return Ok(KeyType::NONE);
  }
}
SchemaBuilder::SchemaBuilder(std::string schema_name): schema_name(std::move(schema_name)), is_pk_exist(false) {}

Result<SchemaBuilder*, AlreadyExist> SchemaBuilder::set_field(TypeKind type, const std::string& name, KeyType key_type) {
  switch (key_type) {
    case KeyType::PK:
      if (is_pk_exist) {
        return Err(AlreadyExist("The PK is already set"));
      }
      pk = FieldSchema(type, name);
      is_pk_exist = true;
      return Ok(this);
    case KeyType::FK:
      for (auto& fk: fks){
        if (fk.get_name() == name){
          return Err(AlreadyExist("Field name \"" + name + "\" is already exist!"));
        }
      }
      fks.emplace_back(type, name);
      std::sort(fks.begin(), fks.end());
      return Ok(this);
    case KeyType::NONE:
      for (auto &field : other_fields) {
        if (field.get_name() == name) {
          return Err(AlreadyExist("Field name \"" + name + "\" is already exist!"));
        }
      }
      this->other_fields.emplace_back(type, name);
      std::sort(other_fields.begin(), other_fields.end());
      return Ok(this);
  }
}

Result<SchemaShared, NotFound> SchemaBuilder::build(){
  if (!is_pk_exist){
    return Err(NotFound("The PK is not set!"));
  }
  SchemaShared new_schema = std::make_shared<Schema>(schema_name);
  new_schema->pk = pk;
  new_schema->fks.assign(fks.begin(), fks.end());
  new_schema->other_fields.assign(other_fields.begin(), other_fields.end());

  return Ok(new_schema);
}
bool FieldSchema::operator<(const FieldSchema &rhs) const {
  return name < rhs.name;
}
bool FieldSchema::operator>(const FieldSchema &rhs) const {
  return rhs < *this;
}
bool FieldSchema::operator<=(const FieldSchema &rhs) const {
  return !(rhs < *this);
}
bool FieldSchema::operator>=(const FieldSchema &rhs) const {
  return !(*this < rhs);
}
