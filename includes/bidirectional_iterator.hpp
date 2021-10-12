#if !defined(BIDIRECTIONAL_ITERATOR_HPP)
#define BIDIRECTIONAL_ITERATOR_HPP

#include "iterator.hpp"
#include "utils.hpp"

namespace ft {
template < typename T >
class bidirectional_iterator
    : public ft::iterator< ft::bidirectional_iterator_tag, T > {
 public:
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef typename ft::iterator< ft::bidirectional_iterator_tag,
                                 T >::difference_type difference_type;
  typedef typename ft::iterator< ft::bidirectional_iterator_tag,
                                 T >::iterator_category iterator_category;

 protected:
  pointer _ptr;

 public:
  bidirectional_iterator(pointer ptr = u_nullptr) : _ptr(ptr) {}
  bidirectional_iterator(const bidirectional_iterator< T > &other)
      : _ptr(other.base()) {}
  virtual ~bidirectional_iterator() {}

  bidirectional_iterator &operator=(const bidirectional_iterator< T > &other) {
    if (this != &other) this->_ptr = other.base();
    return (*this);
  }

  pointer const &base() const { return (this->_ptr); }

  reference operator*() const { return (*this->_ptr); }

  pointer operator->() const { return (&(operator*())); }

  bidirectional_iterator &operator++() {
    ++this->_ptr;
    return (*this);
  }

  bidirectional_iterator operator++(int) {
    bidirectional_iterator tmp = *this;
    ++*this;
    return (tmp);
  }

  bidirectional_iterator &operator--() {
    --this->_ptr;
    return (*this);
  }

  bidirectional_iterator operator--(int) {
    bidirectional_iterator tmp = *this;
    --*this;
    return (tmp);
  }
};

template < class T >
bool operator==(const ft::bidirectional_iterator< T > &lhs,
                const ft::bidirectional_iterator< T > &rhs) {
  return (lhs.base() == rhs.base());
}

template < class T_L, class T_R >
bool operator==(const ft::bidirectional_iterator< T_L > &lhs,
                const ft::bidirectional_iterator< T_R > &rhs) {
  return (lhs.base() == rhs.base());
}

template < class T >
bool operator!=(const ft::bidirectional_iterator< T > &lhs,
                const ft::bidirectional_iterator< T > &rhs) {
  return (lhs.base() != rhs.base());
}

template < class T_L, class T_R >
bool operator!=(const ft::bidirectional_iterator< T_L > &lhs,
                const ft::bidirectional_iterator< T_R > &rhs) {
  return (lhs.base() != rhs.base());
}

}  // namespace ft

#endif  // BIDIRECTIONAL_ITERATOR_HPP
