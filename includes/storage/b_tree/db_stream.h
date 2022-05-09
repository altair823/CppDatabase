//
// Created by 김태현 on 2022/05/09.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_B_TREE_DB_STREAM_H_
#define CPPDATABASE_INCLUDES_STORAGE_B_TREE_DB_STREAM_H_

#include <fstream>
#include "node.h"

template <typename Key, typename Pointer>
class DBStream{
 public:
  Result<std::string> write(IndexNode<Pointer> node){
    std::string file_name;
    for (auto& key: node.get_string_keys().unwrap()){
      file_name.append(key);
    }
    return Ok(file_name);
  }
};

#endif //CPPDATABASE_INCLUDES_STORAGE_B_TREE_DB_STREAM_H_
