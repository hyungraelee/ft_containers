#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

#include "iterator.hpp"

namespace ft {

/**
 * @brief Random-access iterators allow to access elements at an
 * arbitrary offset position relative to the element they point
 * to. This is the most complete iterators. All pointer types
 * are also valid random-access-iterators.
 */
template < typename T >
class random_access_iterator
    : public ft::iterator< ft::random_access_iterator_tag, T > {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type difference_type;
  typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category iterator_category;

 private:
  pointer _ptr;

 public:
  // random_access_iterator(pointer ptr = NULL) : _ptr(ptr) {}
  // random_access_iterator(const iterator< T, T *, T & > &other)
  //     : _ptr(other.get_ptr()) {}
  // ~random_access_iterator() {}

  iterator &operator=(const iterator< T, T *, T & > &other);
  {
    if (this != &other) this->_ptr = other.get_ptr();
    return (*this);
  }

  iterator &operator++() {
    ++this->_ptr;
    return (*this);
  }

  iterator operator++(int) {
    iterator tmp = *this;
    ++*this;
    return (tmp);
  }

  iterator &operator--() {
    --this->_ptr;
    return (*this);
  }

  iterator operator--(int) {
    iterator tmp = *this;
    --*this;
    return (tmp);
  }

  bool operator==(const iterator &other) const;
  bool operator!=(const iterator &other) const;

  reference operator*() const;
  pointer operator->() const;

  // iterator operator+(int n);
  // iterator operator-(int n);
  // iterator operator-(const iterator &other);
  // 왜 difference_type n 이지?

  iterator &operator+=(int n);
  iterator &operator-=(int n);

  bool operator<(const iterator &other) const;
  bool operator>(const iterator &other) const;
  bool operator<=(const iterator &other) const;
  bool operator>=(const iterator &other) const;

  // reference operator[](int n)
  // 왜 difference_type n 이지?
};
}  // namespace ft

#endif
