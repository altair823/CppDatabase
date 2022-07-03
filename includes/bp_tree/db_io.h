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

using Offset_t = long long int;

enum class NodeType {
  None, // 0
  IndexNode, // 1
  DataNode, // 2
};

class DBPointer : public Serializable {
 public:
  DBPointer(): offset(0), length(0) {}
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

NodeType byte_to_node_type(Byte byte);

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
