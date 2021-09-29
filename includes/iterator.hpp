#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstddef>

namespace ft {

/**
 * @brief Empty class to identify the category of an iterator as an input
 * iterator.
 */
struct input_iterator_tag {};

/**
 * @brief Empty class to identify the category of an iterator as an output
 * iterator.
 */
struct output_iterator_tag {};

/**
 * @brief Empty class to identify the category of an iterator as a forward
 * iterator.
 */
struct forward_iterator_tag : public input_iterator_tag {};

/**
 * @brief Empty class to identify the category of an iterator as a bidirectional
 * iterator.
 */
struct bidirectional_iterator_tag : public forward_iterator_tag {};

/**
 * @brief Empty class to identify the category of an iterator as a random-access
 * iterator.
 */
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/**
 * @brief Traits class defining properties of iterators. Standard algorithms
 * determine certain properties of the iterators passed to them and the range
 * they represent by using the members of the corresponding iterator_traits
 * instantiation.
 */
template < class Iterator >
struct iterator_traits {
  /* Type to express the result of subtracting one iterator from another. */
  typedef typename Iterator::difference_type difference_type;
  /* The type of the element the iterator can point to. */
  typedef typename Iterator::value_type value_type;
  /* The type of a pointer to an element the iterator can point to. */
  typedef typename Iterator::pointer pointer;
  /* The type of a reference to an element the iterator can point to. */
  typedef typename Iterator::reference reference;
  /* The iterator category. */
  typedef typename Iterator::iterator_category iterator_category;
};

/**
 * @brief Traits class specialized for pointers (T*).
 */
template < class T >
struct iterator_traits< T* > {
  /* Type to express the result of subtracting one iterator from another. */
  typedef ptrdiff_t difference_type;
  /* The type of the element the iterator can point to. */
  typedef T value_type;
  /* The type of a pointer to an element the iterator can point to. */
  typedef T* pointer;
  /* The type of a reference to an element the iterator can point to. */
  typedef T& reference;
  /* The iterator category. */
  typedef random_access_iterator_tag iterator_category;
};

/**
 * @brief Traits class specialized for pointers to const (const T*).
 */
template < class T >
struct iterator_traits< const T* > {
  /* Type to express the result of subtracting one iterator from another. */
  typedef ptrdiff_t difference_type;
  /* The type of the element the iterator can point to. */
  typedef T value_type;
  /* The type of a pointer to an element the iterator can point to. */
  typedef const T* pointer;
  /* The type of a reference to an element the iterator can point to. */
  typedef const T& reference;
  /* The iterator category. */
  typedef random_access_iterator_tag iterator_category;
};

/**
 * @brief This is a base class template that can be used to derive iterator
 * classes from it. It is not an iterator class and does not provide any of the
 * functionality an iterator is expected to have.
 * @details This base class only provides some member types, which in fact are
 * not required to be present in any iterator type (iterator types have no
 * specific member requirements), but they might be useful, since they define
 * the members needed for the default iterator_traits class template to generate
 * the appropriate instantiation automatically (and such instantiation is
 * required to be valid for all iterator types).
 */
template < class Category, class T, class Distance = ptrdiff_t,
           class Pointer = T*, class Reference = T& >
struct iterator {
  /* Type of elements pointed by the iterator. */
  typedef T value_type;
  /* Type to represent the difference between two iterators. */
  typedef Distance difference_type;
  /* Type to represent a pointer to an element pointed by the iterator. */
  typedef Pointer pointer;
  /* Type to represent a reference to an element pointed by the iterator. */
  typedef Reference reference;
  /* Category to which the iterator belongs to. */
  typedef Category iterator_category;
};

/**
 * @brief This class reverses the direction in which a bidirectional or
 * random-access iterator iterates through a range. A copy of the original
 * iterator (the base iterator) is kept internally and used to reflect the
 * operations performed on the reverse_iterator: whenever the reverse_iterator
 * is incremented, its base iterator is decreased, and vice versa. A copy of the
 * base iterator with the current state can be obtained at any time by calling
 * member base.
 * @details Notice however that when an iterator is reversed, the reversed
 * version does not point to the same element in the range, but to the one
 * preceding it. This is so, in order to arrange for the past-the-end element of
 * a range: An iterator pointing to a past-the-end element in a range, when
 * reversed, is pointing to the last element (not past it) of the range (this
 * would be the first element of the reversed range). And if an iterator to the
 * first element in a range is reversed, the reversed iterator points to the
 * element before the first element (this would be the past-the-end element of
 * the reversed range).
 */
template < class Iterator >
class reverse_iterator {
 public:
  /* Iterator's type */
  typedef Iterator iterator_type;
  /* Preserves Iterator's category */
  typedef
      typename iterator_traits< Iterator >::iterator_category iterator_category;
  /* Preserves Iterator's value type */
  typedef typename iterator_traits< Iterator >::value_type value_type;
  /* Preserves Iterator's difference type */
  typedef typename iterator_traits< Iterator >::difference_type difference_type;
  /* Preserves Iterator's pointer type */
  typedef typename iterator_traits< Iterator >::pointer pointer;
  /* Preserves Iterator's reference type */
  typedef typename iterator_traits< Iterator >::reference reference;

