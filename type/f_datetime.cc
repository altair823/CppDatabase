//
// Created by 김태현 on 2022/05/02.
//

#include <f_datetime.h>

std::unique_ptr<unsigned char[]> DateTime::serialize() {
  auto result = std::make_unique<unsigned char[]>(6);
  set_mem(result[0], type_to_4_bits(field_type), Location_in_byte::First);
  set_mem(result[0], result[1], f_year);
  set_mem(result[1], f_month, Location_in_byte::Second);
  set_mem(result[2], f_day);
  set_mem(result[3], f_hour);
  set_mem(result[4], f_min);
  set_mem(result[5], f_sec);
  return result;
}

DateTime::DateTime() : FieldType(Type::DATETIME), f_year(0), f_month(0), f_day(0), f_hour(0), f_min(0), f_sec(0) {}

void DateTime::deserialize(std::unique_ptr<unsigned char[]> &binary_array) {
  unsigned char a;
  read_mem(binary_array[0], a, Location_in_byte::First);
  this->field_type = bits_to_type(a);
  read_mem(binary_array[0], binary_array[1], this->f_year);
  read_mem(binary_array[1], this->f_month, Location_in_byte::Second);
  read_mem(binary_array[2], this->f_day);
  read_mem(binary_array[3], this->f_hour);
  read_mem(binary_array[4], this->f_min);
  read_mem(binary_array[5], this->f_sec);
}

bool DateTime::operator==(const DateTime &rhs) const {
  return this->field_type == rhs.field_type &&
      f_year == rhs.f_year &&
      f_month == rhs.f_month &&
      f_day == rhs.f_day &&
      f_hour == rhs.f_hour &&
      f_min == rhs.f_min &&
      f_sec == rhs.f_sec;
}
std::ostream &DateTime::out(std::ostream &os) const {
  os << "field_type: " << this->field_type << " f_year: " << (int) this->f_year << " f_month: " << (int) this->f_month
     << " f_day: " << (int) this->f_day << " f_hour: " << (int) this->f_hour << " f_min: " << (int) this->f_min
     << " f_sec: "
     << (int) this->f_sec;
  return os;
}


