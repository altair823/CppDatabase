//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_

#include <fstream>
#include <utility>
#include <filesystem>
#include "mem_core.h"
#include "result.h"
#include "error.h"
#include <ostream>
#include "serializable.h"
#include <f_string.h>

using Offset_t = long long int;

/**
 * @brief Node type enum.
 * @details This enum is used to distinguish between IndexNode and DataNode.
 * @see IndexNode
 * @see DataNode
 */
enum class NodeType {
  None, // 0
  IndexNode, // 1
  DataNode, // 2
};

/**
 * @brief DBPointer class
 * @details This class is a pointer to a node in the B+ tree.
 */
class DBPointer : public Serializable {
 public:
  /**
   * @brief Construct a new DBPointer object.
   */
  DBPointer(): offset(0), length(0) {}
  /**
   * @brief Construct a new DBPointer object.
   * @param file Pointer file path.
   * @param offset Offset of the node.
   * @param length Length of the binary data of the node.
   */
  DBPointer(std::filesystem::path file, Offset_t offset, Offset_t length) : file(std::move(file)), offset(offset), length(length) {}
  [[nodiscard]] BinaryUnique serialize() const override;
  Result<BinaryIndex, DeserializeError> deserialize(const Binary &binary, BinaryIndex begin) override;
  [[nodiscard]] std::string get_file_name() const {return file;}
  [[nodiscard]] Offset_t get_offset() const {return offset;}
  [[nodiscard]] Offset_t get_length() const {return length;}
  bool operator==(const DBPointer &rhs) const;
  bool operator!=(const DBPointer &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const DBPointer &pointer);
 private:
  std::filesystem::path file;
  Offset_t offset;
  Offset_t length;
};

class DBFile{
 public:
  DBFile(std::filesystem::path file, uintmax_t threshold_file_size);
  std::filesystem::path get_free_file();
  friend std::ostream &operator<<(std::ostream &os, const DBFile &file);
 private:
  std::filesystem::path add_index_to_filename(const std::filesystem::path& file, unsigned int idx);
  std::filesystem::path original_file;
  std::filesystem::path current_file;
  unsigned int index;
  uintmax_t threshold_file_size;
};

NodeType byte_to_node_type(Byte byte);

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
