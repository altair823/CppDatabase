//
// Created by 김태현 on 2022/05/04.
//

#include <schema.h>

#include <utility>

Schema::Schema() = default;

Result<bool> Schema::set_field(std::shared_ptr<FieldData> data, const std::string& field_name) {
  for (const auto& f: fields){
    if (f.name == field_name){
      return Err(false, "Cannot set a new variable. Same variable name is already exist! : " + field_name);
    }
  }
  fields.emplace_back(data, field_name);
  return Ok(true);
}

Result<FieldShared> Schema::get_field(const std::string& field_name) const {
  for (auto& field: fields){
    if (field.name == field_name){
      return Ok(std::make_shared<Field>(field.data, field.name));
    }
  }
  return Err((FieldShared)nullptr, "Cannot find the field name {" + field_name + "} in record!");
}

std::ostream &operator<<(std::ostream &os, const Schema &schema) {
  os << "fields: ";
  for (auto& f: schema.fields){
    os << "\n" << f.name << ": " << *f.data;
  }
  return os;
}

bool Schema::operator==(const Schema &rhs) const {
  if (this->fields.size() != rhs.fields.size()){
    return false;
  }

  for (auto& f: fields){
    if (!rhs.get_field(f.name).is_ok){
      return false;
    }
    if (*f.data != *rhs.get_field(f.name).ok->data){
      return false;
    }
  }
  return true;
}
int Schema::get_total_byte_size() const {
  int total_byte_size = 0;
  std::for_each(fields.begin(), fields.end(), [&](const Field& f) {
    total_byte_size += f.data->get_total_byte_size();
  });
  return total_byte_size;
}

Result<FieldDataShared> type_to_field(Type type){
  switch (type) {
    case Type::NONE:return Err((FieldDataShared)nullptr, "None type error!");
    case Type::STRING:return Ok(FieldDataShared(new String()));
    case Type::DATETIME:return Ok(FieldDataShared(new DateTime));
    case Type::DATE:break;
    case Type::TIME:break;
    case Type::CHAR:break;
    case Type::SHORT:break;
    case Type::INT:break;
    case Type::BIGINT:break;
    case Type::FLOAT:break;
    case Type::DOUBLE:break;
    case Type::BIT:break;
    case Type::BLOB:break;
    default: return Err((FieldDataShared)nullptr, "Wrong type error!");
  }
  return Err((FieldDataShared)nullptr, "Wrong type error!");
}

Binary serialize(Schema &schema) {
  int total_size = schema.get_total_byte_size();
  auto binary = std::make_unique<unsigned char[]>(total_size);
  int index = 0;
  for (auto& f: schema.fields){
    auto b = f.data->serialize();

    for (int i = 0; i < b.length; i++) {
      set_mem(binary[index], b.data[i]);
      index++;
    }
  }
  return {std::move(binary), total_size};
}

SchemaUnique deserialize(BinaryRef binary, std::vector<std::string> &field_names) {
  auto schema = std::make_unique<Schema>();
  int name_index = 0;
  for (int index = 0; index < binary.length && name_index < field_names.size(); ){
    unsigned char type = 0;
    read_mem(binary.data[index], type, Location_in_byte::First);
    auto field = type_to_field(bits_to_type(type)).ok;
    auto index_before = field->deserialize(binary, index).ok;
    index = index_before;
    schema->set_field(field, field_names[name_index++]);
  }
  return schema;
}