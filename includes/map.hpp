/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunlee <hyunlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:37:24 by hyunlee           #+#    #+#             */
/*   Updated: 2021/11/26 19:41:00 by hyunlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(MAP_HPP)
#define MAP_HPP

#include "RB_Tree.hpp"

namespace ft {
template < class Key, class T, class Compare = ft::less< Key >,
           class Alloc = std::allocator< ft::pair< const Key, T > > >
class map {
 public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef ft::pair< const key_type, mapped_type > value_type;
  typedef Compare key_compare;

  class value_compare : ft::binary_function< value_type, value_type, bool > {
   protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

   public:
    typedef bool result_type;
    typedef value_type first_argument_type;
    typedef value_type second_argument_type;
    value_compare() : comp() {}
    bool operator()(const value_type& x, const value_type& y) const {
      return comp(x.first, y.first);
    }
  };

  typedef Alloc allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;

  typedef ft::RB_TreeIterator< value_type, value_type*, value_type& > iterator;
  typedef ft::RB_TreeIterator< value_type, const value_type*,
                               const value_type& >
      const_iterator;
  typedef ft::reverse_iterator< iterator > reverse_iterator;
  typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;
  typedef
      typename ft::iterator_traits< iterator >::difference_type difference_type;
  typedef size_t size_type;
  typedef
      typename ft::RB_Tree< value_type, value_compare >::node_type node_type;
  typedef ft::RB_Tree< value_type, value_compare > tree_type;

 private:
  tree_type _tree;
  key_compare _comp;
  allocator_type _alloc;

 public:
  explicit map(const key_compare& comp = key_compare(),
               const allocator_type& alloc = allocator_type())
      : _tree(), _comp(comp), _alloc(alloc) {}

  template < class InputIterator >
  map(InputIterator first, InputIterator last,
      const key_compare& comp = key_compare(),
      const allocator_type& alloc = allocator_type(),
      typename ft::enable_if< !ft::is_integral< InputIterator >::value,
                              InputIterator >::type* = u_nullptr)
      : _tree(), _comp(comp), _alloc(alloc) {
    insert(first, last);
  }

  map(const map& x) : _tree(), _comp(x._comp), _alloc(x._alloc) { *this = x; }

  virtual ~map() {}

  map& operator=(const map& x) {
    if (this != &x) {
      this->_tree.copy(x._tree);
    }
    return (*this);
  }

  iterator begin() { return (iterator(_tree.get_front_node())); }
  const_iterator begin() const {
    return (const_iterator(_tree.get_front_node()));
  }
  iterator end() { return (iterator(_tree.get_nil())); }
  const_iterator end() const { return (const_iterator(_tree.get_nil())); }

  reverse_iterator rbegin() { return (reverse_iterator(_tree.get_nil())); }
  const_reverse_iterator rbegin() const {
    return (const_reverse_iterator(_tree.get_nil()));
  }
  reverse_iterator rend() { return (reverse_iterator(_tree.get_front_node())); }
  const_reverse_iterator rend() const {
    return (const_reverse_iterator(_tree.get_front_node()));
  }

  bool empty() const { return (_tree.is_empty_tree()); }
  size_type size() const { return (_tree.size()); }
  size_type max_size() const {
    typename tree_type::node_alloc_type _node_alloc;
    return (_node_alloc.max_size());
  }

  /* tree?????? k ?????? ?????? ?????? ???????????? ???????????? ?????????. (insert ??????.)
  ????????? ????????? ?????? ??????????????? ???????????????. */
  mapped_type& operator[](const key_type& k) {
    ft::pair< node_type*, bool > result =
        _tree.insert(value_type(k, mapped_type()));
    return ((result.first)->value->second);
  }

  /**
   * insert ?????????
   * pair(????????? element??? ???????????? iterator, true) ??????.
   * insert ?????????
   * pair(val??? ????????? Key??? ???????????? iterator, false) ??????.
   */
  pair< iterator, bool > insert(const value_type& val) {
    ft::pair< node_type*, bool > result = _tree.insert(val);
    return (ft::make_pair(iterator(result.first), result.second));
  }

  /**
   * position??? hint, ?????? ??????.
   * insert ?????????
   * ????????? element ???????????? iterator ??????.
   * insert ?????????
   * val??? ????????? Key??? ???????????? iterator ??????.
   */
  iterator insert(iterator position, const value_type& val) {
    ft::pair< node_type*, bool > result = _tree.insert(val, position.base());
    return (iterator(result.first));
  }

  /**
   * [first, last) ????????? element??? insert.
   * ??????, ?????? Key??? ???????????? element??? ?????? X.
   * ???????????? ?????? element??? ????????????.
   */
  template < class InputIterator >
  void insert(InputIterator first, InputIterator last,
              typename ft::enable_if< !ft::is_integral< InputIterator >::value,
                                      InputIterator >::type* = u_nullptr) {
    while (first != last) {
      _tree.insert(*first);
      first++;
    }
  }

  void erase(iterator position) { _tree.erase(position.base()); }

  /**
   * erase ?????? ??? 1, ?????? ??? 0 ??????.
   * tree??? ?????? key_type k??? ????????? ?????? search?????? nil????????? ????????????.
   * nil?????? erase????????? ??????.
   */
  size_type erase(const key_type& k) {
    return (_tree.erase(_tree.search(value_type(k, mapped_type()))));
  }

  void erase(iterator first, iterator last) {
    while (first != last) {
      erase(first++);
    }
  }

  void swap(map& x) {
    if (*this == x) {
      return;
    }

    unsigned char buf[sizeof(map)];
    memcpy(buf, reinterpret_cast< void* >(&x), sizeof(map));
    memcpy(reinterpret_cast< unsigned char* >(&x),
           reinterpret_cast< void* >(this), sizeof(map));
    memcpy(reinterpret_cast< unsigned char* >(this),
           reinterpret_cast< void* >(buf), sizeof(map));
  }

  void clear() { _tree.clear(); }

  key_compare key_comp() const { return (key_compare()); }
  value_compare value_comp() const { return (value_compare()); }

  iterator find(const key_type& k) {
    return (iterator(_tree.search(value_type(k, mapped_type()))));
  }

  const_iterator find(const key_type& k) const {
    return (const_iterator(_tree.search(value_type(k, mapped_type()))));
  }

  /* k Key ????????? 1, ????????? 0.
  map??? Key??? unique???????????????, 1 ?????? 0??????. */
  size_type count(const key_type& k) const {
    if (_tree.search(value_type(k, mapped_type()))->is_nil()) {
      return 0;
    }
    return 1;
  }

  iterator lower_bound(const key_type& k) {
    return (iterator(_tree.lower_bound(value_type(k, mapped_type()))));
  }
  const_iterator lower_bound(const key_type& k) const {
    return (const_iterator(_tree.lower_bound(value_type(k, mapped_type()))));
  }
  iterator upper_bound(const key_type& k) {
    return (iterator(_tree.upper_bound(value_type(k, mapped_type()))));
  }
  const_iterator upper_bound(const key_type& k) const {
    return (const_iterator(_tree.upper_bound(value_type(k, mapped_type()))));
  }

  pair< const_iterator, const_iterator > equal_range(const key_type& k) const {
    return (ft::make_pair(lower_bound(k), upper_bound(k)));
  }
  pair< iterator, iterator > equal_range(const key_type& k) {
    return (ft::make_pair(lower_bound(k), upper_bound(k)));
  }

  allocator_type get_allocator() const { return (allocator_type()); }

  void showTree() { _tree.showMap(); }
};

template < class Key, class T, class Compare, class Alloc >
bool operator==(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs) {
  return (lhs.size() == rhs.size() &&
          ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template < class Key, class T, class Compare, class Alloc >
bool operator!=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs) {
  return (!(lhs == rhs));
}

template < class Key, class T, class Compare, class Alloc >
bool operator<(const map< Key, T, Compare, Alloc >& lhs,
               const map< Key, T, Compare, Alloc >& rhs) {
  return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end()));
}

template < class Key, class T, class Compare, class Alloc >
bool operator<=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs) {
  return (!(rhs < lhs));
}

template < class Key, class T, class Compare, class Alloc >
bool operator>(const map< Key, T, Compare, Alloc >& lhs,
               const map< Key, T, Compare, Alloc >& rhs) {
  return (rhs < lhs);
}

template < class Key, class T, class Compare, class Alloc >
bool operator>=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs) {
  return (!(lhs < rhs));
}

template < class Key, class T, class Compare, class Alloc >
void swap(map< Key, T, Compare, Alloc >& x, map< Key, T, Compare, Alloc >& y) {
  x.swap(y);
}

}  // namespace ft

#endif  // MAP_HPP
