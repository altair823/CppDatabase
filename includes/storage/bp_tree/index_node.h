//
// Created by 김태현 on 2022/05/27.
//

#ifndef CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
#define CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_

#include <memory>
#include <vector>

#define LEAF true
#define NOT_LEAF false

template <typename Key, typename Pointer>
class RawIndexNode;

template <typename Key, typename Pointer>
using IndexNodeUnique = std::unique_ptr<RawIndexNode<Key, Pointer>>;

template <typename Key, typename Pointer>
class IndexNode {
 public:
  static IndexNodeUnique<Key, Pointer> create(std::vector<Key> keys, std::vector<Pointer> pointers);
};

template <typename Key, typename Pointer>
class RawIndexNode {
 public:
  [[nodiscard]] Key get_key(int index) const {return keys[index];}
  [[nodiscard]] Pointer get_pointer(int index) const {return pointers[index];}
  [[nodiscard]] bool is_leaf() const {return is_node_leaf;}
  [[nodiscard]] int get_key_count() const {return keys.size();}
  [[nodiscard]] int get_pointer_count() const {return pointers.size();}
 private:
  friend class IndexNode<Key, Pointer>;
  RawIndexNode(std::vector<Key> keys, std::vector<Pointer> pointers);
  std::vector<Key> keys;
  std::vector<Pointer> pointers;
  bool is_node_leaf;
};

template<typename Key, typename Pointer>
IndexNodeUnique<Key, Pointer> IndexNode<Key, Pointer>::create(std::vector<Key> keys, std::vector<Pointer> pointers) {
  return IndexNodeUnique<Key, Pointer>(keys, pointers);
}

template<typename Key, typename Pointer>
RawIndexNode<Key, Pointer>::RawIndexNode(std::vector<Key> keys, std::vector<Pointer> pointers)
: keys(std::move(keys)), pointers(std::move(pointers)), is_node_leaf(true) {

}
#endif //CPPDATABASE_INCLUDES_STORAGE_BP_TREE_INDEX_NODE_H_
