/**
 * @file error.h
 * @brief Error classes.
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_INCLUDES_TYPE_ERROR_H_
#define CPPDATABASE_INCLUDES_TYPE_ERROR_H_

#include <utility>
#include <string>

/**
 * @brief Error class.
 * @details This class is used to store the error information.
 */
class Error{
 public:

  /**
   * @brief Constructor of the Error class.
   * @details This constructor is used to create the Error object.
   */
  explicit Error(std::string msg, std::string error_type)
  : message(std::move(msg)), error_type(std::move(error_type)){}

  /**
   * @brief Get the error message.
   * @return The error message.
   */
  [[nodiscard]] std::string get_message() const {return error_type + ": " + message;}
 protected:
  std::string message;
 private:
  std::string error_type;
};

/**
 * @brief Deserialize error class.
 * @details This class is used to store the deserialize error information.
 */
class DeserializeError : public std::runtime_error {
 public:
  explicit DeserializeError(const std::string& msg): runtime_error("DeserializeError: " + msg) {}
};

/**
 * @brief Serialize error class.
 * @details This class is used to store the serialize error information.
 */
class SerializeError : public std::runtime_error {
 public:
  explicit SerializeError(const std::string& msg): runtime_error("SerializeError: " + msg) {}
};

/**
 * @brief File already exist error class.
 * @details This class is used to store the file error information.
 */
class AlreadyExist : public std::runtime_error {
 public:
  explicit AlreadyExist(const std::string& msg): runtime_error("AlreadyExist: " + msg) {}
};

/**
 * @brief File not found error class.
 * @details This class is used to store the file error information.
 */
class NotFound : public std::runtime_error {
 public:
  explicit NotFound(const std::string& msg): runtime_error("NotFound: " + msg) {}
};

/**
 * @brief Cannot convert error class.
 * @details This class is used to store the convert error information.
 */
class CannotConvert : public std::runtime_error {
 public:
  explicit CannotConvert(const std::string& msg): runtime_error("CannotConvert: " + msg) {}
};

/**
 * @brief Insert error class.
 * @details This class is used to store the insert error information.
 */
class InsertionError : public std::runtime_error {
 public:
  explicit InsertionError(const std::string& msg): runtime_error("InsertionError: " + msg) {}
};

#endif //CPPDATABASE_INCLUDES_TYPE_ERROR_H_
