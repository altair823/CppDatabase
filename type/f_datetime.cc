//
// Created by 김태현 on 2022/05/02.
//

#include <f_datetime.h>

Binary DateTime::serialize() {
  auto result = std::make_unique<unsigned char[]>(6);
  set_mem(result[0], type_to_4_bits(field_type), Location_in_byte::First);
  set_mem(result[0], result[1], f_year);
  set_mem(result[1], f_month, Location_in_byte::Second);
  set_mem(result[2], f_day);
  set_mem(result[3], f_hour);
  set_mem(result[4], f_min);
  set_mem(result[5], f_sec);
  return {std::move(result), 6};
}

DateTime::DateTime() : FieldData(Type::DATETIME), f_year(0), f_month(0), f_day(0), f_hour(0), f_min(0), f_sec(0) {}

DateTime::DateTime(int year, char month, char day, char hour, char min, char sec)
: FieldData(Type::DATETIME), f_month(month), f_day(day), f_hour(hour), f_min(min), f_sec(sec) {
  if (year > 99){
    int y = year % 100;
    f_year = (unsigned char)y;
  } else{
    f_year = year;
  }
}

Result<int> DateTime::deserialize(Binary &binary, int begin) {
  unsigned char a = 0;
  read_mem(binary.data[begin], a, Location_in_byte::First);
  this->field_type = bits_to_type(a);
  read_mem(binary.data[begin], binary.data[begin + 1], this->f_year);
  read_mem(binary.data[begin + 1], this->f_month, Location_in_byte::Second);
  read_mem(binary.data[begin + 2], this->f_day);
  read_mem(binary.data[begin + 3], this->f_hour);
  read_mem(binary.data[begin + 4], this->f_min);
  read_mem(binary.data[begin + 5], this->f_sec);
  return Ok(begin + 6);
}

std::ostream &DateTime::out(std::ostream &os) const {
  os << "field_type: " << this->field_type << " f_year: " << (int) this->f_year << " f_month: " << (int) this->f_month
     << " f_day: " << (int) this->f_day << " f_hour: " << (int) this->f_hour << " f_min: " << (int) this->f_min
     << " f_sec: "
     << (int) this->f_sec;
  return os;
}

bool DateTime::eq(const FieldData &rhs) const {
  auto rh = dynamic_cast<const DateTime&>(rhs);
  return this->field_type == rh.field_type &&
      f_year == rh.f_year &&
      f_month == rh.f_month &&
      f_day == rh.f_day &&
      f_hour == rh.f_hour &&
      f_min == rh.f_min &&
      f_sec == rh.f_sec;
}
int DateTime::get_total_byte_size() const {
  return 6;
}

