//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_

#include <memory>
#include <algorithm>
#include "error.h"
#include "result.h"
#include <serializable.h>
#include "db_io.h"
#include <field.h>
#include <record.h>
#include <schema.h>
#include <ostream>
#include <utility>

using Key = FieldShared;
using Value = RecordShared;

class DataNode;

using DataNodeShared = std::shared_ptr<DataNode>;

class DataNodeFactory {
 public:
  static DataNodeShared create(const SchemaShared& schema, std::string key_field_name);
};

class DataNode : public Serializable{
 public:
  [[nodiscard]] Key get_key(int index) const {return data[index]->get_field(key_field_name).unwrap();}
  [[nodiscard]] Value get_value(int index) const {return data[index];}
  void remove(int index);
  [[nodiscard]] int get_data_count() const {return (int)data.size();}

  void set_left_sibling(DBPointer data_node) {left = std::move(data_node);};
  void set_right_sibling(DBPointer data_node) {right = std::move(data_node);};
  [[nodiscard]] DBPointer get_left_sibling() const {return left;}
  [[nodiscard]] DBPointer get_right_sibling() const {return right;}

  int in_begin() {return 0;}
  int in_end() {return (int)data.size() - 1;}

  void insert(int index, Value new_data);
  void push_back(Value new_data);
  [[nodiscard]] int search(const Key& key) const;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  bool operator==(const DataNode &rhs) const;
  bool operator!=(const DataNode &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const DataNode &node) {
    os << "DataNode{\nleft: " << node.left << ",\nright: " << node.right << ",\ndata: ";
    for (auto& d: node.data){
      os << "\n" << *d;
    }
    return os;
  }
 private:
  friend class DataNodeFactory;
  explicit DataNode(SchemaShared schema, std::string key_field_name) : schema(std::move(schema)), key_field_name(std::move(key_field_name)) {}

  SchemaShared schema;
  DBPointer left, right;
  std::vector<Value> data;
  std::string key_field_name;
};
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
