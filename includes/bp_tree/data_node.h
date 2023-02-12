/**
 * @file data_node.h
 * @brief DataNode class
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

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
#include <vector>

using Key = FieldShared;
using Value = RecordShared;

/**
 * @brief DataNode class
 * @details This class is a node of B+ tree.
 * @see IndexNode
 * @see IndexNodeFactory
 * @see BPTree
 * @see BPTreeFactory
 */
class DataNode;

using DataNodeShared = std::shared_ptr<DataNode>;

/**
 * @brief DataNodeFactory class
 * @details This class is a factory class for DataNode.
 * @see IndexNodeFactory
 * @see BPTreeFactory
 * @see DataNode
 * @see IndexNode
 */
class DataNodeFactory {
 public:
  /**
   * @brief Create a Data Node object.
   * @param schema Schema object for this Data Node.
   * @param key_field_name PK field name for this Data Node.
   * @param record_file RecordFile object for this Data Node.
   * @return DataNodeShared pointer.
   */
  static DataNodeShared create(const SchemaShared& schema, std::string key_field_name, std::filesystem::path record_file);
};

class DataNode : public Serializable{
 public:
  /**
   * @brief Get the key of the given index.
   * @param index Index of the key.
   * @return Key object.
   */
  [[nodiscard]] Key get_key(int index) const {return keys[index];}

  /**
   * @brief Get the value of the given index.
   * @param index Index of the value.
   * @return Value object.
   */
  [[nodiscard]] Value get_value(int index) const;

  /**
   * @brief Remove the key and value of the given index.
   * @param index Index of the key and value to remove.
   */
  void remove(int index);

  /**
   * @brief Get the number of data in this node.
   * @return Number of data.
   */
  [[nodiscard]] int get_data_count() const {return (int)data_pointer.size();}

  /**
   * @brief Set left sibling of this node to given data node.
   * @param data_node DataNode object to set as left sibling.
   */
  void set_left_sibling(DBPointer data_node) {left = std::move(data_node);};

  /**
   * @brief Set right sibling of this node to given data node.
   * @param data_node DataNode object to set as right sibling.
   */
  void set_right_sibling(DBPointer data_node) {right = std::move(data_node);};

  /**
   * @brief Get left sibling of this node.
   * @return DBPointer object that is left sibling of this node.
   */
  [[nodiscard]] DBPointer get_left_sibling() const {return left;}

  /**
   * @brief Get right sibling of this node.
   * @return DBPointer object that is right sibling of this node.
   */
  [[nodiscard]] DBPointer get_right_sibling() const {return right;}

  /**
   * @brief Return whether this node is full or not.
   * @return True if this node is full, false otherwise.
   */
  bool is_full() const {return is_data_node_full;}


  int in_begin() {return 0;}
  int in_end() {return (int)data_pointer.size() - 1;}

  /**
   * @brief Insert the given data to this node.
   * @param index Index to insert the data.
   * @param new_data Data to insert.
   */
  void insert(int index, Value new_data);

  /**
   * @brief Pushback the given data to this node.
   * @param new_data Data to pushback.
   */
  void push_back(Value new_data);

  /**
   * @brief Search the given key in this node.
   * @param key Key to search.
   * @return Index of the key if found, -1 otherwise.
   */
  [[nodiscard]] int search(const Key& key) const;

  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;

  bool operator==(const DataNode &rhs) const;
  bool operator!=(const DataNode &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const DataNode &node) {
    os << "DataNode{\nleft: " << node.left << ",\nright: " << node.right << ",\ndata: ";
    for (auto& k: node.keys){
      os << "\n" << k;
    }
    for (auto& d: node.data_pointer){
      os << "\n" << d;
    }
    return os;
  }
 private:
  friend class DataNodeFactory;

  /**
   * @brief Construct a new Data Node object.
   * @param schema Schema object for this Data Node.
   * @param key_field_name PK field name for this Data Node.
   * @param record_file Path of file that contains the record for this Data Node.
   */
  DataNode(SchemaShared schema, std::string key_field_name, std::filesystem::path record_file);

  bool is_data_node_full;
  SchemaShared schema;
  DBPointer left, right;
  std::vector<Key> keys;
  std::vector<DBPointer> data_pointer;
  std::string key_field_name;
  DBFile record_file;
};
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DATA_NODE_H_
