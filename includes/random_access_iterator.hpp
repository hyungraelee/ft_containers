#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

#include "bidirectional_iterator.hpp"

namespace ft {

/**
 * @brief Random-access iterators allow to access elements at an
 * arbitrary offset position relative to the element they point
 * to. This is the most complete iterators. All pointer types
 * are also valid random-access-iterators.
 */
template < typename T >
class random_access_iterator : public ft::bidirectional_iterator< T > {
 public:
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef typename ft::iterator< ft::random_access_iterator_tag,
                                 T >::difference_type difference_type;
  typedef typename ft::iterator< ft::random_access_iterator_tag,
                                 T >::iterator_category iterator_category;

  random_access_iterator(pointer ptr = (void *)0)
      : ft::bidirectional_iterator< T >(ptr) {}
  random_access_iterator(const random_access_iterator< T > &other)
      : ft::bidirectional_iterator< T >(other.base()) {}
  virtual ~random_access_iterator() {}

  random_access_iterator &operator=(const random_access_iterator< T > &other) {
    if (this != &other) this->_ptr = other.base();
    return (*this);
  }

  random_access_iterator operator+(difference_type n) {
    return (random_access_iterator(this->_ptr + n));
  }

  random_access_iterator operator-(difference_type n) {
    return (random_access_iterator(this->_ptr - n));
  }

  difference_type operator-(const random_access_iterator< T > &other) {
    return (this->_ptr - other.base());
  }

  random_access_iterator &operator+=(difference_type n) {
    this->_ptr += n;
    return (*this);
  }

  random_access_iterator &operator-=(difference_type n) {
    this->_ptr -= n;
    return (*this);
  }

  bool operator<(const random_access_iterator< T > &other) const {
    return (this->_ptr < other.base());
  }

  bool operator>(const random_access_iterator< T > &other) const {
    return (this->_ptr > other.base());
  }

  bool operator<=(const random_access_iterator< T > &other) const {
    return (this->_ptr <= other.base());
  }

  bool operator>=(const random_access_iterator< T > &other) const {
    return (this->_ptr >= other.base());
  }

  reference operator[](difference_type n) { return (*(this->_ptr + n)); }
};

template < class T >
ft::random_access_iterator< T > operator+(
    typename ft::random_access_iterator< T >::difference_type n,
    ft::random_access_iterator< T > &it) {
  return (it + n);
}

}  // namespace ft

#endif
