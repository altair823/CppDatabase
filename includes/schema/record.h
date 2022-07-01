//
// Created by 김태현 on 2022/05/04.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
#define CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_

#include <utility>
#include <set>
#include <utility>
#include <type.h>
#include <mem_core.h>
#include <ostream>
#include <serializable.h>

#include "schema.h"

#include "field.h"

#include <error.h>



class [[nodiscard]] Record : public Serializable{
 public:
  explicit Record(SchemaShared schema);

  bool set_field(const TypeShared& data, const std::string& field_name);
  [[nodiscard]] Result<FieldShared, NotFound> get_field(const std::string& field_name) const;
  [[nodiscard]] Result<FieldShared, NotFound> get_pk_field() const;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] BinaryUnique serialize() const override;

  friend std::ostream &operator<<(std::ostream &os, const Record &schema);
  bool operator==(const Record &rhs) const;

  SchemaShared schema;
  FieldShared pk;
  std::set<FieldShared> fks;
  std::set<FieldShared> other_fields;
};

typedef std::shared_ptr<Record> RecordShared;

bool field_comparator(const FieldShared& fk1, const FieldShared& fk2);


#endif //CPPDATABASE_INCLUDES_SCHEMA_RECORD_H_
