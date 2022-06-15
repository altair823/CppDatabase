//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_

#include <utility>

#include "data.h"
#include "index_node.h"
#include "data_node.h"

class BPTree {
 public:
  explicit BPTree(SchemaShared schema);
  bool is_empty();
  bool insert(Key key, Value value, bool to_override);
  Result<Value, NotFound> search(Key key);
  Result<std::vector<Value>, NotFound> search(Key begin, Key end);
  bool remove(Key key);
 private:
  IndexNodeUnique head;
  SchemaShared schema;
};

using BPTreeShared = std::shared_ptr<BPTree>;


#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_BP_TREE_H_
