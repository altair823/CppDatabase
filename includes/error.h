//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_TYPE_ERROR_H_
#define CPPDATABASE_INCLUDES_TYPE_ERROR_H_

#include <utility>
#include <mem_core.h>

class Error{
 public:
  explicit Error(std::string msg, std::string error_type)
  : message(std::move(msg)), error_type(std::move(error_type)){}
  [[nodiscard]] std::string get_message() const {return error_type + ": " + message;}
 protected:
  std::string message;
 private:
  std::string error_type;
};

class DeserializeError : public Error{
 public:
  DeserializeError(std::string msg, BinaryIndex previous_index)
  : Error(std::move(msg), "DeserializeError"), previous_index(previous_index) {}
  [[nodiscard]] BinaryIndex get_previous_index() const {return previous_index;}
 private:
  BinaryIndex previous_index;
};

class SerializeError : public Error {
 public:
  explicit SerializeError(std::string msg): Error(std::move(msg), "SerializeError") {}
};

class AlreadyExist : public Error {
 public:
  explicit AlreadyExist(std::string msg): Error(std::move(msg), "AlreadyExist") {}
};

class NotFound : public Error {
 public:
  explicit NotFound(std::string msg): Error(std::move(msg), "NotFound") {}
};

class CannotConvert : public Error {
 public:
  explicit CannotConvert(std::string msg): Error(std::move(msg), "CannotConvert") {}
};

class InsertionError : public Error {
 public:
  explicit InsertionError(std::string msg): Error(std::move(msg), "InsertionError") {}
};

#endif //CPPDATABASE_INCLUDES_TYPE_ERROR_H_
