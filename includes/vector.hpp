#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

#include "VectorIterator.hpp"
#include "utils.hpp"

namespace ft {

template < typename T, typename Allocator = std::allocator< T > >
class vector {
 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef ft::VectorIterator< T > iterator;
  typedef ft::VectorIterator< const T > const_iterator;
  typedef ft::reverse_iterator< iterator > reverse_iterator;
  typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;
  typedef typename allocator_type::size_type size_type;
  typedef typename allocator_type::difference_type difference_type;

 private:
  allocator_type _alloc;
  pointer _start;
  pointer _end;
  pointer _end_capacity;

 public:
  explicit vector(const allocator_type &alloc = allocator_type())
      : _alloc(alloc),
        _start(u_nullptr),
        _end(u_nullptr),
        _end_capacity(u_nullptr) {}

  explicit vector(size_type n, const value_type &val = value_type(),
                  const allocator_type &alloc = allocator_type())
      : _alloc(alloc) {
    this->_start = this->_alloc.allocate(n);
    this->_end = this->_start;
    this->_end_capacity = this->_start + n;
    for (; n > 0; n--) {
      this->_alloc.construct(this->_end, val);
      this->_end++;
    }
  }

  template < typename InputIterator >
  vector(InputIterator first, InputIterator last,
         const allocator_type &alloc = allocator_type(),
         typename ft::enable_if< !ft::is_integral< InputIterator >::value,
                                 InputIterator >::type * = u_nullptr)
      : _alloc(alloc),
        _start(u_nullptr),
        _end(u_nullptr),
        _end_capacity(u_nullptr) {
    difference_type n = ft::distance(first, last);
    this->_start = this->_alloc.allocate(n);
    this->_end_capacity = this->_start + n;
    this->_end = this->_start;
    for (; first != last; first++) {
      this->_alloc.construct(this->_end, *first);
      this->_end++;
    }
  }

  vector(const vector &x)
      : _alloc(x._alloc),
        _start(u_nullptr),
        _end(u_nullptr),
        _end_capacity(u_nullptr) {
    difference_type n = x._end - x._start;
    this->_start = this->_alloc.allocate(n);
    this->_end = this->_start;
    iterator tmp_start = x._start;
    iterator tmp_end = x._end;
    for (; tmp_start != tmp_end; tmp_start++) {
      this->_alloc.construct(this->_end, *tmp_start);
      this->_end++;
    }
    this->_end_capacity = this->_end;
  }

  ~vector() {
    this->clear();
    this->_alloc.deallocate(this->_start, this->capacity());
  }

  vector &operator=(const vector &x) {
    if (this != &x) {
      this->clear();
      this->assign(x.begin(), x.end());
    }
    return (*this);
  }

  iterator begin() { return (iterator(this->_start)); }
  const_iterator begin() const { return (const_iterator(this->_start)); }
  iterator end() { return (iterator(this->_end)); }
  const_iterator end() const { return (const_iterator(this->_end)); }
  reverse_iterator rbegin() { return (reverse_iterator(this->_end)); }
  const_reverse_iterator rbegin() const {
    return (const_reverse_iterator(this->_end));
  }
  reverse_iterator rend() { return (reverse_iterator(this->_start)); }
  const_reverse_iterator rend() const {
    return (const_reverse_iterator(this->_start));
  }

  size_type size() const { return (this->_end - this->_start); }
  size_type max_size() const { return (this->_alloc.max_size()); }

  // n개의 val값들을 갖는 vector로 resize한다.
  void resize(size_type n, value_type val = value_type()) {
    size_type prev_size = this->size();

    if (n > max_size()) throw(std::length_error("ft::vector::resize"));
    if (n <= this->size()) {
      while (n < this->size()) {
        this->_alloc.destroy(--this->_end);
      }
    } else {
      if (n <= this->capacity()) {
        while (n > this->size()) this->push_back(val);
      } else if (n <= this->capacity() * 2) {
        this->reserve(this->capacity() * 2);
        while (n-- - prev_size > 0) this->_alloc.construct(this->_end++, val);
      } else {
        this->reserve(n);
        while (n-- - prev_size > 0) this->_alloc.construct(this->_end++, val);
      }
    }
  }

