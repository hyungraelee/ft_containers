#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

namespace ft
{

	template <typename T, typename Allocator = std::allocator<T> >
	class vector
	{
	public:
		//===------------------------------ Typedefs --------------------------------===//
		// A type that represents the data type stored in a vector.
		typedef T value_type;
		// A type that represents the allocator class for the vector object.
		typedef Allocator allocator_type;
		// A type that provides a reference to an element stored in a vector.
		typedef typename allocator_type::reference reference;
		// A type that provides a reference to a const element stored in a vector. It's used for reading and doing const operations.
		typedef typename allocator_type::const_reference const_reference;
		// A type that provides a pointer to an element in a vector.
		typedef typename allocator_type::pointer pointer;
		// A type that provides a pointer to a const element in a vector.
		typedef typename allocator_type::const_pointer const_pointer;
		// A type that provides a random-access iterator that can read or modify any element in a vector.
		typedef typename pointer iterator;
		// A type that provides a random-access iterator that can read a const element in a vector.
		typedef typename const_pointer const_iterator;
		// A type that provides a random-access iterator that can read or modify any element in a reversed vector.
		typedef std::reverse_iterator<iterator> reverse_iterator;
		// A type that provides a random-access iterator that can read any const element in the vector.
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		// A type that counts the number of elements in a vector. -> size_t
		typedef typename allocator_type::size_type size_type;
		// A type that provides the difference between the addresses of two elements in a vector. -> ptrdiff_t
		typedef typename allocator_type::difference_type difference_type;

	private:
		allocator_type _alloc;
		size_type _size;
		size_type _capacity;
		pointer _vec;

	public:
		//===------------------------------ Member functions --------------------------------===//
		// empty container constructor (default constructor)
		explicit vector(const allocator_type &alloc = allocator_type())
			: _alloc(alloc), _size(0), _capacity(0)
		{
			_vec = _alloc.allocate(0);
		}

		// fill constructor
		explicit vector(size_type n, const value_type &val = value_type(),
						const allocator_type &alloc = allocator_type()) : _alloc(alloc), _size(n), _capacity(n)
		{
			_vec = _alloc.allocate(sizeof(value_type) * n);
			for (size_type i = 0; i < n; i++)
			{
				_vec[i] = val;
			}
		}

		// range constructor
		template <typename InputIterator>
		vector(InputIterator first, InputIterator last,
			   const allocator_type &alloc = allocator_type());

		// copy constructor
		vector(const vector &x);

		// destructor
		~vector();

		// assign operator
		vector &operator=(const vector &);

		//===------------------------------ Iterators --------------------------------===//
		// Returns an iterator pointing to the first element in the vector.
		iterator begin();

		const_iterator begin() const
			return (static_cast<const_iterator>(_vec));

		// Returns an iterator referring to the past-the-end element in the vector container.
		iterator end();
		const_iterator end() const;

		/* Returns a reverse iterator pointing to the last element in the vector
		(i.e., its reverse beginning). */
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;

		/* Returns a reverse iterator pointing to the theoretical element preceding the first element in the vector
		(which is considered its reverse end). */
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		//===------------------------------ Capacity --------------------------------===//
		// Returns the number of elements in the vector.
		size_type size() const
		{
			return (this->_size);
		}

		/* Returns the maximum number of elements that the vector can hold.
		This is not the amount of storage space currently allocated to the vector
		(this can be obtained with member vector::capacity),
		but the maximum potential size the vector could reach due to system or library implementation limitations. */
		size_type max_size() const
		{
			return (std::numeric_limits<size_type>::max() / sizeof(value_type));
		}

		// Resizes the container so that it contains n elements.
		void resize(size_type n, value_type val = value_type());

		// Return size of allocated storage capacity.
		size_type capacity() const
		{
			return (this->_capacity);
		}

		// Returns whether the vector is empty (i.e. whether its size is 0).
		bool empty() const
		{
			if (this->_size == 0)
				return (true);
			else
				return (false);
		}

		// Requests that the vector capacity be at least enough to contain n elements.
		void reserve(size_type n);

		//===------------------------------ Element access --------------------------------===//
		// Returns a reference to the element at position n in the vector container.
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;

		// Returns a reference to the element at position n in the vector.
		reference at(size_type n);
		const_reference at(size_type n) const;

		// Returns a reference to the first element in the vector.
		reference front();
		const_reference front() const;

		// Returns a reference to the last element in the vector.
		reference back();
		const_reference back() const;

		//===------------------------------ Modifiers --------------------------------===//
		/* Assigns new contents to the vector, replacing its current contents,
		and modifying its size accordingly. */
		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const value_type &val);

		/* Adds a new element at the end of the vector, after its current last element.
		The content of val is copied (or moved) to the new element. */
		void push_back(const value_type &val);

		// Removes the last element in the vector, effectively reducing the container size by one.
		void pop_back();

		/* The vector is extended by inserting new elements before the element at the specified position,
		effectively increasing the container size by the number of elements inserted. */
		iterator insert(iterator position, const value_type &val);
		void insert(iterator position, size_type n, const value_type &val);
		template <typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		// Removes from the vector either a single element (position) or a range of elements ([first,last)).
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		/* Exchanges the content of the container by the content of x,
		which is another vector object of the same type. Sizes may differ. */
		void swap(vector &x);

		/* Removes all elements from the vector (which are destroyed),
		leaving the container with a size of 0. */
		void clear();

		//===------------------------------ Allocator --------------------------------===//
		// Returns a copy of the allocator object associated with the vector.
		allocator_type get_allocator() const;

	private:
	};

	//===------------------------------ Non-member function overloads --------------------------------===//

	//===------------------------------ Relational Operators --------------------------------===//
	/* The equality comparison (operator==) is performed by first comparing sizes,
	and if they match, the elements are compared sequentially using operator==,
	stopping at the first mismatch (as if using algorithm equal). */
	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
	template <typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

	/* The less-than comparison (operator<) behaves as if using algorithm lexicographical_compare,
	which compares the elements sequentially using operator< in a reciprocal manner
	(i.e., checking both a<b and b<a) and stopping at the first occurrence. */
	template <typename T, typename Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
	template <typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
	template <typename T, typename Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
	template <typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

	//===------------------------------ Swap --------------------------------===//
	/* The contents of container x are exchanged with those of y.
	Both container objects must be of the same type (same template parameters), although sizes may differ. */
	template <typename T, typename Alloc>
	void swap(vector<T, Alloc> &x, vector<T, Alloc> &y);

	//===------------------------------ Template specializations --------------------------------===//
	// vector<bool> ?????????

}

#endif
