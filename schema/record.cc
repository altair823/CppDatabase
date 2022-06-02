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

  return (std::all_of(fields.begin(), fields.end(), [&rhs](const Field& field){
    if (!rhs.get_field(field.name).unwrap() || *field.data != *rhs.get_field(field.name).unwrap()->data){
      return false;
    } else{
      return true;
    }
  }));
}
int Record::get_total_byte_size() const {
  int total_byte_size = 0;
  std::for_each(fields.begin(), fields.end(), [&](const Field& f) {
    total_byte_size += f.data->get_total_byte_size();
  });
  return total_byte_size;
}

BinaryUnique Record::serialize() {
  int total_size = get_total_byte_size();
  auto binary = BinaryFactory::create(total_size);
  int index = 0;
  for (auto& f: fields){
    auto b = f.data->serialize();

    for (int i = 0; i < b->get_length(); i++) {
      binary->set_mem(index, b->read_mem(i));
      index++;
    }
  }
  return binary;
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

RecordUnique deserialize(Binary &binary, std::vector<std::string> &field_names) {
  auto schema = std::make_unique<Record>();
  int name_index = 0;
  for (BinaryIndex index = 0; index < binary.get_length() && name_index < field_names.size(); ){
    Byte type = binary.read_mem(index, Location_in_byte::FirstFourBit);
    auto field = type_to_field(byte_to_type(type)).unwrap();
    auto index_before = field->deserialize(binary, index).unwrap();
    index = index_before;
    schema->set_field(field, field_names[name_index++]);
  }
  return schema;
}