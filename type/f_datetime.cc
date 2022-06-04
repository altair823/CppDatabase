//
// Created by 김태현 on 2022/05/02.
//

#include <f_datetime.h>

BinaryUnique DateTime::serialize() const {
  auto result = BinaryFactory::create(6);
  result->set_mem(0, Location_in_byte::FirstFourBit, type_to_4_byte(field_type));
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
    f_year = (Byte)y;
  } else{
    f_year = year;
  }
}

Result<BinaryIndex, DeserializeError> DateTime::deserialize(const Binary &binary, BinaryIndex begin) {
  auto type = byte_to_type(binary.read_mem(begin, Location_in_byte::FirstFourBit));
  if (type != field_type){
    return Err(DeserializeError("Wrong type of binary!"));
  }
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
BinaryIndex DateTime::get_total_byte_size() const {
  return 6;
}
bool DateTime::under(const FieldData &rhs) const {
  if (rhs.field_type != Type::DATETIME){
    throw CannotConvert("rhs is not Datetime!");
  }
  auto r = dynamic_cast<const DateTime&>(rhs);
  if (f_year >= r.f_year){
    return false;
  } else if (f_year == r.f_year) {
    if (f_month >= r.f_month) {
      return false;
    } else if (f_month == r.f_month) {
      if (f_day >= r.f_day) {
        return false;
      } else if (f_day == r.f_day) {
        if (f_hour >= r.f_hour) {
          return false;
        } else if (f_hour == r.f_hour) {
          if (f_min >= r.f_min) {
            return false;
          } else if (f_min == r.f_min) {
            if (f_sec < r.f_sec) {
              return true;
            } else{
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}

