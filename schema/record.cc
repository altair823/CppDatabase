//
// Created by 김태현 on 2022/05/04.
//

#include <record.h>

#include <utility>

Record::Record() = default;

Result<bool, AlreadyExist> Record::set_field(std::shared_ptr<FieldData> data, const std::string& field_name) {
  for (const auto& f: fields){
    if (f.name == field_name){
      return Err(AlreadyExist("Cannot set a new variable. Same variable name is already exist! : " + field_name));
    }
  }
  fields.emplace_back(data, field_name);
  return Ok(true);
}

Result<FieldShared, NotFound> Record::get_field(const std::string& field_name) const {
  for (auto& field: fields){
    if (field.name == field_name){
      return Ok(std::make_shared<Field>(field.data, field.name));
    }
  }
  return Err(NotFound("Cannot find the field name {" + field_name + "} in record!"));
}

std::ostream &operator<<(std::ostream &os, const Record &schema) {
  os << "fields: ";
  for (auto& f: schema.fields){
    os << "\n" << f.name << ": " << *f.data;
  }
  return os;
}

bool Record::operator==(const Record &rhs) const {
  if (this->fields.size() != rhs.fields.size()){
    return false;
  }

  for (auto& f: fields){
    if (!rhs.get_field(f.name).unwrap()){
      return false;
    }
    if (*f.data != *rhs.get_field(f.name).unwrap()->data){
      return false;
    }
  }
  return true;
}
int Record::get_total_byte_size() const {
  int total_byte_size = 0;
  std::for_each(fields.begin(), fields.end(), [&](const Field& f) {
    total_byte_size += f.data->get_total_byte_size();
  });
  return total_byte_size;
}

Binary Record::serialize() {
  int total_size = get_total_byte_size();
  auto binary = std::make_unique<unsigned char[]>(total_size);
  int index = 0;
  for (auto& f: fields){
    auto b = f.data->serialize();

    for (int i = 0; i < b.length; i++) {
      set_mem(binary[index], b.data[i]);
      index++;
    }
  }
  return {std::move(binary), total_size};
}

Result<FieldDataShared, CannotConvert> type_to_field(Type type){
  switch (type) {
    case Type::NONE:return Err(CannotConvert("None type error!"));
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
    default: return Err(CannotConvert("Wrong type error!"));
  }
  return Err(CannotConvert("Wrong type error!"));
}

RecordUnique deserialize(BinaryRef binary, std::vector<std::string> &field_names) {
  auto schema = std::make_unique<Record>();
  int name_index = 0;
  for (BINARY_INDEX index = 0; index < binary.length && name_index < field_names.size(); ){
    unsigned char type = 0;
    read_mem(binary.data[index], type, Location_in_byte::First);
    auto field = type_to_field(bits_to_type(type)).unwrap();
    auto index_before = field->deserialize(binary, index).unwrap();
    index = index_before;
    schema->set_field(field, field_names[name_index++]);
  }
  return schema;
}