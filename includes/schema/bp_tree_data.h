//
// Created by 김태현 on 2022/06/08.
//

#ifndef CPPDATABASE_INCLUDES_SCHEMA_BP_TREE_DATA_H_
#define CPPDATABASE_INCLUDES_SCHEMA_BP_TREE_DATA_H_

#include <bp_tree/data.h>
#include <field.h>
#include <record.h>

class BPTreeData : public Data<Field, Record> {
 public:
  BPTreeData(const std::string& key_name, const Record& record);
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] std::shared_ptr<Field> get_key() const override;
  [[nodiscard]] std::shared_ptr<Record> get_value() const override;

 private:
  [[nodiscard]] bool eq(const Data<Field, Record> &rhs) const override;
  std::ostream &out(std::ostream &ostream) const override;

  FieldShared key;
  RecordShared value;
};

#endif //CPPDATABASE_INCLUDES_SCHEMA_BP_TREE_DATA_H_
