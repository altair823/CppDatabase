//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_

#include <memory>
#include <algorithm>
#include "error.h"
#include "result.h"
#include "serializable.h"
#include "data.h"
#include "db_io.h"
#include <ostream>

class DataNode;

using DataNodeShared = std::shared_ptr<DataNode>;

class DataNodeFactory {
 public:
  static DataNodeShared create(std::unique_ptr<DataFactory> data_factory);
};

class DataNode : public Serializable{
 public:
using DataUnique = std::unique_ptr<Data>;
using KeyShared = std::shared_ptr<Key>;
using ValueShared = std::shared_ptr<Value>;
  [[nodiscard]] Key get_key(int index) const {return data[index]->get_key();}
  [[nodiscard]] Value get_value(int index) const {return data[index]->get_value();}
  DataUnique pop_data(int index) {return std::move(data[index]);}
  [[nodiscard]] int get_data_count() const {return (int)data.size();}

  void set_left_sibling(DBPointer data_node) {left = data_node;};
  void set_right_sibling(DBPointer data_node) {right = data_node;};
  DBPointer get_left_sibling() const {return left;}
  DBPointer get_right_sibling() const {return right;}

  int in_begin() {return 0;}
  int in_end() {return (int)data.size() - 1;}

  void insert(int index, DataUnique new_data);
  void push_back(DataUnique new_data);
  int search(Key key) const;

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
  explicit DataNode(std::unique_ptr<DataFactory> data_factory): data_factory(std::move(data_factory)) {}

  DBPointer left, right;
  std::vector<DataUnique> data;

  std::unique_ptr<DataFactory> data_factory;
};
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
