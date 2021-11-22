#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>

namespace ft {


/**
 * 각 iterator의 catecory를 구분하기 위한 빈 클래스.
*/
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


/**
 * iterator 특성들의 type이름을 고정.
 * Iterator의 성격이 달라도 통일해서 사용할 수 있다.
 */
template < class Iterator >
struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

template < class T >
struct iterator_traits< T* > {
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef random_access_iterator_tag iterator_category;
};

template < class T >
struct iterator_traits< const T* > {
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef random_access_iterator_tag iterator_category;
};

template < class Category, class T, class Distance = ptrdiff_t,
           class Pointer = T*, class Reference = T& >
struct iterator {
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Category iterator_category;
};

template < class Iterator >
class reverse_iterator {
 public:
  typedef Iterator iterator_type;
  typedef
      typename iterator_traits< Iterator >::iterator_category iterator_category;
  typedef typename iterator_traits< Iterator >::value_type value_type;
  typedef typename iterator_traits< Iterator >::difference_type difference_type;
  typedef typename iterator_traits< Iterator >::pointer pointer;
  typedef typename iterator_traits< Iterator >::reference reference;

 protected:
  Iterator current;

 public:
  reverse_iterator() : current() {}
  explicit reverse_iterator(iterator_type it) : current(it) {}
  template < class Iter >
  reverse_iterator(const reverse_iterator< Iter >& rev_it)
      : current(rev_it.base()) {}

  /* Destructor added to follow subject. */
  // virtual ~reverse_iterator() {}

  iterator_type base() const { return (current); }

  reference operator*() const {
    iterator_type tmp = current;
    return (*(--tmp));
  }

  reverse_iterator operator+(difference_type n) const {
    return (reverse_iterator(current - n));
  }

  reverse_iterator& operator++() {
    --(this->current);
    return (*this);
  }

  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    ++(*this);
    return (tmp);
  }

  reverse_iterator& operator+=(difference_type n) {
    this->current -= n;
    return (*this);
  }

  reverse_iterator operator-(difference_type n) const {
    return (reverse_iterator(current + n));
  }

  reverse_iterator& operator--() {
    ++(this->current);
    return (*this);
  }

  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    --(*this);
    return (tmp);
  }

  reverse_iterator& operator-=(difference_type n) {
    this->current += n;
    return (*this);
  }

  pointer operator->() const { return (&(operator*())); }

  reference operator[](difference_type n) const {
    return (this->current[-n - 1]);
  }
};

template < class Iterator >
bool operator==(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() == rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator==(const reverse_iterator< Iterator_L >& lhs,
                const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() == rhs.base());
}

template < class Iterator >
bool operator!=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() != rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator!=(const reverse_iterator< Iterator_L >& lhs,
                const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() != rhs.base());
}

template < class Iterator >
bool operator<(const reverse_iterator< Iterator >& lhs,
               const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() > rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator<(const reverse_iterator< Iterator_L >& lhs,
               const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() > rhs.base());
}

template < class Iterator >
bool operator<=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() >= rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator<=(const reverse_iterator< Iterator_L >& lhs,
                const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() >= rhs.base());
}

template < class Iterator >
bool operator>(const reverse_iterator< Iterator >& lhs,
               const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() < rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator>(const reverse_iterator< Iterator_L >& lhs,
               const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() < rhs.base());
}

template < class Iterator >
bool operator>=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs) {
  return (lhs.base() <= rhs.base());
}

template < class Iterator_L, class Iterator_R >
bool operator>=(const reverse_iterator< Iterator_L >& lhs,
                const reverse_iterator< Iterator_R >& rhs) {
  return (lhs.base() <= rhs.base());
}

template < class Iterator >
reverse_iterator< Iterator > operator+(
    typename reverse_iterator< Iterator >::difference_type n,
    const reverse_iterator< Iterator >& rev_it) {
  return (rev_it + n);
}

template < class Iterator >
typename reverse_iterator< Iterator >::difference_type operator-(
    const reverse_iterator< Iterator >& lhs,
    const reverse_iterator< Iterator >& rhs) {
  return (rhs.base() - lhs.base());
}

template < class Iterator_L, class Iterator_R >
typename reverse_iterator< Iterator_L >::difference_type operator-(
    const reverse_iterator< Iterator_L >& lhs,
    const reverse_iterator< Iterator_R >& rhs) {
  return (rhs.base() - lhs.base());
}

}  // namespace ft

#endif
