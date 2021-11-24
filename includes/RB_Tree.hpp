/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RB_Tree.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunlee <hyunlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:37:29 by hyunlee           #+#    #+#             */
/*   Updated: 2021/11/24 20:37:30 by hyunlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(RB_TREE_HPP)
#define RB_TREE_HPP

#include "RB_TreeIterator.hpp"
#include "printTree.hpp"

namespace ft {

template < typename T, typename Compare = ft::less< T >,
           typename Alloc = std::allocator< T >,
           typename NodeAlloc = std::allocator< ft::RB_TreeNode< T > > >
class RB_Tree {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef Compare value_comp;
  typedef Alloc alloc_type;
  typedef ft::RB_TreeNode< T > node_type;
  typedef NodeAlloc node_alloc_type;
  typedef ft::RB_TreeIterator< T, T*, T& > iterator;
  typedef ft::RB_TreeIterator< T, const T*, const T& > const_iterator;
  typedef size_t size_type;

 private:
  node_type* _root;
  node_type* _nil;
  size_type _size;
  value_comp _comp;
  node_alloc_type _node_alloc;

 public:
  RB_Tree()
      : _root(NULL),
        _nil(NULL),
        _size(0),
        _comp(value_comp()),
        _node_alloc(node_alloc_type()) {
    // nil로 루트 노드 세팅
    this->_nil = make_nil_node();
    this->_nil->color = BLACK;
    this->_nil->leftChild = this->_nil;
    this->_nil->rightChild = this->_nil;
    this->_nil->parent = this->_nil;
    this->_root = this->_nil;
  }

  RB_Tree(const RB_Tree& ot)
      : _root(NULL),
        _nil(NULL),
        _size(0),
        _comp(value_comp()),
        _node_alloc(node_alloc_type()) {
    this->_nil = make_nil_node();
    this->_nil->leftChild = this->_nil;
    this->_nil->rightChild = this->_nil;
    copy(ot);
    this->_nil->parent = get_back_node();
  }

  virtual ~RB_Tree() {
    clear();
    _node_alloc.destroy(this->_nil);
    _node_alloc.deallocate(this->_nil, 1);
  }

  RB_Tree& operator=(const RB_Tree& x) {
    if (this != &x) {
      copy(x);
    }
    return (*this);
  }

  bool is_empty_tree() const { return (this->_size == 0); }

  node_type* get_front_node() const {
    node_type* res = this->_root;

    while (res->leftChild && !res->is_nil() && !res->leftChild->is_nil()) {
      res = res->leftChild;
    }
    return (res);
  }

  // 이 node를 nil의 parent로 설정.
  node_type* get_back_node() const {
    node_type* res = this->_root;

    while (res->rightChild && !res->is_nil() && !res->rightChild->is_nil()) {
      res = res->rightChild;
    }
    return (res);
  }

  node_type* get_nil() const { return (this->_nil); }

  node_type* search(value_type val) const {
    node_type* result = this->_root;

    if (is_empty_tree()) {
      return (this->_nil);
    }
    while (!result->is_nil() &&
           (_comp(val, *result->value) || _comp(*result->value, val))) {
      if (_comp(val, *(result->value))) {
        result = result->leftChild;
      } else {
        result = result->rightChild;
      }
    }
    return (result);
  }

  size_type size() const { return (this->_size); }

  ft::pair< node_type*, bool > insert(const value_type& val,
                                      node_type* hint = NULL) {
    node_type* position = this->_root;
    node_type* inserted = make_val_node(val);

    // 비어있는 tree인 경우를 먼저 처리하고 리턴 시켜버림.
    if (is_empty_tree()) {
      return (insert_root(inserted));
    }

    // hint위치가 올바르지 않으면 root-node부터 탐색한다.
    if (hint != NULL && !hint->is_nil()) {
      position = check_hint(val, hint);
    }

    // 들어갈 자리 찾기.
    ft::pair< node_type*, bool > tmp = find_insert_position(position, inserted);
    if (!tmp.second) {
      return (tmp);
    }
    position = tmp.first;

    // 여기로 넘어오면 일단 insert 됨.
    ++this->_size;
    if (inserted->color == RED && inserted->parent->color == RED) {
      if (inserted->get_uncle_color() == BLACK) {
        Restructuring(inserted);
      } else {
        Recoloring(inserted);
      }
    }
    this->_nil->parent = get_back_node();
    return (ft::make_pair(inserted, true));
  }

  size_type erase(node_type* target) {
    if (this->_nil == target) {
      return 0;
    }
    // target-node를 왼쪽의 최대 또는 오른쪽의 최소 node와 위치 변경.
    // 원래 target-node 위치에는 대체할 node가 들어가있다.
    // 이후 target-node는 노드 자체를 삭제해야 함.
    // (위치 변경 후 target은 child에 non-nil 노드가 최대 1개)
    target = switch_to_erase(target);

    // Child는 target 노드의 non-nil child 우선 노드.
    node_type* child;
    if (target->rightChild->is_nil()) {
      child = target->leftChild;
    } else {
      child = target->rightChild;
    }
    // target-node가 RED인 경우, replace에서 정리 끝남.
    replace_node(target, child);
    if (target->color == BLACK) {
      if (child->color == RED) {
        // target-node가 BLACK인데 Child-node가 RED인 경우,
        // (이 경우만 child-node가 non-nil노드임)
        // 새로 바뀐 child-node의 색만 BLACK로 변경.
        child->color = BLACK;
      } else {
        // target-node가 BLACK인데 Child-node가 BLACK인 경우,
        // 이떄 사실상 target-node의 두 자식은 모두 nil이다. 즉, child-node도
        // nil. replace_node에서 child(nil)->parent를 상황에 맞게 설정해두었음.
        delete_case_1(child);
      }
    }
    delete_node(target);
    // nil->parent 다시 세팅.
    this->_nil->parent = get_back_node();
    return 1;
  }

  void clear(node_type* p = NULL) {
    if (p == NULL) {
      p = this->_root;
    }
    if (p->is_nil()) {
      return;
    }
    if (!p->leftChild->is_nil()) {
      clear(p->leftChild);
    }
    if (!p->rightChild->is_nil()) {
      clear(p->rightChild);
    }
    delete_node(p);
  }

  // k보다 크거나 같은 범위를 구하기 위함.
  node_type* lower_bound(const value_type& k) const {
    iterator itlow(get_front_node());

    while (!itlow.base()->is_nil() && _comp(*itlow, k)) {
      itlow++;
    }
    return (itlow.base());
  }

  // k보다 큰 범위를 구하기 위함.
  node_type* upper_bound(const value_type& k) const {
    iterator itup(lower_bound(k));

    if (!itup.base()->is_nil() && !_comp(*itup, k) && !_comp(k, *itup)) {
      itup++;
    }
    return (itup.base());
  }

  void copy(const RB_Tree& x) {
    clear();
    copy(x._root);
  }

  void copy(node_type* node) {
    if (node->is_nil()) {
      return;
    }
    insert(*node->value);
    if (!node->leftChild->is_nil()) {
      copy(node->leftChild);
    }
    if (!node->rightChild->is_nil()) {
      copy(node->rightChild);
    }
  }

  // void showMap() { ft::printMap(_root, 0); }
  // void showSet() { ft::printSet(_root, 0); }

 private:
  node_type* make_val_node(const value_type& val) {
    node_type* node;

    node = _node_alloc.allocate(1);
    _node_alloc.construct(node, node_type(val));
    return (node);
  }

  node_type* make_nil_node() {
    node_type* node;

    node = _node_alloc.allocate(1);
    _node_alloc.construct(node, node_type());
    return (node);
  }

  ft::pair< node_type*, bool > insert_root(node_type* inserted) {
    this->_root = inserted;
    this->_root->leftChild = this->_nil;
    this->_root->rightChild = this->_nil;
    this->_root->parent = this->_nil;
    this->_nil->parent = this->_root;
    this->_root->color = BLACK;
    ++this->_size;
    return (ft::make_pair(this->_root, true));
  }

  ft::pair< node_type*, bool > find_insert_position(node_type* position,
                                                    node_type* inserted) {
    while (!position->is_nil()) {
      if (_comp(*(inserted->value),
                *(position->value))) {  // position 기준 왼쪽으로
        if (position->leftChild->is_nil()) {
          // position의 leftChild 자리로 들어감.
          position->leftChild = inserted;
          inserted->parent = position;
          inserted->leftChild = this->_nil;
          inserted->rightChild = this->_nil;
          break;
        } else {
          position = position->leftChild;
        }
      } else if (_comp(*(position->value),
                       *(inserted->value))) {  // position 기준 오른쪽으로
        if (position->rightChild->is_nil()) {
          // position의 rightChild 자리로 들어감.
          position->rightChild = inserted;
          inserted->parent = position;
          inserted->leftChild = this->_nil;
          inserted->rightChild = this->_nil;
          break;
        } else {
          position = position->rightChild;
        }
      } else {
        return (ft::make_pair(position, false));
      }
    }
    return (ft::make_pair(position, true));
  }

  bool is_double_RED(node_type* child, node_type* parent) {
    return (child->color == RED && parent->color == RED);
  }

  // Double RED 발생 시, uncle이 RED인 경우.
  void Recoloring(node_type* node) {
    node_type* grand_parent = node->parent->parent;

    node->parent->color = BLACK;
    if (node->parent->is_leftchild()) {
      grand_parent->rightChild->color = BLACK;
    } else {
      grand_parent->leftChild->color = BLACK;
    }
    grand_parent->color = RED;

    if (grand_parent->is_root()) {
      grand_parent->color = BLACK;
    } else if (is_double_RED(grand_parent, grand_parent->parent)) {
      if (grand_parent->get_uncle_color() == RED) {
        Recoloring(grand_parent);
      } else {
        Restructuring(grand_parent);
      }
    }
  }

  /**
   *
   *  Double RED 발생 시, uncle이 BLACK인 경우.
   *  Restructuring 발생하는 4가지 경우 (_comp = less< T > 일 때)
   *  조건문 순서대로,
   *
   *  CASE #1
   *          3
   *      2
   *  1
   *  argument node - 1, RED node - 1 & 2
   *
   *  CASE #2
   *          3
   *  1
   *      2
   *  argument node - 2, RED node - 1 & 2
   *
   *  CASE #3
   *  1
   *      2
   *          3
   *  argument node - 3, RED node - 2 & 3
   *
   *  CASE #4
   *  1
   *          3
   *      2
   *  argument node - 2, RED node - 2 & 3
   *
   */
  void Restructuring(node_type* node) {
    node_type* grand_parent = node->parent->parent;
    node_type* top = node->parent->parent->parent;
    node_type *prevChild_A, *prevChild_B, *prevChild_C, *prevChild_D;
    node_type* order[3];

    if (_comp(*node->value, *node->parent->value) &&
        _comp(*node->parent->value, *node->parent->parent->value)) {
      order[0] = node;
      order[1] = node->parent;
      order[2] = node->parent->parent;
      prevChild_A = node->leftChild;
      prevChild_B = node->rightChild;
      prevChild_C = node->parent->rightChild;
      prevChild_D = node->parent->parent->rightChild;
    } else if (_comp(*node->parent->value, *node->value) &&
               _comp(*node->value, *node->parent->parent->value)) {
      order[0] = node->parent;
      order[1] = node;
      order[2] = node->parent->parent;
      prevChild_A = node->parent->leftChild;
      prevChild_B = node->leftChild;
      prevChild_C = node->rightChild;
      prevChild_D = node->parent->parent->rightChild;
    } else if (_comp(*node->parent->parent->value, *node->parent->value) &&
               _comp(*node->parent->value, *node->value)) {
      order[0] = node->parent->parent;
      order[1] = node->parent;
      order[2] = node;
      prevChild_A = node->parent->parent->leftChild;
      prevChild_B = node->parent->leftChild;
      prevChild_C = node->leftChild;
      prevChild_D = node->rightChild;
    } else {  // _comp(*node->parent->parent->value, *node->value) &&
      //            _comp(*node->value, *node->parent->value)
      order[0] = node->parent->parent;
      order[1] = node;
      order[2] = node->parent;
      prevChild_A = node->parent->parent->leftChild;
      prevChild_B = node->leftChild;
      prevChild_C = node->rightChild;
      prevChild_D = node->parent->rightChild;
    }
    if (grand_parent->is_leftchild()) {
      connect_left(top, order[1]);
    } else if (grand_parent->is_rightchild()) {
      connect_right(top, order[1]);
    } else if (grand_parent->is_nil()) {
      order[1]->parent = this->_nil;
    } else if (grand_parent->is_root()) {
      this->_root = order[1];
      this->_root->parent = this->_nil;
      this->_root->color = BLACK;
    }
    connect_left(order[1], order[0]);
    connect_right(order[1], order[2]);
    connect_left(order[0], prevChild_A);
    connect_right(order[0], prevChild_B);
    connect_left(order[2], prevChild_C);
    connect_right(order[2], prevChild_D);
    order[0]->color = RED;
    order[1]->color = BLACK;
    order[2]->color = RED;
  }

  void connect_left(node_type* parent, node_type* child) {
    parent->leftChild = child;
    child->parent = parent;
  }

  void connect_right(node_type* parent, node_type* child) {
    parent->rightChild = child;
    child->parent = parent;
  }

  void set_root(node_type* n) {
    node_type* tmp = n;

    while (!tmp->parent->is_nil()) {
      tmp = tmp->parent;
    }
    this->_root = tmp;
  }

  void rotate_left(node_type* p) {
    node_type* new_p = p->rightChild;

    replace_node(p, new_p);
    p->parent = new_p;
    p->rightChild = new_p->leftChild;
    p->rightChild->parent = p;
    new_p->leftChild = p;
    set_root(p);
  }

  void rotate_right(node_type* p) {
    node_type* new_p = p->leftChild;

    replace_node(p, new_p);
    p->parent = new_p;
    p->leftChild = new_p->rightChild;
    p->leftChild->parent = p;
    new_p->rightChild = p;
    set_root(p);
  }

  /**
   * *** Hint 쓰는 경우. (hint가 적절한 위치인 경우)
   * inserted value는 hint node의 right-sub-tree로 들어간다.
   * 공통 조건 :  hint < inserted value 인 경우.
   * 1) hint가 leftChild인 경우, inserted value < hint-parent 이면,
   * hint부터 탐색.
   * 2) hint가 rightChild인 경우, parent를 따라가다가 처음으로 leftChild인
   * 노드의 parent보다 작으면 hint부터 탐색.
   */
  node_type* check_hint(value_type val, node_type* hint) {
    if (_comp(*(hint->value), val)) {
      if (hint->is_leftchild() && _comp(val, *(hint->parent->value))) {
        return hint;
      } else if (hint->is_rightchild()) {
        node_type* tmp = hint->parent;

        while (tmp->is_rightchild()) {
          tmp = tmp->parent;
        }
        if (tmp->is_leftchild() && !_comp(val, *(tmp->parent->value))) {
          return (this->_root);
        }
        return hint;
      }
    }
    return (this->_root);
  }

  // target의 left가 있으면 left-sub-tree의 최댓값,
  // target의 left가 없으면 right-sub-tree의 최솟값을 target에 복사한다.
  // 복사해온 노드는 삭제해야 하므로 새로운 target로 쓰기위해 리턴함.
  node_type* switch_to_erase(node_type* target) {
    iterator tmp(target);
    node_type* n;
    alloc_type _alloc;

    // target노드가 tree 가지 끝의 노드인 경우.
    if (target->leftChild->is_nil() && target->rightChild->is_nil()) {
      return target;
    }

    if (!target->leftChild->is_nil()) {
      n = (--tmp).base();
    } else {
      n = (++tmp).base();
    }

    node_type* tmp_lc = target->leftChild;
    node_type* tmp_rc = target->rightChild;
    node_type* tmp_p = target->parent;
    Color tmp_c = target->color;

    target->leftChild = n->leftChild;
    if (!n->leftChild->is_nil()) {
      n->leftChild->parent = target;
    }
    target->rightChild = n->rightChild;
    if (!n->rightChild->is_nil()) {
      n->rightChild->parent = target;
    }
    if (target->is_leftchild()) {
      tmp_p->leftChild = n;
    } else if (target->is_rightchild()) {
      tmp_p->rightChild = n;
    }
    if (target == n->parent) {
      target->parent = n;
    } else {
      target->parent = n->parent;
    }
    target->color = n->color;
    if (n == tmp_lc) {
      n->leftChild = target;
    } else {
      n->leftChild = tmp_lc;
    }
    tmp_lc->parent = n;
    if (n == tmp_rc) {
      n->rightChild = target;
    } else {
      n->rightChild = tmp_rc;
    }
    tmp_rc->parent = n;
    if (n->is_leftchild()) {
      n->parent->leftChild = target;
    } else if (n->is_rightchild()) {
      n->parent->rightChild = target;
    }
    n->parent = tmp_p;
    if (n->parent->is_nil()) {
      this->_root = n;
    }
    n->color = tmp_c;

    return target;
  }

  node_type* sibling(node_type* node) {
    if (node == node->parent->leftChild)
      return node->parent->rightChild;
    else
      return node->parent->leftChild;
  }

  void replace_node(node_type* n, node_type* child) {
    child->parent = n->parent;
    if (n->is_root()) {
      return;
    }

    if (n->is_leftchild()) {
      n->parent->leftChild = child;
    } else if (n->is_rightchild()) {
      n->parent->rightChild = child;
    }
  }

  // n이 루트인경우 더이상 할 게 없음. (사실상 빈 트리가 됨)
  void delete_case_1(node_type* n) {
    if (!n->is_root()) {
      delete_case_2(n);
    }
  }

  // s: RED 인 경우, 여기서 한 작업으로 끝나지 않음.
  // 이때 s의 두 자식은 모두 nil이 아님. (BLACK의 갯수가 동일해야 하기 때문)
  // case_4 에서 마무리 될 예정.
  void delete_case_2(node_type* n) {
    node_type* s = sibling(n);

    if (s->color == RED) {
      n->parent->color = RED;
      s->color = BLACK;
      if (n->is_leftchild()) {
        rotate_left(n->parent);
      } else {
        rotate_right(n->parent);
      }
    }
    delete_case_3(n);
  }

  // n->parent, s, s_l, s_r 모두 BLACK.
  // s를 RED로 바꿔주면 현재 sub_tree에서는 양쪽의 BLACK 동일해짐.
  // 하지만 sub_tree 전체의 BLACK이 하나 감소했기 때문에
  // n->parent를 기준으로 case_1 부터 다시 돌림.
  void delete_case_3(node_type* n) {
    node_type* s = sibling(n);

    if (n->parent->color == BLACK && s->color == BLACK &&
        s->leftChild->color == BLACK && s->rightChild->color == BLACK) {
      s->color = RED;
      delete_case_1(n->parent);
    } else {
      delete_case_4(n);
    }
  }

  // s, s_l, s_r : BLACK,
  // n->parent : RED
  // s와 n->parent의 색을 바꾼다.
  // n->parent가 BLACK가 되어
  // n쪽 subtree에서 소실된 BLACK을 채워준다.
  // case_2에서 넘어온 경우 여기서 해결됨.
  void delete_case_4(node_type* n) {
    node_type* s = sibling(n);

    if (n->parent->color == RED && s->color == BLACK &&
        s->leftChild->color == BLACK && s->rightChild->color == BLACK) {
      s->color = RED;
      n->parent->color = BLACK;
    } else {
      delete_case_5(n);
    }
  }

  // s, s_r : BLACK, s_l : RED, n이 leftChild인 경우.
  // s를 기준으로 rotate_right한다. (rotate후의 s의 부모(s_l)와 색 변경)
  // s, s_l : BLACK, s_r : RED, ndl rightChild인 경우.
  // s를 기준으로 rotate_left한다. (rotate후의 s의 부모(s_r)와 색 변경)
  // 이 경우 n->parent의 색은 RED or BLACK.
  // case_5 작업만으로는 한쪽의 BLACK갯수가 더 많다.
  // case_6에서 마무리함.
  void delete_case_5(node_type* n) {
    node_type* s = sibling(n);
    node_type* save = n->parent;

    if (s->color == BLACK) {
      if (n->is_leftchild() && s->rightChild->color == BLACK &&
          s->leftChild->color == RED) {
        s->color = RED;
        s->leftChild->color = BLACK;
        rotate_right(s);
      } else if (n->is_rightchild() && s->leftChild->color == BLACK &&
                 s->rightChild->color == RED) {
        s->color = RED;
        s->rightChild->color = BLACK;
        rotate_left(s);
      }
    }
    n->parent = save;
    delete_case_6(n);
  }

  // s : BLACK, s_r : RED, n이 leftChild인 경우.
  // n->parent를 rotate_left한다.
  // n->parent와 s의 색을 바꾸고, s_r을 BLACK로 변경한다.
  // 반대의 경우, 동일한 방법으로 반대로 시행함.
  void delete_case_6(node_type* n) {
    node_type* s = sibling(n);

    s->color = n->parent->color;
    n->parent->color = BLACK;

    if (n->is_leftchild()) {
      s->rightChild->color = BLACK;
      rotate_left(n->parent);
    } else {
      s->leftChild->color = BLACK;
      rotate_right(n->parent);
    }
  }

  void delete_node(node_type* node) {
    if (node->is_root()) {
      this->_root = this->_nil;
    }

    _node_alloc.destroy(node);
    _node_alloc.deallocate(node, 1);
    --this->_size;
  }

};  // class RB_Tree

}  // namespace ft

#endif  // RBTREE_HPP
