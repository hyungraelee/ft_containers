#if !defined(VECTORITERATOR_HPP)
#define VECTORITERATOR_HPP

#include "iterator.hpp"
#include "utils.hpp"

namespace ft {
template < typename T >
class VectorIterator
    : public ft::iterator< ft::random_access_iterator_tag, T > {
 public:
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef typename ft::iterator< ft::random_access_iterator_tag,
                                 T >::difference_type difference_type;
  typedef typename ft::iterator< ft::random_access_iterator_tag,
                                 T >::iterator_category iterator_category;

 protected:
  pointer _ptr;

 public:
  VectorIterator(pointer ptr = u_nullptr) : _ptr(ptr) {}
  VectorIterator(const VectorIterator< T > &other) : _ptr(other.base()) {}
  virtual ~VectorIterator() {}

  operator VectorIterator< const T >() const {
    return (VectorIterator< const T >(this->_ptr));
  }

  VectorIterator &operator=(const VectorIterator< T > &other) {
    if (this != &other) this->_ptr = other.base();
    return (*this);
  }

  pointer const &base() const { return (this->_ptr); }

  reference operator*() const { return (*this->_ptr); }

  pointer operator->() const { return (&(operator*())); }

  VectorIterator &operator++() {
    ++this->_ptr;
    return (*this);
  }

  VectorIterator operator++(int) {
    VectorIterator tmp = *this;
    ++*this;
    return (tmp);
  }

  VectorIterator &operator--() {
    --this->_ptr;
    return (*this);
  }

  VectorIterator operator--(int) {
    VectorIterator tmp = *this;
    --*this;
    return (tmp);
  }

  VectorIterator operator+(difference_type n) const { return (this->_ptr + n); }

  VectorIterator operator-(difference_type n) const { return (this->_ptr - n); }

  VectorIterator &operator+=(difference_type n) {
    this->_ptr += n;
    return (*this);
  }

  VectorIterator &operator-=(difference_type n) {
    this->_ptr -= n;
    return (*this);
  }

  reference operator[](difference_type n) const { return (*(this->_ptr + n)); }
};

template < class T >
ft::VectorIterator< T > operator+(
    typename ft::VectorIterator< T >::difference_type n,
    ft::VectorIterator< T > &it) {
  return (it + n);
}

template < class T >
typename ft::VectorIterator< T >::difference_type operator-(
    const ft::VectorIterator< T > &lhs, const ft::VectorIterator< T > &rhs) {
  return (lhs.base() - rhs.base());
}

template < class T_L, class T_R >
typename ft::VectorIterator< T_L >::difference_type operator-(
    const ft::VectorIterator< T_L > &lhs,
    const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() - rhs.base());
}

template < class T >
bool operator==(const ft::VectorIterator< T > &lhs,
                const ft::VectorIterator< T > &rhs) {
  return (lhs.base() == rhs.base());
}

template < class T_L, class T_R >
bool operator==(const ft::VectorIterator< T_L > &lhs,
                const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() == rhs.base());
}

template < class T >
bool operator!=(const ft::VectorIterator< T > &lhs,
                const ft::VectorIterator< T > &rhs) {
  return (lhs.base() != rhs.base());
}

template < class T_L, class T_R >
bool operator!=(const ft::VectorIterator< T_L > &lhs,
                const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() != rhs.base());
}

template < class T >
bool operator<(const ft::VectorIterator< T > &lhs,
               const ft::VectorIterator< T > &rhs) {
  return (lhs.base() < rhs.base());
}

template < class T_L, class T_R >
bool operator<(const ft::VectorIterator< T_L > &lhs,
               const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() < rhs.base());
}

template < class T >
bool operator>(const ft::VectorIterator< T > &lhs,
               const ft::VectorIterator< T > &rhs) {
  return (lhs.base() > rhs.base());
}

template < class T_L, class T_R >
bool operator>(const ft::VectorIterator< T_L > &lhs,
               const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() > rhs.base());
}

template < class T >
bool operator<=(const ft::VectorIterator< T > &lhs,
                const ft::VectorIterator< T > &rhs) {
  return (lhs.base() <= rhs.base());
}

template < class T_L, class T_R >
bool operator<=(const ft::VectorIterator< T_L > &lhs,
                const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() <= rhs.base());
}

template < class T >
bool operator>=(const ft::VectorIterator< T > &lhs,
                const ft::VectorIterator< T > &rhs) {
  return (lhs.base() >= rhs.base());
}

template < class T_L, class T_R >
bool operator>=(const ft::VectorIterator< T_L > &lhs,
                const ft::VectorIterator< T_R > &rhs) {
  return (lhs.base() >= rhs.base());
}

}  // namespace ft

#endif  // VECTORITERATOR_HPP
