#if !defined(PRINTTREE_HPP)
#define PRINTTREE_HPP

#include <iostream>

#include "RB_TreeNode.hpp"

namespace ft {
template < typename T >
void printTree(RB_TreeNode<T>* node, int depth) {
    if (node->is_nil()) {
        std::cout << "depth: " << depth << ", color: " << (node->color ? "BLACK\t" : "RED\t") << ", key: nil" << std::endl;
        return ;
    }
    std::cout << "depth: " << depth << ",\tLR: " << (node->is_root() ? "Root" : (node->is_leftchild() ? "Left" : "Right")) << ",\tcolor: " << (node->color ? "BLACK" : "RED") << ",\tkey: " << node->value->first << std::endl;
    if (!node->leftChild->is_nil())
        printTree(node->leftChild, depth+1);
    if (!node->rightChild->is_nil())
        printTree(node->rightChild, depth+1);
    return ;
}
}  // namespace ft

#endif  // PRINTTREE_HPP
