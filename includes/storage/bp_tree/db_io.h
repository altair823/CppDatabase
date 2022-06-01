//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_

#include <fstream>
#include <utility>
#include <mem_core.h>
#include <result.h>
#include <error.h>
#include <ostream>
#include <serializable.h>

#define OFFSET unsigned long long int

enum class NodeType {
  None,
  IndexNode,
  DataNode,
};

class DBPointer : public Serializable{
 public:
  DBPointer(): offset(0) {}
  DBPointer(std::string file, OFFSET offset) : file_name(std::move(file)), offset(offset) {}
  [[nodiscard]] BinaryUnique serialize() const override;
  bool operator==(const DBPointer &rhs) const;
  Result<BINARY_INDEX, DeserializeError> deserialize(const Binary &binary, BINARY_INDEX begin) override;
  friend std::ostream &operator<<(std::ostream &os, const DBPointer &pointer);

 private:
  std::string file_name;
  OFFSET offset;
};


#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
