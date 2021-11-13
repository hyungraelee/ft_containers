#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

#include "VectorIterator.hpp"
#include "utils.hpp"

namespace ft {

template < typename T, typename Allocator = std::allocator< T > >
class vector {
 public:
  //=+=+=+=+=+=+= Typedefs =+=+=+=+=+=+=//
  /**
   * @brief A type that represents the data type stored in a vector.
   */
  typedef T value_type;

  /**
   * @brief A type that represents the allocator class for the vector object.
   */
  typedef Allocator allocator_type;

  /**
   * @brief A type that provides a reference to an element stored in a vector.
   */
  typedef typename allocator_type::reference reference;

  /**
   * @brief A type that provides a reference to a const element stored in a
   * vector. It's used for reading and doing const operations.
   */
  typedef typename allocator_type::const_reference const_reference;

  /**
   * @brief A type that provides a pointer to an element in a vector.
   */
  typedef typename allocator_type::pointer pointer;

  /**
   * @brief A type that provides a pointer to a const element in a vector.
   */
  typedef typename allocator_type::const_pointer const_pointer;

  /**
   * @brief A type that provides a random-access iterator that can read or
   * modify any element in a vector.
   */
  typedef ft::VectorIterator< T > iterator;
  /**
   * @brief A type that provides a random-access iterator that can read a const
   * element in a vector.
   */
  typedef ft::VectorIterator< const T > const_iterator;
  /**
   * @brief A type that provides a random-access iterator that can read or
   * modify any element in a reversed vector.
   */
  typedef ft::reverse_iterator< iterator > reverse_iterator;
  /**
   * @brief A type that provides a random-access iterator that can read any
   * const element in the vector.
   */
  typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

  /**
   * @brief A type that counts the number of elements in a vector. -> size_t
   */
  typedef typename allocator_type::size_type size_type;

  /**
   * @brief A type that provides the difference between the addresses of two
   * elements in a vector. -> ptrdiff_t
   */
  typedef typename allocator_type::difference_type difference_type;

 private:
  allocator_type _alloc;
  pointer _start;
  pointer _end;
  pointer _end_capacity;

 public:
  //=+=+=+=+=+=+= Member functions =+=+=+=+=+=+=//
  // empty container constructor (default constructor)
  explicit vector(const allocator_type &alloc = allocator_type())
      : _alloc(alloc),
        _start(u_nullptr),
        _end(u_nullptr),
        _end_capacity(u_nullptr) {}

  // fill constructor
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

  // range constructor
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

  // copy constructor
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

  // destructor
  ~vector() {
    this->clear();
    this->_alloc.deallocate(this->_start, this->capacity());
  }

  // assign operator
  vector &operator=(const vector &x) {
    if (this != &x) {
      this->clear();
      this->assign(x.begin(), x.end());
    }
    return (*this);
  }

  //=+=+=+=+=+=+= Iterators =+=+=+=+=+=+=//
  // Returns an iterator pointing to the first element in the vector.
  iterator begin() { return (iterator(this->_start)); }
  const_iterator begin() const { return (const_iterator(this->_start)); }

  // Returns an iterator referring to the past-the-end element in the vector
  // container.
  iterator end() { return (iterator(this->_end)); }
  const_iterator end() const { return (const_iterator(this->_end)); }

  /* Returns a reverse iterator pointing to the last element in the vector
  (i.e., its reverse beginning). */
  reverse_iterator rbegin() { return (reverse_iterator(this->_end)); }
  const_reverse_iterator rbegin() const {
    return (const_reverse_iterator(this->_end));
  }

  /* Returns a reverse iterator pointing to the theoretical element preceding
  the first element in the vector (which is considered its reverse end). */
  reverse_iterator rend() { return (reverse_iterator(this->_start)); }
  const_reverse_iterator rend() const {
    return (const_reverse_iterator(this->_start));
  }

  //=+=+=+=+=+=+= Capacity =+=+=+=+=+=+=//
  // Returns the number of elements in the vector.
  size_type size() const { return (this->_end - this->_start); }

  /* Returns the maximum number of elements that the vector can hold.
  This is not the amount of storage space currently allocated to the vector
  (this can be obtained with member vector::capacity),
  but the maximum potential size the vector could reach due to system or library
  implementation limitations. */
  size_type max_size() const { return (this->_alloc.max_size()); }

  // Resizes the container so that it contains n elements.
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

  // Return size of allocated storage capacity.
  size_type capacity() const { return (this->_end_capacity - this->_start); }

  // Returns whether the vector is empty (i.e. whether its size is 0).
  bool empty() const {
    if (this->size() == 0)
      return (true);
    else
      return (false);
  }

