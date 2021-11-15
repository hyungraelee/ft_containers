#if !defined(RB_TREEITERATOR_HPP)
#define RB_TREEITERATOR_HPP

#include "RB_TreeNode.hpp"
// #include "bidirectional_iterator.hpp"
#include "iterator.hpp"
#include "utils.hpp"

namespace ft {
template < typename T, typename Pointer = T *, typename Reference = T & >
class RB_TreeIterator
    : public ft::iterator< ft::bidirectional_iterator_tag, T > {
 public:
  typedef T value_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef typename ft::iterator< ft::bidirectional_iterator_tag,
                                 T >::difference_type difference_type;
  typedef typename ft::iterator< ft::bidirectional_iterator_tag,
                                 T >::iterator_category iterator_category;
  typedef ft::RB_TreeNode< T > node_type;

 private:
  node_type *np;
  node_type *get_next() {
    node_type *ret = NULL;

    if (!this->np->rightChild->is_nil()) {  // 현재 rightChild가 있는 경우.
      ret = this->np->rightChild;
      while (!ret->leftChild->is_nil()) {  // rightChild의 가장 왼쪽을 탐색.
        ret = ret->leftChild;
      }
    } else {  // 현재 rightChild가 없는 경우.
      ret = this->np->parent;
      if (this->np->is_rightchild()) {  // 현재 rightChild인 경우.
        while (ret->is_rightchild()) {
          ret = ret->parent;
        }
        // 여기서 nil을 가리키게 하기 위해 수정했음.
        // 조건문 두개 모두 리턴값 동일하기 때문에 나중에 다시 확인하고 수정할
        // 것!
        if (ret->is_root()) {
          ret = ret->parent;
        } else if (ret->is_leftchild()) {
          ret = ret->parent;  // leftChild인 parent를 찾은 후 그의 부모.
        }
      }
    }
    return (ret);
  }

  node_type *get_before() {
    node_type *ret = NULL;

    if (!this->np->leftChild->is_nil()) {  // 현재 leftChild가 있는 경우.
      ret = this->np->leftChild;
      while (!ret->rightChild->is_nil()) {
        ret = ret->rightChild;
      }
    } else {  // 현재 leftChild가 없는 경우.
      ret = this->np->parent;
      if (this->np->is_leftchild()) {  // 현재 leftChild인 경우.
        while (ret->is_leftchild()) {
          ret = ret->parent;
        }
        // 여기서 nil을 가리키게 하기 위해 수정했음.
        // 조건문 두개 모두 리턴값 동일하기 때문에 나중에 다시 확인하고 수정할
        // 것!
        // if (ret->is_root()) {
        //   ret = ret->parent;
        // } else if (ret->is_rightchild()) {
        //   ret = ret->parent;  // rightChild인 parent를 찾은 후 그의 부모.
        // }
        ret = ret->parent;
      }
    }
    return (ret);
  }

 public:
  RB_TreeIterator(node_type *ptr = u_nullptr) : np(ptr) {}
  RB_TreeIterator(const RB_TreeIterator< T > &other) : np(other.base()) {}
  virtual ~RB_TreeIterator() {};

  RB_TreeIterator &operator=(const RB_TreeIterator< T > &other) {
    if (this != &other) this->np = other.base();
    return (*this);
  }

  operator RB_TreeIterator< const T >() const {
    return (RB_TreeIterator< const T >(this->np));
  }

  node_type *const &base() const { return (this->np); }

  reference operator*() const { return (*(this->np->value)); }

  pointer operator->() const { return (this->np->value); }

  RB_TreeIterator &operator++() {
    this->np = get_next();
    return (*this);
  }

  RB_TreeIterator operator++(int) {
    RB_TreeIterator tmp = *this;
    ++*this;
    return (tmp);
  }

  RB_TreeIterator &operator--() {
    this->np = get_before();
    return (*this);
  }

  RB_TreeIterator operator--(int) {
    RB_TreeIterator tmp = *this;
    --*this;
    return (tmp);
  }

  template < class P, class Q, class R >
  bool operator==(const ft::RB_TreeIterator< P, Q, R > &ot) {
    return (this->np == ot.base());
  }

  template < class P, class Q, class R >
  bool operator!=(const ft::RB_TreeIterator< P, Q, R > &ot) {
    return (this->np != ot.base());
  }
};

// template < class T >
// bool operator==(const ft::RB_TreeIterator< T, T*, T& > &lhs,
//                 const ft::RB_TreeIterator< T, T*, T& > &rhs) {
//   return (lhs.base() == rhs.base());
// }

// template < class T_L, class T_R >
// bool operator==(const ft::RB_TreeIterator< T_L, T_L*, T_L& > &lhs,
//                 const ft::RB_TreeIterator< T_R, T_R*, T_R& > &rhs) {
//   return (lhs.base() == rhs.base());
// }

// template < class T >
// bool operator!=(const ft::RB_TreeIterator< T > &lhs,
//                 const ft::RB_TreeIterator< T > &rhs) {
//   return (lhs.base() != rhs.base());
// }

// template < class T_L, class T_R >
// bool operator!=(const ft::RB_TreeIterator< T_L > &lhs,
//                 const ft::RB_TreeIterator< T_R > &rhs) {
//   return (lhs.base() != rhs.base());
// }

}  // namespace ft

#endif  // RB_TREEITERATOR_HPP
