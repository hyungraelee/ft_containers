#if !defined(UTILS_HPP)
#define UTILS_HPP

#include "iterator.hpp"

namespace ft {

// 그냥 NULL 세팅보다 type에 맞는 null-pointer로 세팅하기 위함.
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

template < typename InputIterator >
typename ft::iterator_traits< InputIterator >::difference_type distance(
    InputIterator first, InputIterator last) {
  typename ft::iterator_traits< InputIterator >::difference_type dist = 0;

  for (; first != last; first++) {
    dist++;
  }
  return (dist);
}

/**
 * Cond가 true인 경우를 specialize하여 type을 typename으로 사용할 수 있게 한다.
 */
template < bool Cond, typename T = void >
struct enable_if {};

template < typename T >
struct enable_if< true, T > {
  typedef T type;
};

/**
 * T가 integral type인지 체크한다.
 * T가 다음의 type들 중 하나라면 true,
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
 * 그렇지 않으면 false.
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
 * T가 integral type인지 식별하는 class.
 * is_integral_type< T > 를 상속받아 T에 따라 type을 다르게 갖는다.
 */
template < typename T >
struct is_integral : public is_integral_type< T > {};

/**
 * 사전식 순서 비교_1
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
 * 사전식 순서 비교_2 : comp로 비교.
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
 * iterator범위 사이에서 동일한지 check.
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
 * iterator범위 사이에서 동일한지 check. : pred로 비교.
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

template < class Arg1, class Arg2, class Result >
struct binary_function {
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};

template < class T >
struct less : binary_function< T, T, bool > {
  bool operator()(const T& x, const T& y) const {
    return x < y; }
};

template < class T1, class T2 >
struct pair {
 public:
  typedef T1 first_type;
  typedef T2 second_type;

  first_type first;
  second_type second;

  pair() : first(), second() {}

  template < class U, class V >
  pair(const pair< U, V >& pr) : first(pr.first), second(pr.second) {}
  pair(const first_type& a, const second_type& b) : first(a), second(b) {}
  pair& operator=(const pair& pr) {
    if (this != &pr) {
      this->first = pr.first;
      this->second = pr.second;
    }
    return (*this);
  }
};

template < class T1, class T2 >
bool operator==(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return (lhs.first == rhs.first && lhs.second == rhs.second);
}

template < class T1, class T2 >
bool operator!=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return !(lhs == rhs);
}

template < class T1, class T2 >
bool operator<(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return lhs.first < rhs.first ||
         (!(rhs.first < lhs.first) && (lhs.second < rhs.second));
}

template < class T1, class T2 >
bool operator<=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return (!(rhs < lhs));
}

template < class T1, class T2 >
bool operator>(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return (rhs < lhs);
}

template < class T1, class T2 >
bool operator>=(const pair< T1, T2 >& lhs, const pair< T1, T2 >& rhs) {
  return !(lhs < rhs);
}

template < class T1, class T2 >
pair< T1, T2 > make_pair(T1 x, T2 y) {
  return (ft::pair< T1, T2 >(x, y));
}

}  // namespace ft

#endif  // UTILS_HPP
