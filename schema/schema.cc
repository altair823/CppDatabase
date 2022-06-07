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

Schema::Schema(std::string name): schema_name(std::move(name)), pk_index(UNDEFINED_INDEX) {

}
std::ostream &operator<<(std::ostream &os, const Schema &schema) {
  os << "schema_name: " << schema.schema_name << "\npk_name: " << schema.fields[schema.pk_index] << "\n";
  if (!schema.fk_indexes.empty()) {
    os << "fk_names: \n";
    for (auto &n : schema.fk_indexes) {
      os << schema.fields[n].get_name() << "\n";
    }
  }
  if (!schema.fields.empty()) {
    os << "fields: \n";
    for (auto &f : schema.fields) {
      os << f << "\n";
    }
  }
  return os;
}
bool Schema::operator==(const Schema &rhs) const {
  return schema_name == rhs.schema_name &&
      fields[pk_index] == rhs.fields[pk_index] &&
      fk_indexes.size() == rhs.fk_indexes.size() &&
      fields == rhs.fields;
}
bool Schema::operator!=(const Schema &rhs) const {
  return !(rhs == *this);
}
Result<FieldSchema, NotFound> Schema::get_field(const std::string &field_name) const {
  auto it = std::find_if(fields.begin(), fields.end(), [&](const FieldSchema &field_schema) {
    if (field_schema.get_name() == field_name) {
      return true;
    } else {
      return false;
    }
  });
  if (it != fields.end()) {
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    return Ok(*it);
  }
}
template<typename Predicate>
Result<FieldSchema, NotFound> Schema::get_field(const std::string &field_name, Predicate predicate) const {
  auto it = std::find_if(fields.begin(), fields.end(), predicate);
  if (it != fields.end()) {
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    return Ok(*it);
  }
}
Result<FieldSchema, NotFound> Schema::get_field(unsigned int index) const {
  return Ok(fields[index]);
}
Result<FieldSchema, NotFound> Schema::get_field(KeyType key_type, const std::string &field_name) const {
  if (KeyType::PK == key_type) {
    return Ok(fields[pk_index]);
  } else if (KeyType::FK == key_type && !field_name.empty()) {
    auto it = std::find_if(fk_indexes.begin(), fk_indexes.end(), [&](unsigned int index){
      if (fields[index].get_name() == field_name){
        return true;
      } else{
        return false;
      }
    });
    if (it != fk_indexes.end()){
      return Err(NotFound("Cannot found field " + field_name + " in schema!"));
    } else {
      return Ok(fields[*it]);
    }
  } else if (KeyType::NONE == key_type && !field_name.empty()) {
    auto it = std::find_if(fields.begin(), fields.end(), [&](const FieldSchema &field_schema) {
      if (field_schema.get_name() == field_name) {
        return true;
      } else {
        return false;
      }
    });
    if (it != fields.end()){
      return Err(NotFound("Cannot found field " + field_name + " in schema!"));
    } else {
      return Ok(*it);
    }
  }
  return Err(NotFound("Cannot found field " + field_name + " in schema!"));
}
Result<KeyType, NotFound> Schema::get_field_key_type(const std::string &field_name) const {
  auto it = std::find_if(fields.begin(), fields.end(), [&](const FieldSchema& field){
    if (field.get_name() == field_name){
      return true;
    } else {
      return false;
    }
  });
  if (it == fields.end()){
    return Err(NotFound("Cannot found field " + field_name + " in schema!"));
  } else {
    auto index = std::distance(fields.begin(), it);
    if (index == pk_index){
      return Ok(KeyType::PK);
    } else if (std::find(fk_indexes.begin(), fk_indexes.end(), index) != fk_indexes.end()) {
      return Ok(KeyType::FK);
    } else {
      return Ok(KeyType::NONE);
    }
  }
}
SchemaBuilder::SchemaBuilder(std::string schema_name): schema_name(std::move(schema_name)), pk_index(UNDEFINED_INDEX) {}

Result<SchemaBuilder*, AlreadyExist> SchemaBuilder::set_field(TypeKind type, const std::string& name, KeyType key_type) {
  switch (key_type) {
    case KeyType::NONE:
      for (auto &field : fields) {
        if (field.get_name() == name) {
          return Err(AlreadyExist("Field name \"" + name + "\" is already exist!"));
        }
      }
      this->fields.emplace_back(type, name);
      return Ok(this);
    case KeyType::PK:
      if (pk_index != UNDEFINED_INDEX) {
        return Err(AlreadyExist("The PK is already set"));
      }
      this->fields.emplace_back(type, name);
      this->pk_index = this->fields.size() - 1;
      return Ok(this);
    case KeyType::FK:this->fields.emplace_back(type, name);
      this->fk_indexes.push_back(this->fields.size() - 1);
      return Ok(this);
  }
}

Result<SchemaShared, NotFound> SchemaBuilder::build(){
  if (pk_index == UNDEFINED_INDEX){
    return Err(NotFound("The PK is not set!"));
  }
  else if (fields.empty()){
    return Err(NotFound("There are no fields to build!"));
  }
  SchemaShared new_schema = std::make_shared<Schema>(schema_name);
  new_schema->pk_index = pk_index;
  new_schema->fk_indexes.assign(fk_indexes.begin(), fk_indexes.end());
  new_schema->fields.assign(fields.begin(), fields.end());

  return Ok(new_schema);
}