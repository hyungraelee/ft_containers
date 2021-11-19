#if !defined(RB_TREENODE_HPP)
#define RB_TREENODE_HPP

#include <memory>
#include <iostream>

namespace ft {

enum Color { RED, BLACK };

template < typename T, typename Alloc = std::allocator< T > >
struct RB_TreeNode {
 public:
  typedef T value_type;

  T *value;
  RB_TreeNode *leftChild;
  RB_TreeNode *rightChild;
  RB_TreeNode *parent;
  Color color;
  Alloc alloc;

  RB_TreeNode()
      : value(NULL),
        leftChild(NULL),
        rightChild(NULL),
        parent(NULL),
        color(BLACK),
        alloc(Alloc()) {
    // this->value = alloc.allocate(1);
    // alloc.construct(this->value, T());
  }

  RB_TreeNode(const T &val)
      : value(NULL),
        leftChild(NULL),
        rightChild(NULL),
        parent(NULL),
        color(RED),
        alloc(Alloc()) {
    this->value = alloc.allocate(1);
    alloc.construct(this->value, val);
  }

  RB_TreeNode(const RB_TreeNode &ot)
      : value(NULL),
        leftChild(NULL),
        rightChild(NULL),
        parent(NULL),
        color(RED),
        alloc(Alloc()) {
    if (ot.value != NULL) {
      this->value = alloc.allocate(1);
      alloc.construct(this->value, *(ot.value));
      this->color = ot.color;
    }
  }

  ~RB_TreeNode() {
    if (this->value != NULL) {
      alloc.destroy(value);
      alloc.deallocate(value , 1);
    }
  }

  bool is_root() const {
    if (this->parent->is_nil()) {
      return true;
    }
    return false;
  }

  bool is_leftchild() const {
    if (this->parent->leftChild == this) {
      return true;
    }
    return false;
  }

  bool is_rightchild() const {
    if (this->parent->rightChild == this) {
      return true;
    }
    return false;
  }

  // nil 노드.
  bool is_nil() const {
    if (this->value == NULL) {
      return true;
    }
    return false;
  }

  // branch의 끝 노드인지. (자신은 nil이 아니며, 두 자식 모두 nil인 노드)
  // bool is_end_branch() const {
  //   if (this->leftChild->is_nil() && this->rightChild->is_nil()) {
  //     return true;
  //   }
  //   return false
  // }

  Color get_uncle_color() const {
    if (this->parent->is_leftchild())
      return this->parent->parent->rightChild->color;
    else
      return this->parent->parent->leftChild->color;
  }

  bool operator<(const RB_TreeNode &other) const {
    return (*(this->value) < *(other->value));
  }

  bool operator>(const RB_TreeNode &other) const { return (other < *this); }

  bool operator==(const RB_TreeNode &other) const {
    return (*(this->value) == *(other->value));
  }

  bool operator!=(const RB_TreeNode &other) const {
    return (*(this->value) != *(other->value));
  }
};

}  // namespace ft

#endif  // RB_TREENODE_HPP
