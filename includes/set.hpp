#if !defined(SET_HPP)
#define SET_HPP

#include "RB_Tree.hpp"
#include "printTree.hpp"

namespace ft {
template < class T, class Compare = ft::less< T >,
           class Alloc = std::allocator< T > >
class set {
 public:
  typedef T key_type;
  typedef T value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
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
  explicit set(const key_compare& comp = key_compare(),
               const allocator_type& alloc = allocator_type())
      : _tree(), _comp(comp), _alloc(alloc) {}

  template < class InputIterator >
  set(InputIterator first, InputIterator last,
      const key_compare& comp = key_compare(),
      const allocator_type& alloc = allocator_type(),
      typename ft::enable_if< !ft::is_integral< InputIterator >::value,
                              InputIterator >::type* = u_nullptr)
      : _tree(), _comp(comp), _alloc(alloc) {
    insert(first, last);
  }

  set(const set& x) : _tree(), _comp(x._comp), _alloc(x._alloc) { *this = x; }

  virtual ~set() {}

  set& operator=(const set& x) {
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
    // return (_tree.max_size());
  }

  /**
   * insert 성공시
   * pair(새로운 element를 가리키는 iterator, true) 리턴.
   * insert 실패시
   * pair(val과 동일한 Key값 갖고있는 iterator, false) 리턴.
   */
  pair< iterator, bool > insert(const value_type& val) {
    ft::pair< node_type*, bool > result = _tree.insert(val);
    return (ft::make_pair(iterator(result.first), result.second));
  }

  /**
   * position은 hint, 강제 아님.
   * insert 성공시
   * 새로운 element 가리키는 iterator 리턴.
   * insert 실패시
   * val과 동일한 Key값 갖고있는 iterator 리턴.
   */
  iterator insert(iterator position, const value_type& val) {
    ft::pair< node_type*, bool > result = _tree.insert(val, position.base());
    return (iterator(result.first));
  }

  /**
   * [first, last) 구간의 element를 insert.
   * 이때, 기존 Key와 중복되는 element는 추가 X.
   * 중복되지 않는 element만 추가한다.
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

  size_type erase(const key_type& k) {
    return (_tree.erase(_tree.search(value_type(k))));
  }

  void erase(iterator first, iterator last) {
    while (first != last) {
      iterator tmp = first;
      ++first;
      erase(tmp);
    }
  }

  void swap(set& x) {
    if (*this == x) {
      return ;
    }

    unsigned char buf[sizeof(set)];
    memcpy(buf, reinterpret_cast< void* >(&x), sizeof(set));
    memcpy(reinterpret_cast< unsigned char* >(&x),
           reinterpret_cast< void* >(this), sizeof(set));
    memcpy(reinterpret_cast< unsigned char* >(this),
           reinterpret_cast< void* >(buf), sizeof(set));
  }

  void clear() { _tree.clear(); }

  key_compare key_comp() const { return (key_compare()); }
  value_compare value_comp() const { return (value_compare()); }

  iterator find(const key_type& k) {
    return (iterator(_tree.search(value_type(k))));
  }

  const_iterator find(const key_type& k) const {
    return (const_iterator(_tree.search(value_type(k))));
  }

  /* k Key 있으면 1, 없으면 0.
  map은 Key가 unique하기떄문에, 1 또는 0이다. */
  size_type count(const key_type& k) const {
    if (_tree.search(value_type(k))->is_nil()) {
      return 0;
    }
    return 1;
  }

  iterator lower_bound(const key_type& k) {
    return (iterator(_tree.lower_bound(value_type(k))));
  }
  const_iterator lower_bound(const key_type& k) const {
    return (const_iterator(_tree.lower_bound(value_type(k))));
  }
  iterator upper_bound(const key_type& k) {
    return (iterator(_tree.upper_bound(value_type(k))));
  }
  const_iterator upper_bound(const key_type& k) const {
    return (const_iterator(_tree.upper_bound(value_type(k))));
  }

  pair< const_iterator, const_iterator > equal_range(const key_type& k) const {
    return (ft::make_pair(lower_bound(k), upper_bound(k)));
  }
  pair< iterator, iterator > equal_range(const key_type& k) {
    return (ft::make_pair(lower_bound(k), upper_bound(k)));
  }

  allocator_type get_allocator() const { return (allocator_type()); }

  void showTree() { _tree.showSet(); }
};

template < class T, class Compare, class Alloc >
bool operator==(const set< T, Compare, Alloc >& lhs,
                const set< T, Compare, Alloc >& rhs) {
  return (lhs.size() == rhs.size() &&
          ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template < class T, class Compare, class Alloc >
bool operator!=(const set< T, Compare, Alloc >& lhs,
                const set< T, Compare, Alloc >& rhs) {
  return (!(lhs == rhs));
}

template < class T, class Compare, class Alloc >
bool operator<(const set< T, Compare, Alloc >& lhs,
               const set< T, Compare, Alloc >& rhs) {
  return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end()));
}

template < class T, class Compare, class Alloc >
bool operator<=(const set< T, Compare, Alloc >& lhs,
                const set< T, Compare, Alloc >& rhs) {
  return (!(rhs < lhs));
}

template < class T, class Compare, class Alloc >
bool operator>(const set< T, Compare, Alloc >& lhs,
               const set< T, Compare, Alloc >& rhs) {
  return (rhs < lhs);
}

template < class T, class Compare, class Alloc >
bool operator>=(const set< T, Compare, Alloc >& lhs,
                const set< T, Compare, Alloc >& rhs) {
  return (!(lhs < rhs));
}

template < class T, class Compare, class Alloc >
void swap(set< T, Compare, Alloc >& x, set< T, Compare, Alloc >& y) {
  x.swap(y);
}

}  // namespace ft

#endif  // MAP_HPP