  // element의 갯수가 아닌, 할당받은 메모리의 갯수. (잠재적 크기)
  size_type capacity() const { return (this->_end_capacity - this->_start); }

  bool empty() const {
    if (this->size() == 0)
      return (true);
    else
      return (false);
  }

  void reserve(size_type n) {
    if (n > max_size()) {
      throw(std::length_error("ft::vector::reserve"));
    }
    if (n > this->capacity()) {
      pointer prev_start = this->_start;
      pointer prev_end = this->_end;
      pointer prev_end_capacity = this->_end_capacity;

      this->_start = this->_alloc.allocate(n);
      this->_end_capacity = this->_start + n;
      this->_end = this->_start;
      for (pointer tmp = prev_start; tmp != prev_end; tmp++) {
        this->_alloc.construct(this->_end++, *tmp);
        this->_alloc.destroy(tmp);
      }
      this->_alloc.deallocate(prev_start, prev_end_capacity - prev_start);
    }
  }

  reference operator[](size_type n) { return (*(this->_start + n)); }
  const_reference operator[](size_type n) const {
    return (*(this->_start + n));
  }

  reference at(size_type n) {
    if (n >= this->size()) {
      throw(std::out_of_range("ft::vector::at"));
    }
    return ((*this)[n]);
  }

  const_reference at(size_type n) const {
    if (n >= this->size()) {
      throw(std::out_of_range("ft::vector::at"));
    }
    return ((*this)[n]);
  }

  // 벡터의 첫번째 element를 리턴.
  reference front() { return (*(this->_start)); }
  const_reference front() const { return (*(this->_start)); }

  // 벡터의 마지막 element를 리턴.
  reference back() { return (*(this->_end - 1)); }
  const_reference back() const { return (*(this->_end - 1)); }

  template < typename InputIterator >
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if< !ft::is_integral< InputIterator >::value,
                                      InputIterator >::type * = u_nullptr) {
    this->clear();
    size_type _size = ft::distance(first, last);
    if (this->capacity() >= _size) {
      while (first != last) {
        this->_alloc.construct(this->_end++, *first);
        first++;
      }
    } else {
      pointer prev_start = this->_start;
      pointer prev_end_capacity = this->_end_capacity;

      this->_start = this->_alloc.allocate(_size);
      this->_end_capacity = this->_start + _size;
      this->_end = this->_start;
      for (const_pointer tmp = &(*first); tmp != &(*last); tmp++) {
        this->_alloc.construct(this->_end++, *tmp);
      }
      this->_alloc.deallocate(prev_start, prev_end_capacity - prev_start);
    }
  }

  void assign(size_type n, const value_type &val) {
    this->clear();
    if (this->capacity() >= n) {
      while (n--) {
        this->_alloc.construct(this->_end++, val);
      }
    } else {
      pointer prev_start = this->_start;
      pointer prev_end_capacity = this->_end_capacity;

      this->_start = this->_alloc.allocate(n);
      this->_end_capacity = this->_start + n;
      this->_end = this->_start;
      while (n--) {
        this->_alloc.construct(this->_end++, val);
      }
      this->_alloc.deallocate(prev_start, prev_end_capacity - prev_start);
    }
  }

  // 벡터의 뒤에 새로운 element를 추가한다.
  // 늘어난 벡터의 크기가 capacity를 넘어갈 경우, 이전 capacity * 2의 크기로
  // 늘어남.
  void push_back(const value_type &val) {
    if (this->_end == this->_end_capacity) {
      if (this->_start == this->_end)
        this->reserve(1);
      else
        this->reserve(this->capacity() * 2);
    }
    this->_alloc.construct(this->_end, val);
    ++this->_end;
  }

  // 벡터의 맨 뒤 요소를 하나 제거한다.
  void pop_back() { this->_alloc.destroy(--this->_end); }

  iterator insert(iterator position, const value_type &val) {
    size_type position_idx = &(*position) - this->_start;
    this->insert(position, 1, val);
    return (this->begin() + position_idx);
  }

