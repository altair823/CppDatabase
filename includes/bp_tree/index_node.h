//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_

#include <memory>
#include <utility>
#include <vector>
#include <ostream>
#include "serializable.h"
#include "error.h"
#include "db_io.h"
#include "data_node.h"

#define LEAF true
#define NOT_LEAF false

class IndexNode;

using IndexNodeUnique = std::unique_ptr<IndexNode>;

class IndexNodeFactory {
 public:
  static IndexNodeUnique create(const SchemaShared& schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers);
  static IndexNodeUnique create(const SchemaShared& schema, std::string key_field_name);
};

class IndexNode : public Serializable {
 public:
  [[nodiscard]] Key get_key(int index) const { return keys[index]; }
  [[nodiscard]] IndexNodeUnique get_index_child(int index) const;
  [[nodiscard]] DataNodeShared get_data_child(int index) const;
  [[nodiscard]] bool is_leaf() const { return is_node_leaf; }
  void set_leaf(bool is_leaf) { is_node_leaf = is_leaf; }
  [[nodiscard]] int get_key_count() const { return (int)keys.size(); }
  [[nodiscard]] int get_pointer_count() const { return (int)pointers.size(); }
  void insert_key(int index, const Key& key);
  void insert_pointer(int index, const DBPointer& pointer);
  void push_back_key(const Key& key);
  void push_back_pointer(const DBPointer& pointer);

  void erase_key(int index);
  void erase_pointer(int index);

  int search_key(const Key& key);


  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  bool operator==(const IndexNode &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const IndexNode &node) {
    os << "keys: ";
    for (auto &k : node.keys) {
      os << k << " ";
    }
    os << "\n pointers: ";
    for (auto &p : node.pointers) {
      os << p << " | ";
    }
    os << "\nis_node_leaf: " << node.is_node_leaf << "\n";
    return os;
  }

 private:
  friend class IndexNodeFactory;
  SchemaShared schema;
  std::string key_field_name;
  explicit IndexNode(SchemaShared schema, std::string key_field_name) : is_node_leaf(true), schema(std::move(schema)), key_field_name(key_field_name) {};
  IndexNode(SchemaShared schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers);
  std::vector<Key> keys;
  std::vector<DBPointer> pointers;
  bool is_node_leaf;
};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
