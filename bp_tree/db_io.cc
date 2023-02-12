#include "db_io.h"

#include <utility>

Result<BinaryIndex, DeserializeError> DBPointer::deserialize(const Binary &binary, BinaryIndex start_index) {
  BinaryIndex index = start_index;
  auto file_name_byte_count = binary.read_mem(index);
  index++;
  std::string temp;
  for (int i = 0; i < file_name_byte_count; i++){
    char b = (char)binary.read_mem(index);
    temp.push_back((char)b);
    index++;
  }
  file = std::filesystem::path(temp);
  auto offset_byte_count = binary.read_mem(index);
  index++;
  std::vector<Byte> valid_offset_bytes;
  for (int i = 0; i < offset_byte_count; i++){
    valid_offset_bytes.push_back(binary.read_mem(index));
    index++;
  }
  this->offset = (Offset_t)byte_vec_to_num(valid_offset_bytes);

  auto length_byte_count = binary.read_mem(index);
  index++;
  std::vector<Byte> valid_length_bytes;
  for (int i = 0; i < length_byte_count; i++){
    valid_length_bytes.push_back(binary.read_mem(index));
    index++;
  }
  this->length = (Offset_t) byte_vec_to_num(valid_length_bytes);
  return Ok(index);
}
BinaryUnique DBPointer::serialize() const {
  auto file_name_byte_count = (int) file.string().size();
  if (file_name_byte_count > 225){
    throw SerializeError("Too long file name!");
  }
  auto offset_to_chars = uint64_to_char_vec(offset);
  bool flag = false;
  std::vector<Byte> valid_offset_bytes;
  for (auto &f: offset_to_chars){
    if (flag || f != 0){
      valid_offset_bytes.push_back(f);
      flag = true;
    }
  }
  if (valid_offset_bytes.size() >= 256){
    throw SerializeError("File is too large!");
  }
  auto offset_byte_count = (Byte)valid_offset_bytes.size();
  auto length_to_chars = uint64_to_char_vec(length);
  flag = false;
  std::vector<Byte> valid_length_bytes;
  for (auto &l: length_to_chars){
    if (flag || l != 0){
      valid_length_bytes.push_back(l);
      flag = true;
    }
  }
  if (valid_length_bytes.size() >= 256){
    throw SerializeError("Length of pointer is too long!");
  }
  auto length_byte_count = (Byte)valid_length_bytes.size();

  auto binary = BinaryFactory::create(1 + file_name_byte_count + 1 + offset_byte_count + 1 + length_byte_count);
  int index = 0;
  binary->set_mem(index, file_name_byte_count);
  index++;
  for (auto& f: file.string()){
    binary->set_mem(index, f);
    index++;
  }
  binary->set_mem(index, offset_byte_count);
  index++;
  for (auto& offset_byte: valid_offset_bytes){
    binary->set_mem(index, offset_byte);
    index++;
  }
  binary->set_mem(index, length_byte_count);
  index++;
  for (auto& length_byte: valid_length_bytes){
    binary->set_mem(index, length_byte);
    index++;
  }
  return binary;
}
std::ostream &operator<<(std::ostream &os, const DBPointer &pointer) {
  os << "DBPointer{\nfile_name: " << pointer.file << ", offset: " << pointer.offset << ", length: " << pointer.length << "\n}";
  return os;
}
bool DBPointer::operator==(const DBPointer &rhs) const {
  return file == rhs.file &&
      offset == rhs.offset &&
      length == rhs.length;
}
bool DBPointer::operator!=(const DBPointer &rhs) const {
  return !(rhs == *this);
}

NodeType byte_to_node_type(Byte byte) {
  return static_cast<NodeType>(byte);
}

DBFile::DBFile(std::filesystem::path  file, uintmax_t threshold_file_size): original_file(std::move(file)), index(0), threshold_file_size(threshold_file_size) {
  current_file = add_index_to_filename(original_file, index);
}
std::filesystem::path DBFile::get_free_file() {
  while (exists(current_file) && file_size(current_file) > threshold_file_size){
    index++;
    current_file = add_index_to_filename(original_file, index);
  }
  return current_file;
}
std::filesystem::path DBFile::add_index_to_filename(const std::filesystem::path& file, unsigned int idx) {
    auto new_stem = original_file.stem().string() + std::to_string(idx);
    auto extension = original_file.extension();
    return original_file.parent_path() / std::filesystem::path(new_stem + extension.string());
}
std::ostream &operator<<(std::ostream &os, const DBFile &file) {
  os << "original_file: " << file.original_file << " current_file: " << file.current_file << " index: " << file.index
     << " threshold_file_size: " << file.threshold_file_size;
  return os;
}
