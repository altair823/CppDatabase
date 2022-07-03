//
// Created by 김태현 on 2022/05/02.
//
#include <mem_core.h>



int get_byte_count(unsigned long long size) {
  int i = 0;
  while (size > 0) {
    size = size >> 8;
    i++;
  }
  return i;
}

int write_str_size_bits(BinaryUnique &binary, int size, int byte_count) {
  int b_index_after_header = 1;
  for (auto i = 0; i < byte_count; i++) {
    int data = size >> ((byte_count - (i + 1)) * 8);
    binary->set_mem(b_index_after_header, (char) data);
    b_index_after_header++;
  }
  return b_index_after_header;
}

std::vector<Byte> uint64_to_char_vec(std::uint64_t num){
  std::uint64_t a = (std::uint64_t)1 << 56;
  std::vector<Byte> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
  }
  return result;
}

std::vector<Byte> uint32_to_char_vec(std::uint32_t num){
  std::uint32_t a = (std::uint32_t)1 << 24;
  std::vector<Byte> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
  }
  return result;
}
std::vector<Byte> uint16_to_char_vec(std::uint16_t num){
  std::uint16_t a = (std::uint16_t)1 << 8;
  std::vector<Byte> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
  }
  return result;
}
std::vector<Byte> uint8_to_char_vec(std::uint8_t num){
  std::uint8_t a = 1;
  std::vector<Byte> result;
  while (a > 0) {
    auto t = num / a;
    result.push_back((char)t);
    a = a >> 8;
  }
  return result;
}
unsigned long long byte_vec_to_num(std::vector<Byte> char_vec) {
  int start_index = -1;
  for (auto i = 0; i < (int)char_vec.size(); i++){
    if (char_vec[i] != 0){
      start_index = i;
      break;
    }
  }
  if (start_index == -1){
    return 0;
  }
  unsigned long long result = 0;
  for (auto i = start_index; i < (int)char_vec.size(); i++){
    result += char_vec[i - start_index] << ((char_vec.size() - start_index - i - 1) * 8);
  }
  return result;
}

BinaryUnique BinaryFactory::create(BinaryIndex length) {
  std::unique_ptr<Binary> binary(new Binary(length));
  return binary;
}
BinaryUnique BinaryFactory::create(std::unique_ptr<Byte[]> byte_buffer, BinaryIndex length) {
  std::unique_ptr<Binary> binary(new Binary(std::move(byte_buffer), length));
  return binary;
}
BinaryUnique BinaryFactory::read(const std::filesystem::path& file, BinaryIndex offset, BinaryIndex length) {
  auto db_file = std::fstream(file, std::ios::binary | std::ios::in);
  if (db_file.fail()){
    throw NotFound("Cannot found DB file(name: " + file.string() + ")!");
  }
  db_file.seekg((long long)offset, db_file.beg);
  auto buffer = std::make_unique<Byte[]>(length);
  db_file.read((char *)buffer.get(), (long)length);
  auto binary = BinaryFactory::create(std::move(buffer), length);
  return binary;
}
Binary::Binary(BinaryIndex length): length(length) {
  if (length == 0){
    data = nullptr;
  } else {
    data = std::make_unique<Byte[]>(length);
  }
}
Binary::Binary(std::unique_ptr<Byte[]> byte_buffer, BinaryIndex length): length(length) {
  if (length == 0){
    data = nullptr;
  } else {
    data = std::move(byte_buffer);
  }
}
void Binary::set_mem(BinaryIndex index, Location_in_byte loc, const Byte &value) {
  verify_index(index);
  if (value > 15) {
    throw std::overflow_error("The value is 16 or higher! ");
  }
  auto v = std::bitset<4>(value);
  if (loc == Location_in_byte::FirstFourBit) {
    for (char i = 0; i < 4; i++) {
      data[index] |= v[i] << i + 4;
    }
  } else {
    for (char i = 0; i < 4; i++) {
      data[index] |= v[i] << i;
    }
  }
}
void Binary::set_mem(BinaryIndex index, const Byte &value) {
  verify_index(index);
  data[index] = value;
}
void Binary::set_mem(BinaryIndex index1, BinaryIndex index2, const Byte &value) {
  verify_index(index1);
  verify_index(index2);
  std::bitset<8> y(value);
  for (auto i = 0; i < 4; i++) {
    data[index1] |= y[i + 4] << i;
    data[index2] |= y[i] << i + 4;
  }
}
void Binary::verify_index(BinaryIndex index) const {
  if (index >= length){
    throw std::range_error("Binary index is out of range!");
  }
}
Byte Binary::read_mem(BinaryIndex index, Location_in_byte loc) const {
  Byte result = 0;
  if (loc == Location_in_byte::FirstFourBit) {
    for (auto i = 0; i < 4; i++) {
      auto b = (data[index] >> (i + 4)) & 1;
      result |= b << i;
    }
  } else {
    for (int i = 0; i < 4; i++) {
      auto b = (data[index] >> i) & 1;
      result |= b << i;
    }
  }
  return result;
}
Byte Binary::read_mem(BinaryIndex index) const {
  return data[index];
}
Byte Binary::read_mem(BinaryIndex index1, BinaryIndex index2) const {
  Byte result = 0;
  for (auto i = 0; i < 4; i++) {
    auto b = (data[index1] >> i) & 1;
    result |= b << (i + 4);
    auto c = (data[index2] >> (i + 4)) & 1;
    result |= c << i;
  }
  return result;
}
Result<Metadata, NotFound> Binary::save(const std::filesystem::path& file) {
  auto db_file = std::fstream(file, std::ios::binary | std::ios::app);
  if (db_file.fail()){
    return Err(NotFound("Cannot open target file!"));
  }
  BinaryIndex offset = db_file.tellg();
  db_file.write((char *)this->data.get(), (long)this->length);
  db_file.close();
  return Ok(Metadata(offset, this->length));
}
Result<Metadata, NotFound> Binary::save(const std::filesystem::path &file, BinaryIndex offset) {
  auto db_file = std::fstream(file, std::ios::binary | std::ios::in | std::ios::out);
  if (db_file.fail()){
    return Err(NotFound("Cannot open target file!"));
  }
  db_file.seekg(offset);
  db_file.write((char *)this->data.get(), this->length);
  db_file.close();
  return Ok(Metadata(offset, this->length));
}
BinaryUnique Binary::operator+(const Binary &binary_ref) const {
  auto result_binary = BinaryFactory::create(this->length + binary_ref.length);
  for (BinaryIndex i = 0; i < this->length; i++){
    result_binary->data[i] = this->data[i];
  }
  for (BinaryIndex i = 0; i < binary_ref.length; i++){
    result_binary->data[this->length + i] = binary_ref.data[i];
  }
  return result_binary;
}
std::ostream &operator<<(std::ostream &os, const Binary &binary) {
  for (BinaryIndex i = 0; i < binary.length; i++){
    for (int j = 7; j >= 0; j--){
      auto a = binary.data[i];
      auto b = (a >> j) & 1;
      os << b;
      if (j == 4){
        os << " ";
      }
    }
    os << "\n";
  }
  return os;
}