  // Requests that the vector capacity be at least enough to contain n elements.
  void reserve(size_type n) {
    if (n > max_size()) throw(std::length_error("ft::vector::reserve"));
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

  //=+=+=+=+=+=+= Element access =+=+=+=+=+=+=//
  // Returns a reference to the element at position n in the vector container.
  reference operator[](size_type n) { return (*(this->_start + n)); }
  const_reference operator[](size_type n) const {
    return (*(this->_start + n));
  }

  // Returns a reference to the element at position n in the vector.
  reference at(size_type n) {
    if (n >= this->size()) throw(std::out_of_range("ft::vector::at"));
    return ((*this)[n]);
  }
  const_reference at(size_type n) const {
    if (n >= this->size()) throw(std::out_of_range("ft::vector::at"));
    return ((*this)[n]);
  }

  // Returns a reference to the first element in the vector.
  reference front() { return (*(this->_start)); }
  const_reference front() const { return (*(this->_start)); }

  // Returns a reference to the last element in the vector.
  reference back() { return (*(this->_end - 1)); }
  const_reference back() const { return (*(this->_end - 1)); }

  //=+=+=+=+=+=+= Modifiers =+=+=+=+=+=+=//
  /* Assigns new contents to the vector, replacing its current contents,
  and modifying its size accordingly. */
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

  /* Adds a new element at the end of the vector, after its current last
  element. The content of val is copied (or moved) to the new element. */
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

  // Removes the last element in the vector, effectively reducing the container
  // size by one.
  void pop_back() { this->_alloc.destroy(--this->_end); }

  /* The vector is extended by inserting new elements before the element at the
  specified position,
  effectively increasing the container size by the number of elements inserted.
*/
  iterator insert(iterator position, const value_type &val) {
    size_type position_idx = &(*position) - this->_start;
    this->insert(position, 1, val);
    return (this->begin() + position_idx);

    // iterator location = position;
    // pointer prev_start = this->_start;
    // pointer prev_end = this->_end;
    // size_type prev_capacity = this->capacity();

    // if (this->_end == this->_end_capacity) {
    //   if (this->_start == this->_end) {
    //     this->_start = this->_alloc.allocate(1);
    //     this->_alloc.construct(&(*position), val);
    //     ++this->_end;
    //     ++this->_end_capacity;
    //   } else {
    //     this->_start = this->_alloc.allocate(prev_capacity * 2);
    //     this->_end_capacity = this->_start + (prev_capacity * 2);
    //     this->_end = this->_start;
    //     for (; prev_start != &(*position); prev_start++) {
    //       this->_alloc.construct(this->_end++, *prev_start);
    //       this->_alloc.destroy(prev_start);
    //     }
    //     this->_alloc.construct(this->_end++, val);
    //     for (; prev_start != prev_end; prev_start++) {
    //       this->_alloc.construct(this->_end++, *prev_start);
    //       this->_alloc.destroy(prev_start);
    //     }
    //     this->_alloc.deallocate(prev_start, prev_capacity);
    //   }
    // } else {
    //   ++this->_end;
    //   for (; prev_end != &(*position); prev_end--) {
    //     this->_alloc.construct(prev_end, *(prev_end - 1));
    //     this->_alloc.destroy(prev_end - 1);
    //   }
    //   this->_alloc.construct(prev_end, val);
    // }
    // return (location);
  }

  void insert(iterator position, size_type n, const value_type &val) {
    size_type mv_back_count = this->_end - &(*position);

    this->resize(this->size() + n);
    pointer tmp = this->_end - 1;
    for (; mv_back_count > 0; mv_back_count--, tmp--) {
      this->_alloc.construct(tmp, *(tmp - n));
      this->_alloc.destroy(tmp - n);
    }
    while (n--) this->_alloc.construct(tmp--, val);
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

  // Removes from the vector either a single element (position) or a range of
  // elements ([first,last)).
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

  /* Exchanges the content of the container by the content of x,
  which is another vector object of the same type. Sizes may differ. */
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

  /* Removes all elements from the vector (which are destroyed),
  leaving the container with a size of 0. */
  void clear() {
    while (this->_end != this->_start) {
      this->_alloc.destroy(--this->_end);
    }
  }

  //=+=+=+=+=+=+= Allocator =+=+=+=+=+=+=//
  // Returns a copy of the allocator object associated with the vector.
  allocator_type get_allocator() const { return (this->_alloc); }
};

//=+=+=+=+=+=+= Non-member function overloads =+=+=+=+=+=+=//

//=+=+=+=+=+=+= Relational Operators =+=+=+=+=+=+=//
/* The equality comparison (operator==) is performed by first comparing sizes,
and if they match, the elements are compared sequentially using operator==,
stopping at the first mismatch (as if using algorithm equal). */
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

/* The less-than comparison (operator<) behaves as if using algorithm
lexicographical_compare, which compares the elements sequentially using
operator< in a reciprocal manner
(i.e., checking both a<b and b<a) and stopping at the first occurrence. */
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

//=+=+=+=+=+=+= Swap =+=+=+=+=+=+=//
/* The contents of container x are exchanged with those of y.
Both container objects must be of the same type (same template parameters),
although sizes may differ. */
template < typename T, typename Alloc >
void swap(vector< T, Alloc > &x, vector< T, Alloc > &y) {
  x.swap(y);
}

//=+=+=+=+=+=+= Template specializations =+=+=+=+=+=+=//

// vector<bool> ?????????

}  // namespace ft

#endif
