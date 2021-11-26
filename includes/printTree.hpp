/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printTree.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunlee <hyunlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:37:27 by hyunlee           #+#    #+#             */
/*   Updated: 2021/11/26 19:32:20 by hyunlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(PRINTTREE_HPP)
#define PRINTTREE_HPP

#include <iostream>

#include "RB_TreeNode.hpp"

#define C_RED "\033[0;31m"
#define C_RESET "\e[0m"

namespace ft {
template < typename T >
void printMap(RB_TreeNode< T >* node, int depth) {
  if (depth == 0) {
    std::cout << "// SHOW TREE //" << std::endl;
  }
  int tmp_depth = depth;
  if (node->is_nil()) {
    while (tmp_depth--) {
      std::cout << "    ";
    }
    std::cout << "nil" << std::endl;
    return;
  }
  while (tmp_depth--) {
    std::cout << "    ";
  }
  std::cout << (node->color ? C_RESET : C_RED)
            << (node->is_root() ? "R" : (node->is_leftchild() ? "L" : "R"))
            << " - key: " << node->value->first << C_RESET << std::endl;
  if (!node->leftChild->is_nil()) {
    printMap(node->leftChild, depth + 1);
  }
  if (!node->rightChild->is_nil()) {
    printMap(node->rightChild, depth + 1);
  }
  return;
}
}  // namespace ft

#endif  // PRINTTREE_HPP
