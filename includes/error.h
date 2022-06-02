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

class DeserializeError : public std::runtime_error {
 public:
  explicit DeserializeError(const std::string& msg): runtime_error("DeserializeError: " + msg) {}
};

class SerializeError : public std::runtime_error {
 public:
  explicit SerializeError(const std::string& msg): runtime_error("SerializeError: " + msg) {}
};

class AlreadyExist : public std::runtime_error {
 public:
  explicit AlreadyExist(const std::string& msg): runtime_error("AlreadyExist: " + msg) {}
};

class NotFound : public std::runtime_error {
 public:
  explicit NotFound(const std::string& msg): runtime_error("NotFound: " + msg) {}
};

class CannotConvert : public std::runtime_error {
 public:
  explicit CannotConvert(const std::string& msg): runtime_error("CannotConvert: " + msg) {}
};

class InsertionError : public std::runtime_error {
 public:
  explicit InsertionError(const std::string& msg): runtime_error("InsertionError: " + msg) {}
};

#endif //CPPDATABASE_INCLUDES_TYPE_ERROR_H_
