/**
 * @file bp_tree.h
 * @brief B+ Tree
 * @author 김태현
 * @date 2022/05/27
 * @version 1.0
 */

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_

#include <utility>
#include <filesystem>

#include "index_node.h"
#include "data_node.h"

/**
 * @brief B+ Tree class
 *
 */
class BPTree {
 public:
  /**
   * @brief Construct a new BPTree object
   * @param schema Schema object for this B+ Tree.
   * @param key_field_name Key field name for this B+ Tree.
   */
  explicit BPTree(SchemaShared schema, std::string key_field_name);

  /**
   * @brief Check if the B+ Tree is empty.
   * @return True if the B+ Tree is empty.
   */
  bool is_empty();

  /**
   * @brief Insert a new record into the B+ Tree.
   * @param key Primary key field of the record.
   * @param value Record object.
   * @param to_override If it is true, override the record if the key already exists.
   * @return True if the insertion is successful.
   * @throw std::runtime_error If the key field is not the primary key.
   * @throw std::runtime_error If the key field is not the same type as the primary key.
   */
  bool insert(const Key& key, const Value& value, bool to_override = false);

  /**
   * @brief Search the record with the given key.
   * @param key Primary key field of the record.
   * @return Result object with the record if the search is successful. If the search is failed, return a NotFound result.
   * @throw std::runtime_error If the key field is not the primary key.
   * @throw std::runtime_error If the key field is not the same type as the primary key.
   * @throw std::runtime_error If the B+ Tree is empty.
   */
  Result<Value, NotFound> search(Key key);

  /**
   * @brief Search the record with the given key in the range.
   * @param begin Begin key field of the range.
   * @param end End key field of the range.
   * @return Result object with the vector of records if the search is successful. If the search is failed, return a NotFound result.
   * @throw std::runtime_error If the key field is not the primary key.
   * @throw std::runtime_error If the key field is not the same type as the primary key.
   * @throw std::runtime_error If the B+ Tree is empty.
   * @throw std::runtime_error If the begin key is greater than the end key.
   * @throw std::runtime_error If the begin key is not in the range of the B+ Tree.
   * @throw std::runtime_error If the end key is not in the range of the B+ Tree.
   */
  Result<std::vector<Value>, NotFound> search(Key begin, Key end);

  /**
   * @brief Delete the record with the given key.
   * @param key Primary key field of the record.
   * @return True if the deletion is successful.
   */
  bool remove(Key key);

 private:
  IndexNodeUnique head;
  SchemaShared schema;
  std::string key_field_name;
  std::filesystem::path db_file;
  std::filesystem::path record_file;
};

using BPTreeShared = std::shared_ptr<BPTree>;


#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