 protected:
  Iterator current;

 public:
  /**
   * @brief default constructor
   * Constructs a reverse iterator that points to no object.
   * The internal base iterator is value-initialized.
   */
  reverse_iterator() : current() {}

  /**
   * @brief initalization constructor
   * Constructs a reverse iterator from some original iterator it.The behavior
   * of the constructed object replicates the original, except that it iterates
   * through its pointed elements in the reverse order.
   * @param it An iterator, whose sense of iteration is inverted in the
   * constructed object. Member type iterator_type is the underlying
   * bidirectional iterator type (the class template parameter: Iterator).
   */
  explicit reverse_iterator(iterator_type it) : current(it) {}

  /**
   * @brief copy / type-cast constructor
   * Constructs a reverse iterator from some other reverse iterator. The
   * constructed object keeps the same sense of iteration as rev_it.
   * @param rev_it An iterator of a reverse_iterator type, whose sense of
   * iteration is preserved.
   */
  template < class Iter >
  reverse_iterator(const reverse_iterator< Iter >& rev_it)
      : current(rev_it.base());

  /* Destructor added to follow subject. */
  // virtual ~reverse_iterator() {}

  /**
   * @brief Returns a copy of the base iterator. The base iterator is an
   * iterator of the same type as the one used to construct the
   reverse_iterator,
   * but pointing to the element next to the one the reverse_iterator is
   currently
   * pointing to (a reverse_iterator has always an offset of -1 with respect to
   * its base iterator).
   * @return A copy of the base iterator, which iterates in the opposite
   direction.
  */
  iterator_type base() const { return (current); }

  /**
   * @brief Dereference iterator.
   * Returns a reference to the element pointed to by the iterator.
   * Internally, the function decreases a copy of its base iterator and returns
   * the result of dereferencing it. The iterator shall point to some object in
   * order to be dereferenceable.
   * @return A reference to the element pointed by the iterator.
   * Member type reference is an alias of the base iterator's own reference
   * type.
   */
  reference operator*() const {
    iterator_type tmp = current;
    return (*(--tmp));
  }

  /**
   * @brief Addition operator.
   * Returns a reverse iterator pointing to the element located n positions away
   * from the element the iterator currently points to.
   * Internally, the function applies the binary operator- on the base iterator
   * and returns a reverse iterator constructed with the resulting iterator
   * value.
   * @param n Number of elements to offset.
   * Member type difference_type is an alias of the base iterator's own
   * difference type.
   * @return An iterator pointing to the element n positions away.
   */
  reverse_iterator operator+(difference_type n) const;
  { return (reverse_iterator(current - n)); }

  /**
   * @brief Pre-increment iterator position.
   * Advances the reverse_iterator by one position.
   * @return The pre-increment version returns *this.
   */
  reverse_iterator& operator++() {
    --(this->current);
    return (*this);
  }

