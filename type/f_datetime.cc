//
// Created by 김태현 on 2022/05/02.
//

#include <f_datetime.h>

BinaryUnique DateTime::serialize() const {
  auto result = BinaryFactory::create(6);
  result->set_mem(0, Location_in_byte::FirstFourBit, type_to_4_bits(field_type));
  result->set_mem(0, 1, f_year);
  result->set_mem(1, Location_in_byte::SecondFourBit, f_month);
  result->set_mem(2, f_day);
  result->set_mem(3, f_hour);
  result->set_mem(4, f_min);
  result->set_mem(5, f_sec);
  return result;
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

Result<BINARY_INDEX, DeserializeError> DateTime::deserialize(const Binary &binary, BINARY_INDEX begin) {
  unsigned char a = 0;
  a = binary.read_mem(begin, Location_in_byte::FirstFourBit);
  this->field_type = bits_to_type(a);
  this->f_year = binary.read_mem(begin, begin + 1);
  this->f_month = binary.read_mem(begin + 1, Location_in_byte::SecondFourBit);
  this->f_day = binary.read_mem(begin + 2);
  this->f_hour = binary.read_mem(begin + 3);
  this->f_min = binary.read_mem(begin + 4);
  this->f_sec = binary.read_mem(begin + 5);
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

