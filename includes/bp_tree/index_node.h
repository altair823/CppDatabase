/**
 * @file index_node.h
 * @brief Index node class
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */


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

/**
 * @brief IndexNode class
 *
 */
class IndexNode;

using IndexNodeUnique = std::unique_ptr<IndexNode>;

/**
 * @brief IndexNodeFactory class
 *
 */
class IndexNodeFactory {
 public:
  /**
   * @brief Create a Index Node object.
   * @param schema Schema object for this Index Node.
   * @param key_field_name PK field name for this Index Node.
   * @param keys Vector of Keys for this Index Node.
   * @param pointers Vector of Pointers for this Index Node.
   * @return IndexNodeUnique pointer.
   * @throw std::runtime_error If the key field is not the primary key.
   * @throw std::runtime_error If the key field is not the same type as the primary key.
   */
  static IndexNodeUnique create(const SchemaShared& schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers);

  /**
   * @brief Create a Index Node object.
   * @param schema Schema object for this Index Node.
   * @param key_field_name PK field name for this Index Node.
   * @return IndexNodeUnique pointer.
   * @throw std::runtime_error If the key field is not the primary key.
   * @throw std::runtime_error If the key field is not the same type as the primary key.
   */
  static IndexNodeUnique create(const SchemaShared& schema, std::string key_field_name);
};

class IndexNode : public Serializable {
 public:
  /**
   * @brief Get the key of the given index.
   * @param index Index of the key.
   * @return Key object.
   */
  [[nodiscard]] Key get_key(int index) const { return keys[index]; }

  /**
   * @brief Get the IndexNode pointer of the given index.
   * @param index Index of the pointer.
   * @return DBPointer object.
   */
  [[nodiscard]] IndexNodeUnique get_index_child(int index) const;

  /**
   * @brief Get the DataNode pointer of the given index.
   * @param index Index of the pointer.
   * @param record_file Path of the record file.
   * @return DBPointer object.
   */
  [[nodiscard]] DataNodeShared get_data_child(int index, std::filesystem::path record_file) const;

  /**
   * @brief Get whether this IndexNode is a leaf node.
   * @return True if this IndexNode is a leaf node.
   */
  [[nodiscard]] bool is_leaf() const { return is_node_leaf; }

  /**
   * @brief Set whether this IndexNode is a leaf node.
   * @param is_leaf Whether this IndexNode is a leaf node.
   */
  void set_leaf(bool is_leaf) { is_node_leaf = is_leaf; }

  /**
   * @brief Get the number of keys in this IndexNode.
   * @return Number of keys.
   */
  [[nodiscard]] int get_key_count() const { return (int)keys.size(); }

  /**
   * @brief Get the number of pointers in this IndexNode.
   * @return Number of pointers.
   */
  [[nodiscard]] int get_pointer_count() const { return (int)pointers.size(); }

  /**
   * @brief Insert a key into this IndexNode.
   * @param index Index to insert the key.
   * @param key Key to insert.
   */
  void insert_key(int index, const Key& key);

  /**
   * @brief Insert a pointer into this IndexNode.
   * @param index Index to insert the pointer.
   * @param pointer Pointer to insert.
   */
  void insert_pointer(int index, const DBPointer& pointer);

  /**
   * @brief Pushback a key into this IndexNode.
   * @param key Key to pushback.
   */
  void push_back_key(const Key& key);

  /**
   * @brief Pushback a pointer into this IndexNode.
   * @param pointer Pointer to pushback.
   */
  void push_back_pointer(const DBPointer& pointer);

  /**
   * @brief Remove a key from this IndexNode.
   * @param index Index to remove the key.
   */
  void erase_key(int index);

  /**
   * @brief Remove a pointer from this IndexNode.
   * @param index Index to remove the pointer.
   */
  void erase_pointer(int index);

  /**
   * @brief Search the given key in this IndexNode.
   * @param key Key to search.
   * @return Index of the found key.
   */
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

  /**
   * @brief Construct a new Index Node object.
   * @param schema Schema object for this Index Node.
   * @param key_field_name PK field name for this Index Node.
   */
  explicit IndexNode(SchemaShared schema, std::string key_field_name) : is_node_leaf(true), schema(std::move(schema)), key_field_name(key_field_name) {};

  /**
   * @brief Construct a new Index Node object.
   * @param schema Schema object for this Index Node.
   * @param key_field_name PK field name for this Index Node.
   * @param keys Vector of Keys for this Index Node.
   * @param pointers Vector of Pointers for this Index Node.
   */
  IndexNode(SchemaShared schema, std::string key_field_name, std::vector<Key> keys, std::vector<DBPointer> pointers);
  std::vector<Key> keys;
  std::vector<DBPointer> pointers;
  bool is_node_leaf;
};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
