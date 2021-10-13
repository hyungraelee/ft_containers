#if !defined(STACK_HPP)
#define STACK_HPP

#include "vector.hpp"

namespace ft {

template < typename T, class Container = ft::vector< T > >
class stack {
 public:
  typedef T value_type;
  typedef Container container_type;
  typedef size_t size_type;

 protected:
  container_type ct;

 public:
  explicit stack(const container_type& ctnr = container_type()) : ct(ctnr) {}
  virtual ~stack() {}
  bool empty() const { return (this->ct.empty()); }
  size_type size() const { return (this->ct.size()); }
  value_type& top() { return (this->ct.back()); }
  const value_type& top() const { return (this->ct.back()); }
  void push(const value_type& val) { this->ct.push_back(val); }
  void pop() { this->ct.pop_back(); }

  template < class U, class C >
  friend bool operator==(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);

  template < class U, class C >
  friend bool operator!=(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);

  template < class U, class C >
  friend bool operator<(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);

  template < class U, class C >
  friend bool operator<=(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);

  template < class U, class C >
  friend bool operator>(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);

  template < class U, class C >
  friend bool operator>=(const stack< U, C >& lhs,
                         const stack< U, C >& rhs);
};

template < class T, class Container >
bool operator==(const stack< T, Container >& lhs,
                const stack< T, Container >& rhs) {
  return (lhs.ct == rhs.ct);
}

template < class T, class Container >
bool operator!=(const stack< T, Container >& lhs,
                const stack< T, Container >& rhs) {
  return (lhs.ct != rhs.ct);
}

template < class T, class Container >
bool operator<(const stack< T, Container >& lhs,
               const stack< T, Container >& rhs) {
  return (lhs.ct < rhs.ct);
}

template < class T, class Container >
bool operator<=(const stack< T, Container >& lhs,
                const stack< T, Container >& rhs) {
  return (lhs.ct <= rhs.ct);
}

template < class T, class Container >
bool operator>(const stack< T, Container >& lhs,
               const stack< T, Container >& rhs) {
  return (lhs.ct > rhs.ct);
}

template < class T, class Container >
bool operator>=(const stack< T, Container >& lhs,
                const stack< T, Container >& rhs) {
  return (lhs.ct >= rhs.ct);
}

}  // namespace ft

#endif  // STACK_HPP
