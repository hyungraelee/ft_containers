#if !defined(UTILS_HPP)
#define UTILS_HPP

#include "iterator.hpp"

namespace ft {

static class nullptr_t {
 public:
  template < class T >
  operator T*() const {
    return (0);
  }

  template < class C, class T >
  operator T C::*() const {
    return (0);
  }

 private:
  void operator&() const;

} u_nullptr = {};

/**
 * @brief Enable type if condition is met.
 * The type T is enabled as member type enable_if::type if Cond is true.
 * Otherwise, enable_if::type is not defined.
 * @param Cond A compile-time constant of type bool.
 * @param T A type.
 */
template < bool Cond, typename T = void >
struct enable_if {};

template < typename T >
struct enable_if< true, T > {
  typedef T type;
};

// template < typename T, typename U >
// struct is_same {
//   static const value = false;
// };

// template < typename T >
// struct is_same< T, T > {
//   static const value = true;
// };

// template < typename T,
//            typename = typename ft::enable_if< ft::is_same<
//                typename T::value_type,
//                typename ft::iterator_traits< T >::value_type >::value >::type
//                >
// struct is_iter {
//   static const value = true;
// };

/**
 * @brief Checks whether T is an integral type.
 * @details Provides the member constant value which is equal to true,
 * if T is the type
 * bool,
 * char,
 * char16_t,
 * char32_t,
 * wchar_t,
 * short,
 * int,
 * long,
 * long long,
 * unsigned char,
 * unsigned short,
 * unsigned int,
 * unsigned long,
 * unsigned long long,
 * or any implementation-defined extended integer types,
 * including any signed, unsigned, and cv-qualified variants.
 * Otherwise, value is equal to false.
 * @param T a type to check.
 */
template < bool is_integral, typename T >
struct integral_traits {
  typedef T type;
  static const bool value = is_integral;
};

template < typename >
struct is_integral_type : public integral_traits< false, bool > {};

template <>
struct is_integral_type< bool > : public integral_traits< true, bool > {};

template <>
struct is_integral_type< char > : public integral_traits< true, char > {};

template <>
struct is_integral_type< signed char >
    : public integral_traits< true, signed char > {};

template <>
struct is_integral_type< short int >
    : public integral_traits< true, short int > {};

template <>
struct is_integral_type< int > : public integral_traits< true, int > {};

template <>
struct is_integral_type< long int > : public integral_traits< true, long int > {
};

template <>
struct is_integral_type< long long int >
    : public integral_traits< true, long long int > {};

template <>
struct is_integral_type< unsigned char >
    : public integral_traits< true, unsigned char > {};

template <>
struct is_integral_type< unsigned short int >
    : public integral_traits< true, unsigned short int > {};

template <>
struct is_integral_type< unsigned int >
    : public integral_traits< true, unsigned int > {};

template <>
struct is_integral_type< unsigned long int >
    : public integral_traits< true, unsigned long int > {};

template <>
struct is_integral_type< unsigned long long int >
    : public integral_traits< true, unsigned long long int > {};

/**
 * @brief Trait class that identifies whether T is an integral type.
 * It inherits from is_integral_type, depending on whether T is an integral
 * type.
 * @param T a type to check.
 */
template < typename T >
struct is_integral : public is_integral_type< T > {};

/**
 * @brief Lexicographical less-than comparison.
 * @details Returns true if the range [first1,last1) compares lexicographically
 * less than the range [first2,last2).
 * @param first1, last1 Input iterators to the initial and final positions of
 * the first sequence. The range used is [first1,last1).
 * @param first2, last2 Input iterators to the initial and final positions of
 * the second sequence. The range used is [first2,last2).
 * @return true if the first range compares lexicographically less than the
 * second. false otherwise (including when all the elements of both ranges are
 * equivalent).
 */
template < typename InputIterator1, typename InputIterator2 >
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2) {
  while (first1 != last1) {
    if (first2 == last2 || *first2 < *first1)
      return false;
    else if (*first1 < *first2)
      return true;
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

/**
 * @brief Lexicographical less-than comparison.
 * @details Returns true if the range [first1,last1) compares lexicographically
 * less than the range [first2,last2). It decided by comp.
 * @param comp the function that will compare.
 * @return true if the first range compares lexicographically less than the
 * second. false otherwise (including when all the elements of both ranges are
 * equivalent).
 */
template < typename InputIterator1, typename InputIterator2, typename Compare >
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp) {
  while (first1 != last1) {
    if (first2 == last2 || comp(*first2, *first1))
      return false;
    else if (comp(*first1, *first2))
      return true;
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

/**
 * @brief Test whether the elements in two ranges are equal.
 * @details Compares the elements in the range [first1,last1) with those in the
 * range beginning at first2, and returns true if all of the elements in both
 * ranges match.
 * @param first1, last1 Input iterators to the initial and final positions of
 * the first sequence. The range used is [first1,last1).
 * @param first2 Input iterator to the initial position of the second sequence.
 * @return true if all the elements in the range [first1,last1) compare equal to
 * those of the range starting at first2, and false otherwise.
 */
template < class InputIterator1, class InputIterator2 >
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  while (first1 != last1) {
    if (!(*first1 == *first2)) return false;
    ++first1;
    ++first2;
  }
  return true;
}

/**
 * @brief Test whether the elements in two ranges are equal.
 * @details Compares the elements in the range [first1,last1) with those in the
 * range beginning at first2, and returns true if all of the elements in both
 * ranges match. It decided by pred.
 * @param first1, last1 Input iterators to the initial and final positions of
 * the first sequence. The range used is [first1,last1).
 * @param first2 Input iterator to the initial position of the second sequence.
 * @param pred Binary function that returns a value convertible to bool.
 * @return true if all the elements in the range [first1,last1) compare equal to
 * those of the range starting at first2, and false otherwise.
 */
template < class InputIterator1, class InputIterator2, class BinaryPredicate >
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
           BinaryPredicate pred) {
  while (first1 != last1) {
    if (!pred(*first1, *first2)) return false;
    ++first1;
    ++first2;
  }
  return true;
}

template < class T1, class T2 >
struct pair {
 public:
  typedef T1 first_type;
  typedef T2 second_type;

  first_type first;
  second_type second;

  /**
   * @brief default constructor
   */
  pair() : first(), second() {}

  /**
   * @brief copy constructor
   * @param pr Another pair object.
   */
  template < class U, class V >
  pair(const pair< U, V >& pr) : first(pr.first), second(pr.second) {}

  /**
   * @brief  initialization constructor
   * @param a An object of the type of first.
   * @param b An object of the type of second.
   */
  pair(const first_type& a, const second_type& b) : first(a), second(b) {}

  /**
   * @brief Assign contents
   * @param pr Another pair object.
   * @return *this
   */
  pair& operator=(const pair& pr) {
    if (this != &pr) {
      this->first = pr.first;
      this->second = pr.second;
    }
    return (*this);
  }
};

/**
 * @brief Equal comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator==(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

/**
 * @brief Difference comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator!=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return !(lhs == rhs);
}

/**
 * @brief Inferior comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator<(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return lhs.first < rhs.first ||
         (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

/**
 * @brief Inferior or equal comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator<=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return !(rhs < lhs);
}

/**
 * @brief Superior comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator>(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return rhs < lhs;
}

/**
 * @brief Superior or equal comparison between two pair object.
 * @param lhs Base of comparison.
 * @param rhs To compare with "lsh".
 * @return True if the condition is hold, otherwise false.
 */
template < class T1, class T2 >
bool operator>=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return !(lhs < rhs);
}

/**
 * @brief Construct pair object
 * @param x,y Values for the members first and second
 * @return A pair object whose elements first and second are set to x and y
 * respectivelly.
 */
template < class T1, class T2 >
pair< T1, T2 > make_pair(T1 x, T2 y) {
  return (ft::pair< T1, T2 >(x, y));
}

}  // namespace ft

#endif  // UTILS_HPP
