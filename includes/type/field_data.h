//
// Created by 김태현 on 2022/05/02.
//

#ifndef CPPDATABASE_FIELD_INTERFACE_H
#define CPPDATABASE_FIELD_INTERFACE_H

#include <memory>
#include <ostream>

#define _Ok true
#define Ok(ok) Result{_Ok, ok}
#define _Err false
#define Err(e, err) Result{_Err, e, err}

template<typename T>
struct Result{
  Result(bool is_ok, T ok, const std::string& err = ""): ok(ok), is_ok(is_ok){}
  bool is_ok = false;
  T ok;
  std::string err;
};


/**
 * Type of fields in DB schema.
 * The current number of types is up to 16.
 */
enum class Type {
  NONE = 0,
  STRING, // 1
  DATETIME, // 2
  DATE, // 3
  TIME, // 4
  CHAR, // 5
  SHORT, // 6
  INT, // 7
  BIGINT, // 8
  FLOAT, // 9
  DOUBLE, // 10
  BIT,
  BLOB,
};

struct Binary{
  std::unique_ptr<unsigned char[]> data;
  int length;

  Binary(std::unique_ptr<unsigned char[]> data, int length): data(std::move(data)), length(length) {}
};

typedef Binary& BinaryRef;

unsigned char type_to_4_bits(Type type);
Type bits_to_type(unsigned char bits);
std::ostream &operator<<(std::ostream &os, const Type &type);

class FieldData {
 public:
  FieldData();
  virtual ~FieldData() = default;
  explicit FieldData(Type field_type);
  virtual Binary serialize() = 0;
  virtual Result<int> deserialize(Binary &binary, int begin) = 0;
  virtual int get_total_byte_size() const = 0;

  bool operator==(const FieldData &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const FieldData &field_type);

  Type field_type;
 private:
  virtual bool eq(const FieldData &rhs) const = 0;
  virtual std::ostream& out(std::ostream&) const = 0;
};

typedef std::shared_ptr<FieldData> FieldDataShared;

#endif //CPPDATABASE_FIELD_INTERFACE_H