  /**
   * @brief Post-increment iterator position.
   * Advances the reverse_iterator by one position.
   * @return The post-increment version returns the value *this had before the
   * call.
   */
  reverse_iterator operator++(int);
  {
    reverse_iterator tmp = *this;
    ++(*this);
    return (tmp);
  }

  /**
   * @brief Advance iterator.
   * Advances the reverse_iterator by n element positions.
   * Internally, the function decreases by n the base iterator kept by the
   * object.
   * @param n Number of elements to offset.
   * Member type difference_type is an alias of the base iterator's own
   * difference type.
   * @return The reverse iterator itself.
   */
  reverse_iterator& operator+=(difference_type n) {
    this->current -= n;
    return (*this);
  }

  /**
   * @brief Subtraction operator.
   * Returns a reverse iterator pointing to the element located n positions
   * before the element the iterator currently points to. Internally, the
   * function applies the binary operator+ on the base iterator and returns a
   * reverse iterator constructed with the resulting iterator value.
   * @param n Number of elements to offset.
   * difference_type is a member type defined as an alias of the base iterator's
   * own difference type
   * @return An iterator pointing to the element n positions before the
   * currently pointed one.
   */
  reverse_iterator operator-(difference_type n) const {
    return (reverse_iterator(current + n));
  }

  /**
   * @brief Pre-decrease iterator position.
   * Decreases the reverse_iterator by one position.
   * @return The pre-decrement version returns *this.
   */
  reverse_iterator& operator--() {
    ++(this->current);
    return (*this);
  }

  /**
   * @brief Post-decrease iterator position.
   * Decreases the reverse_iterator by one position.
   * @return The post-decrement version returns the value *this had before the
   * call.
   */
  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    --(*this);
    return (tmp);
  }

  /**
   * @brief Retrocede iterator.
   * Descreases the reverse_iterator by n element positions.
   * Internally, the function increases by n the base iterator kept by the
   * object.
   * @param n Number of elements to offset.
   * Member type difference_type is an alias of the base iterator's own
   * difference type.
   * @return The reverse iterator itself (*this).
   */
  reverse_iterator& operator-=(difference_type n) {
    this->current += n;
    return (*this);
  }

  /**
   * @brief Dereference iterator.
   * Returns a pointer to the element pointed to by the iterator.
   * @return A pointer to the element pointed by the iterator.
   * Member type pointer is an alias of the base iterator's own pointer type.
   */
  pointer operator->() const { return (&(operator*())); }

  /**
   * @brief Dereference iterator with offset.
   * Accesses the element located n positions away from the element currently
   * pointed to by the iterator. If such an element does not exist, it causes
   * undefined behavior. Internally, the function accesses the proper element of
   * its base iterator, returning the same as: base()[-n-1].
   * @param n Number of elements to offset.
   * Member type difference_type is an alias of the base iterator's own
   * difference type.
   * @return A reference to the element n positions away from the element
   * currently pointed by the iterator. Member type reference is an alias of the
   * base iterator's own reference type.
   */
  reference operator[](difference_type n) const {
    return (this->base()[-n - 1]);
  }
};

template < class Iterator >
bool operator==(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs);

template < class Iterator >
bool operator!=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs);

template < class Iterator >
bool operator<(const reverse_iterator< Iterator >& lhs,
               const reverse_iterator< Iterator >& rhs);

template < class Iterator >
bool operator<=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs);

template < class Iterator >
bool operator>(const reverse_iterator< Iterator >& lhs,
               const reverse_iterator< Iterator >& rhs);

template < class Iterator >
bool operator>=(const reverse_iterator< Iterator >& lhs,
                const reverse_iterator< Iterator >& rhs);

template < class Iterator >
reverse_iterator< Iterator > operator+(
    typename reverse_iterator< Iterator >::difference_type n,
    const reverse_iterator< Iterator >& rev_it);

template < class Iterator >
typename reverse_iterator< Iterator >::difference_type operator-(
    const reverse_iterator< Iterator >& lhs,
    const reverse_iterator< Iterator >& rhs);

}  // namespace ft

#endif
