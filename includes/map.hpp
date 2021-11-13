#if !defined(MAP_HPP)
#define MAP_HPP

// #include <memory>

#include "RB_Tree.hpp"
// #include "RB_TreeIterator.hpp"
// #include "RB_TreeNode.hpp"
// #include "utils.hpp"

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
  typedef ft::RB_TreeIterator< value_type, const value_type*, const value_type& > const_iterator;
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

  map(const map& x) : _tree() { *this = x; }

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

  /* tree에서 k 키를 갖는 노드 만든다고 가정하고 들어감. (insert 사용.)
  어차피 있으면 해당 이터레이터 리턴할것임. */
  mapped_type& operator[](const key_type& k) {
    ft::pair< node_type*, bool > result =
        _tree.insert(value_type(k, mapped_type()));
    return ((result.first)->value->second);
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
    return (_tree.erase(_tree.search(value_type(k, mapped_type()))));
  }

  void erase(iterator first, iterator last) {
    while (first != last) {
      _tree.erase(first.base());
      ++first;
    }
  }

  void swap(map& x) {
    tree_type save;

    save = this->_tree;
    this->_tree = x._tree;
    x._tree = save;
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

  /* k Key 있으면 1, 없으면 0.
  map은 Key가 unique하기떄문에, 1 또는 0이다. */
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
};

template < class Key, class T, class Compare, class Alloc >
bool operator==(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
bool operator!=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
bool operator<(const map< Key, T, Compare, Alloc >& lhs,
               const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
bool operator<=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
bool operator>(const map< Key, T, Compare, Alloc >& lhs,
               const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
bool operator>=(const map< Key, T, Compare, Alloc >& lhs,
                const map< Key, T, Compare, Alloc >& rhs);

template < class Key, class T, class Compare, class Alloc >
void swap(map< Key, T, Compare, Alloc >& x, map< Key, T, Compare, Alloc >& y);
}  // namespace ft

#endif  // MAP_HPP
