//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_

#include <fstream>
#include <utility>
#include "index_node.h"
#include "data_node.h"
#include <mem_core.h>
#include <result.h>
#include <error.h>

#define OFFSET unsigned long long int

class DBPointer {
 public:
  DBPointer(std::string file, OFFSET offset) : file_name(std::move(file)), offset(offset) {}
  Binary serialize();
  Result<BINARY_INDEX, DeserializeError> deserialize(BinaryRef binary, BINARY_INDEX start_index);
 private:
  std::string file_name;
  OFFSET offset;
};

template <typename Key, typename Value>
class DTO {
 public:
  IndexNodeUnique<Key, DBPointer> get_index_node(DBPointer db_pointer){

  };
  DataNodeUnique<Key, DBPointer> get_data_node(DBPointer db_pointer){

  };

};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
