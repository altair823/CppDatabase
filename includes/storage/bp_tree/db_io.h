//
// Created by 김태현 on 2022/05/28.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_

#include <fstream>
#include <utility>

#define OFFSET unsigned long long int

class DBPointer {
 public:
  DBPointer(std::string file, OFFSET offset) : file(std::move(file)), offset(offset) {}
 private:
  std::string file;
  OFFSET offset;
};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DB_IO_H_
