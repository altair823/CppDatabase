//
// Created by 김태현 on 2022/06/05.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_
#define CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_

#include <serializable.h>

#include <schema.h>
#include <f_string.h>
#include <f_datetime.h>
#include <f_int.h>
#include <ostream>
#include <utility>

/**
 * @brief Field class
 * @details This class is used to define the field in DB schema.
 */
class Field;

typedef std::shared_ptr<Field> FieldShared;

/**
 * @brief Field Factory class
 * @details This class is used to create field.
 * @see Field
 */
class FieldFactory {
 public:
  /**
   * @brief Create field.
   * @details This function is used to create field with no value.
   * @return FieldShared
   */
  static FieldShared create();

  /**
   * @brief Create field.
   * @details This function is used to create field with schema.
   * @param field_schema Field schema.
   * @return FieldShared
   */
  static FieldShared create(const FieldSchema& field_schema);

  /**
   * @brief Create field.
   * @details This function is used to create field with name.
   * @param name Field name.
   * @return FieldShared
   */
  static FieldShared create(std::string name);

  /**
   * @brief Create field.
   * @details This function is used to create field with name and type.
   * @param name Field name.
   * @param type Field type.
   * @return
   */
  static FieldShared create(std::string name, TypeShared type);
};

/**
 * @brief TypeKind enum to TypeShared.
 * @param type TypeKind to be converted.
 * @return TypeShared if type is valid, CannotConvert result otherwise.
 */
Result<TypeShared, CannotConvert> type_to_field(TypeKind type);

class [[nodiscard]] Field : public Serializable{
 public:
  /**
   * @brief Get data of field.
   * @return TypeShared
   */
  [[nodiscard]] TypeShared get_data() const {return data;}

  /**
   * @brief Get name of field.
   * @return Name of field.
   */
  [[nodiscard]] std::string get_name() const {return name;}

  /**
   * @brief Set data of field.
   * @param new_data New data of field.
   */
  void set_data(TypeShared new_data) {data = std::move(new_data);}
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  friend std::ostream &operator<<(std::ostream &os, const Field &field);
  bool operator==(const Field &rhs) const;
  bool operator!=(const Field &rhs) const;
  bool operator<(const Field &rhs) const;
  bool operator>(const Field &rhs) const;
  bool operator<=(const Field &rhs) const;
  bool operator>=(const Field &rhs) const;
 private:
  friend class FieldFactory;
  Field(): data(nullptr) {}
  explicit Field(const FieldSchema& field_schema): name(field_schema.get_name()), data(type_to_field(field_schema.get_type()).unwrap()) {}
  explicit Field(std::string name): data(nullptr), name(std::move(name)) {}
  Field(std::string name, TypeShared data): data(std::move(data)), name(std::move(name)){}
  TypeShared data;
  std::string name;
};

bool field_shared_comparator(const FieldShared& field1, const FieldShared& field2);

#endif //CPPDATABASE_INCLUDES_SCHEMA_FIELD_H_
