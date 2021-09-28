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
struct iterator_traits< T * > {
  /* Type to express the result of subtracting one iterator from another. */
  typedef ptrdiff_t difference_type;
  /* The type of the element the iterator can point to. */
  typedef T value_type;
  /* The type of a pointer to an element the iterator can point to. */
  typedef T *pointer;
  /* The type of a reference to an element the iterator can point to. */
  typedef T &reference;
  /* The iterator category. */
  typedef random_access_iterator_tag iterator_category;
};

/**
 * @brief Traits class specialized for pointers to const (const T*).
 */
template < class T >
struct iterator_traits< const T * > {
  /* Type to express the result of subtracting one iterator from another. */
  typedef ptrdiff_t difference_type;
  /* The type of the element the iterator can point to. */
  typedef T value_type;
  /* The type of a pointer to an element the iterator can point to. */
  typedef const T *pointer;
  /* The type of a reference to an element the iterator can point to. */
  typedef const T &reference;
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
           class Pointer = T *, class Reference = T & >
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
  reverse_iterator(const reverse_iterator< Iter > &rev_it)
      : current(rev_it.base());
};

}  // namespace ft

#endif