  void insert(iterator position, size_type n, const value_type &val) {
    size_type mv_back_count = this->_end - &(*position);

    this->resize(this->size() + n);
    pointer tmp = this->_end - 1;
    for (; mv_back_count > 0; mv_back_count--, tmp--) {
      this->_alloc.construct(tmp, *(tmp - n));
      this->_alloc.destroy(tmp - n);
    }
    while (n--) {
      this->_alloc.construct(tmp--, val);
    }
  }

  template < typename InputIterator >
  void insert(
      iterator position, InputIterator first, InputIterator last,
      typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type
          * = u_nullptr) {
    size_type n = ft::distance(first, last);
    size_type mv_back_count = this->_end - &(*position);
    this->resize(this->size() + n);
    pointer tmp = this->_end - 1;
    for (; mv_back_count > 0; mv_back_count--, tmp--) {
      this->_alloc.construct(tmp, *(tmp - n));
      this->_alloc.destroy(tmp - n);
    }
    while (first != last) {
      this->_alloc.construct(tmp--, *(--last));
    }
  }

  iterator erase(iterator position) {
    pointer location = (&(*position));

    this->_alloc.destroy(&(*position));
    --this->_end;
    for (pointer ptr = &(*position); ptr != this->_end; ptr++) {
      this->_alloc.construct(ptr, *(ptr + 1));
      this->_alloc.destroy(ptr + 1);
    }
    return (iterator(location));
  }

  iterator erase(iterator first, iterator last) {
    pointer location = &(*first);
    pointer f_ptr = &(*first), l_ptr = &(*last);
    size_type erase_size = l_ptr - f_ptr;

    for (; f_ptr != l_ptr; f_ptr++) {
      this->_alloc.destroy(f_ptr);
    }
    for (f_ptr = &(*first); l_ptr != this->_end; f_ptr++, l_ptr++) {
      this->_alloc.construct(f_ptr, *l_ptr);
      this->_alloc.destroy(l_ptr);
    }
    this->_end -= erase_size;
    return (iterator(location));
  }

  void swap(vector &x) {
    if (*this == x) return;

    pointer tmp_start = x._start;
    pointer tmp_end = x._end;
    pointer tmp_end_capacity = x._end_capacity;
    allocator_type tmp_alloc = x._alloc;

    x._start = this->_start;
    x._end = this->_end;
    x._end_capacity = this->_end_capacity;
    x._alloc = this->_alloc;

    this->_start = tmp_start;
    this->_end = tmp_end;
    this->_end_capacity = tmp_end_capacity;
    this->_alloc = tmp_alloc;
  }

  void clear() {
    while (this->_end != this->_start) {
      this->_alloc.destroy(--this->_end);
    }
  }

  allocator_type get_allocator() const { return (this->_alloc); }
};

template < typename T, typename Alloc >
bool operator==(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  if (lhs.size() != rhs.size()) return (false);

  typename ft::vector< T, Alloc >::const_iterator it_lhs = lhs.begin();
  typename ft::vector< T, Alloc >::const_iterator it_rhs = rhs.begin();

  for (; it_lhs != lhs.end(); it_lhs++, it_rhs++) {
    if (it_rhs == rhs.end() || *it_lhs != *it_rhs) return (false);
  }
  return (true);
}

template < typename T, typename Alloc >
bool operator!=(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  return (!(lhs == rhs));
}

template < typename T, typename Alloc >
bool operator<(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end()));
}

template < typename T, typename Alloc >
bool operator<=(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  return (!(rhs < lhs));
}

template < typename T, typename Alloc >
bool operator>(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  return (rhs < lhs);
}

template < typename T, typename Alloc >
bool operator>=(const vector< T, Alloc > &lhs, const vector< T, Alloc > &rhs) {
  return (!(lhs < rhs));
}

template < typename T, typename Alloc >
void swap(vector< T, Alloc > &x, vector< T, Alloc > &y) {
  x.swap(y);
}

}  // namespace ft

#endif
