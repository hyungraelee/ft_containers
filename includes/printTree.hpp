#if !defined(PRINTTREE_HPP)
#define PRINTTREE_HPP

#include <iostream>

#include "RB_TreeNode.hpp"

namespace ft {
template < typename T >
void printMap(RB_TreeNode< T >* node, int depth) {
  if (depth == 0) {
    std::cout << "// SHOW TREE //" << std::endl;
  }
  if (node->is_nil()) {
    std::cout << "depth: " << depth
              << ", color: " << (node->color ? "BLACK\t" : "RED\t")
              << ", key: nil" << std::endl;
    return;
  }
  std::cout << "depth: " << depth << ",\tLR: "
            << (node->is_root() ? "Root"
                                : (node->is_leftchild() ? "Left" : "Right"))
            << ",\tcolor: " << (node->color ? "BLACK" : "RED")
            << ",\tkey: " << node->value->first << ", \tnode: " << node << std::endl;
  if (!node->leftChild->is_nil()) {
    printMap(node->leftChild, depth + 1);
  }
  if (!node->rightChild->is_nil()) {
    printMap(node->rightChild, depth + 1);
  }
  return;
}

template < typename T >
void printSet(RB_TreeNode< T >* node, int depth) {
  if (depth == 0) {
    std::cout << "// SHOW TREE //" << std::endl;
  }
  if (node->is_nil()) {
    std::cout << "depth: " << depth
              << ", color: " << (node->color ? "BLACK\t" : "RED\t")
              << ", key: nil" << std::endl;
    return;
  }
  std::cout << "depth: " << depth << ",\tLR: "
            << (node->is_root() ? "Root"
                                : (node->is_leftchild() ? "Left" : "Right"))
            << ",\tcolor: " << (node->color ? "BLACK" : "RED")
            << ",\tkey: " << *node->value << std::endl;
  if (!node->leftChild->is_nil()) {
    printSet(node->leftChild, depth + 1);
  }
  if (!node->rightChild->is_nil()) {
    printSet(node->rightChild, depth + 1);
  }
  return;
}
}  // namespace ft

#endif  // PRINTTREE_HPP
