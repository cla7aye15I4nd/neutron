#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
	template<typename T>
	class vector {

	private:
		size_t _size;
		size_t _capacity;
		T ** _elem;

		void doubleSpace() {
			_capacity *= 2;

			T **tmp;
			tmp = new T *[_capacity];

			for (size_t i = 0; i < _size; ++i)
				tmp[i] = new T(*_elem[i]);

			for (size_t i = 0; i < _size; ++i)
				delete _elem[i];
			delete [] _elem;

			_elem = tmp;
		}

	public:
		/**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
		/**
         * you can see RandomAccessIterator at CppReference for help.
         */
		class const_iterator;
		class iterator {
			friend class vector<T>;
		private:
			/**
             * TODO add data members
             *   just add whatever you want.
             */
			int ptr;
			vector <T> *vec;
		public:
			/**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
			iterator(int p, vector <T> *v) : ptr(p), vec(v) {}

			iterator(const iterator &o) : ptr(o.ptr), vec(o.vec) {}

			iterator operator+(const int &n) const {
				//TODO
				iterator tmp(ptr + n, vec);
				return tmp;
			}
			iterator operator-(const int &n) const {
				//TODO
				iterator tmp(ptr - n, vec);
				return tmp;
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const {
				//TODO
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr - rhs.ptr;
			}
			iterator operator+=(const int &n) {
				//TODO
				ptr += n;
				return *this;
			}
			iterator operator-=(const int &n) {
				//TODO
				ptr -= n;
				return *this;
			}
			/**
             * TODO iter++
             */
			iterator operator++(int) {
				iterator tmp = *this;
				ptr++;
				return tmp;
			}
			/**
             * TODO ++iter
             */
			iterator& operator++() {
				ptr++;
				return *this;
			}
			/**
             * TODO iter--
             */
			iterator operator--(int) {
				iterator tmp = *this;
				ptr--;
				return tmp;
			}
			/**
             * TODO --iter
             */
			iterator& operator--() {
				ptr--;
				return *this;
			}
			/**
             * TODO *it
             */
			T& operator*() const{
				return *(vec->_elem[ptr]);
			}
			/**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
			bool operator==(const iterator &rhs) const {
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr == rhs.ptr;
			}
			bool operator==(const const_iterator &rhs) const {
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr == rhs.ptr;
			}
			/**
             * some other operator for iterator.
             */
			bool operator!=(const iterator &rhs) const {
				return ptr != rhs.ptr;
			}
			bool operator!=(const const_iterator &rhs) const {
				return ptr != rhs.ptr;
			}
		};
		/**
         * TODO
         * has same function as iterator, just for a const object.
         */
		class const_iterator {
			friend class vector<T>;
		private:
			/**
             * TODO add data members
             *   just add whatever you want.
             */
			int ptr;
			const vector <T> *vec;
		public:
			/**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
			const_iterator(int p, const vector <T> *v) : ptr(p), vec(v) {}

			const_iterator(const const_iterator &o) : ptr(o.ptr), vec(o.vec) {}

			const_iterator operator+(const int &n) const {
				//TODO
				const_iterator tmp(ptr + n, vec);
				return tmp;
			}
			const_iterator operator-(const int &n) const {
				//TODO
				const_iterator tmp(ptr - n, vec);
				return tmp;
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const {
				//TODO
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr - rhs.ptr;
			}
			const_iterator operator+=(const int &n) {
				//TODO
				ptr += n;
				return *this;
			}
			const_iterator operator-=(const int &n) {
				//TODO
				ptr -= n;
				return *this;
			}
			/**
             * TODO iter++
             */
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				ptr++;
				return tmp;
			}
			/**
             * TODO ++iter
             */
			const_iterator& operator++() {
				ptr++;
				return *this;
			}
			/**
             * TODO iter--
             */
			const_iterator operator--(int) {
				const_iterator tmp = *this;
				ptr--;
				return tmp;
			}
			/**
             * TODO --iter
             */
			const_iterator& operator--() {
				ptr--;
				return *this;
			}
			/**
             * TODO *it
             */
			const T& operator*() const{
				return *(vec->_elem[ptr]);
			}
			/**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
			bool operator==(const iterator &rhs) const {
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr == rhs.ptr;
			}
			bool operator==(const const_iterator &rhs) const {
				if (vec != rhs.vec)
					throw invalid_iterator();

				return ptr == rhs.ptr;
			}
			/**
             * some other operator for iterator.
             */
			bool operator!=(const iterator &rhs) const {
				return ptr != rhs.ptr;
			}
			bool operator!=(const const_iterator &rhs) const {
				return ptr != rhs.ptr;
			}

		};
		/**
         * TODO Constructs
         * Atleast three: default constructor, copy constructor and a constructor for std::vector
         */
		vector() {
			_size = 0;
			_capacity = 10;
			_elem = new T *[_capacity];
		}
		vector(const vector &other) {
			_size = other._size;
			_capacity = other._capacity;

			_elem = new T *[_capacity];

			for (size_t i = 0; i < _size; ++i)
				_elem[i] = new T(*(other._elem[i]));
		}
		/**
         * TODO Destructor
         */
		~vector() {
			for (size_t i = 0; i < _size; ++i)
				delete _elem[i];

			delete [] _elem;
		}
		/**
         * TODO Assignment operator
         */
		vector &operator=(const vector &other) {
			if (this == &other)
				return *this;

			for (size_t i = 0; i < _size; ++i)
				delete _elem[i];
			delete [] _elem;

			_size = other._size;
			_capacity = other._capacity;

			_elem = new T *[_capacity];
			for (size_t i = 0; i < _size; ++i)
				_elem[i] = new T(*(other._elem[i]));

			return *this;
		}
		/**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
		T & at(const size_t &pos) {
			if (pos >= _size || pos < 0)
				throw index_out_of_bound();

			return *_elem[pos];
		}
		const T & at(const size_t &pos) const {
			if (pos >= _size || pos < 0)
				throw index_out_of_bound();

			return *_elem[pos];
		}
		/**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
		T & operator[](const size_t &pos) {
		    if (pos >= _size)
		    	throw index_out_of_bound();
			    return *_elem[pos];
		}
		const T & operator[](const size_t &pos) const {
		    if (pos >= _size)
		    	throw index_out_of_bound();
			    return *_elem[pos];
		}
		/**
         * access the first element.
         * throw container_is_empty if size == 0
         */
		const T & front() const {
			if (this->empty())
				throw container_is_empty();

			return *_elem[0];
		}
		/**
         * access the last element.
         * throw container_is_empty if size == 0
         */
		const T & back() const {
			if (this->empty())
				throw container_is_empty();

			return *_elem[_size - 1];
		}
		/**
         * returns an iterator to the beginning.
         */
		iterator begin() {
			return iterator(0, this);
		}
		const_iterator cbegin() const {
			return const_iterator(0, this);
		}
		/**
         * returns an iterator to the end.
         */
		iterator end() {
			return iterator(_size, this);
		}
		const_iterator cend() const {
			return const_iterator(_size, this);
		}
		/**
         * checks whether the container is empty
         */
		bool empty() const {
			return _size == 0;
		}
		/**
         * returns the number of elements
         */
		size_t size() const {
			return _size;
		}
		/**
         * returns the number of elements that can be held in currently allocated storage.
         */
		size_t capacity() const {
			return _capacity;
		}
		/**
         * clears the contents
         */
		void clear() {
			_size = 0;
			_capacity = 0;

			for (size_t i = 0; i < _size; ++i)
				delete _elem[i];
			delete [] _elem;

			_elem = new T *[_capacity];
		}
		/**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
		iterator insert(iterator pos, const T &value) {
			if (_size == _capacity)
				doubleSpace();

			int ind = pos.ptr;
//			std::cout << ind << std::endl;
			for (size_t i = _size; i > ind; --i)
				_elem[i] = _elem[i - 1];

			_elem[ind] = new T(value);
			_size++;

			return iterator(ind, this);

		}
		/**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value is true
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
		iterator insert(const size_t &ind, const T &value) {
			if (ind > _size)
				throw index_out_of_bound();

			if (_size == _capacity)
				doubleSpace();

			for (size_t i = _size; i > ind; --i)
				_elem[i] = _elem[i - 1];

			_elem[ind] = new T(value);
			_size++;

			return iterator(ind, this);
		}
		/**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
		iterator erase(iterator pos) {
            size_t ind = pos.ptr;
            delete _elem[ind];
            _size--;

			for (size_t i = ind; i < _size; ++i)
				_elem[i] = _elem[i + 1];

			//delete _elem[_size];

			return iterator(ind, this);
		}
		/**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
		iterator erase(const size_t &ind) {
			if (ind >= _size)
			    throw index_out_of_bound();

			for (size_t i = ind + 1; i < _size; ++i)
				_elem[i - 1] = _elem[i];

			_size--;

			return iterator(ind, this);
		}
		/**
         * adds an element to the end.
         */
		void push_back(const T &value) {
			if (_size == _capacity)
				doubleSpace();

			_elem[_size] = new T(value);

			_size++;
		}
		/**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
		void pop_back() {
			if (this->empty())
				throw container_is_empty();

			delete _elem[_size - 1];
			--_size;
		}
	};


}

#endif
