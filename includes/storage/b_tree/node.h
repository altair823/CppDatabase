//
// Created by 김태현 on 2022/05/08.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_B_TREE_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_B_TREE_NODE_H_

#include <vector>
#include <result.h>
#include <field_data.h>
#include <record.h>

class IndexNode{
 public:
  IndexNode(const FieldData& new_key);

  [[nodiscard]] const std::vector<unsigned long long int> &get_pointer() const {return pointers;}
  [[nodiscard]] const std::vector<FieldDataShared> &get_keys() const {return keys;}

  Result<IndexNode*> push_back_key(const FieldData& key);
  Result<IndexNode*> insert_key(const FieldData& key, unsigned int index);
  Result<IndexNode*> delete_key(const FieldData& key);
  Result<IndexNode*> delete_key(unsigned int index);

  Result<IndexNode*> push_back_pointer(unsigned long long int pointer);
  Result<IndexNode*> insert_pointer(unsigned long long int pointer, unsigned int index);
  Result<IndexNode*> delete_pointer(unsigned long long int pointer);
  Result<IndexNode*> delete_pointer(unsigned int index);

  Result<unsigned int> search_index(const FieldData& key);
  Result<unsigned int> search_index(const unsigned long long int pointer);

  Result<Binary> serialize();

 private:
  std::vector<FieldDataShared> keys;
  std::vector<unsigned long long int> pointers;
  unsigned char key_count = 0;
};

#endif //CPPDATABASE_INCLUDES_STORAGE_B_TREE_NODE_H_
