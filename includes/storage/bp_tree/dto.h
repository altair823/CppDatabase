//
// Created by 김태현 on 2022/06/01.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DTO_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DTO_H_

#include "db_io.h"
#include "index_node.h"
#include "data_node.h"

template <typename Key, typename Value>
class DTO {
 public:
  IndexNodeUnique<Key> get_index_node(DBPointer db_pointer){

  };
  DataNodeUnique<Key, DBPointer> get_data_node(DBPointer db_pointer){

  };

};

#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_DTO_H_
