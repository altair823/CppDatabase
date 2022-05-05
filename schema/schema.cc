//
// Created by 김태현 on 2022/05/05.
//

#include "schema.h"

#include <utility>
Schema::Schema(std::string name): schema_name(std::move(name)) {

}

std::ostream &operator<<(std::ostream &os, const Schema &schema) {
  os << "schema_name: " << schema.schema_name << "\npk_name: " << schema.pk_name << "\n";
  if (!schema.fk_names.empty()) {
    os << "fk_names: \n";
    for (auto &n : schema.fk_names) {
      os << n << "\n";
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
bool Schema::verify_record(Record* record) {
  if (record->fields.size() != this->fields.size()){
    return false;
  }
  for (int i = 0; i < fields.size(); i++){
    if (fields[i].type != record->fields[i].data->field_type || fields[i].name != record->fields[i].name){
      return false;
    }
  }
  return true;
}
SchemaBuilder::SchemaBuilder(std::string schema_name): schema_name(std::move(schema_name)) {}

Result<SchemaBuilder*> SchemaBuilder::set_field(Type type, const std::string& name, KeyType key_type) {
  if (key_type == KeyType::PK && !this->pk_name.empty()) {
    return Err((SchemaBuilderPtr)nullptr, "The PK is already set");
  } else if (key_type == KeyType::PK) {
    this->pk_name = name;
  } else if (key_type == KeyType::FK) {
    this->fk_names.push_back(name);
  }
  for (auto& field: fields){
    if (field.name == name){
      return Err((SchemaBuilderPtr) nullptr, "Field name \"" + name +"\" is already exist!");
    }
  }
  this->fields.emplace_back(type, name);
  return Ok(this);
}

Result<SchemaShared> SchemaBuilder::build(){
  if (pk_name.empty()){
    return Err((SchemaShared) nullptr, "The PK is not set!");
  }
  else if (fields.empty()){
    return Err((SchemaShared) nullptr, "There are no fields to build!");
  }
  SchemaShared new_schema = std::make_shared<Schema>(schema_name);
  new_schema->pk_name = pk_name;
  new_schema->fk_names.assign(fk_names.begin(), fk_names.end());
  new_schema->fields.assign(fields.begin(), fields.end());

  return Ok(new_schema);
}