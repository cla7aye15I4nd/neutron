#ifndef BTREE_MYVECTOR_HPP
#define BTREE_MYVECTOR_HPP

#include "exceptions.hpp"
#include "constant.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

// 这是一个静态数组的vector
namespace sjtu {
	template<typename T>
	class vector {

	private:
		Rank _size;
		//        T _elem[blockSize / sizeof(T)];
		T _elem[blockSize + 1];

	public:
		vector() {
			_size = 0;
		}

		~vector() {

		}

		vector &operator=(const vector &other) {
			if (this == &other)
				return *this;

			_size = other._size;

			for (Rank i = 0; i < _size; ++i)
				_elem[i] = other._elem[i];

			return *this;
		}

		T & at(const Rank &pos) {
			if (pos >= _size || pos < 0)
				throw index_out_of_bound();

			return _elem[pos];
		}

		const T & at(const Rank &pos) const {
			if (pos >= _size || pos < 0)
				throw index_out_of_bound();

			return _elem[pos];
		}

		T & operator[](const Rank &pos) {
			if (pos >= _size)
				throw index_out_of_bound();
			return _elem[pos];
		}

		const T & operator[](const Rank &pos) const {
			if (pos >= _size)
				throw index_out_of_bound();
			return _elem[pos];
		}

		const T & front() const {
			if (this->empty())
				throw container_is_empty();

			return _elem[0];
		}

		const T & back() const {
			if (this->empty())
				throw container_is_empty();

			return _elem[_size - 1];
		}

		bool empty() const {
			return _size == 0;
		}
		/**
		 * returns the number of elements
		 */
		Rank size() const {
			return _size;
		}

		void insert(const Rank &ind, const T &value) {
			if (ind > _size)
				throw index_out_of_bound();

			for (Rank i = _size; i > ind; --i)
				_elem[i] = _elem[i - 1];

			_elem[ind] = value;
			_size++;

			return;
		}

		void erase(const Rank &ind) {
			if (ind >= _size)
				throw index_out_of_bound();

			for (Rank i = ind + 1; i < _size; ++i)
				_elem[i - 1] = _elem[i];

			_size--;

			return;
		}
		/**
		 * adds an element to the end.
		 */
		void push_back(const T &value) {
			_elem[_size] = value;

			_size++;
		}
		/**
		 * remove the last element from the end.
		 * throw container_is_empty if size() == 0
		 */
		void pop_back() {
			if (this->empty())
				throw container_is_empty();

			--_size;
		}

		void copy(const vector &other, const Rank l, const Rank r) {
			if (l < 0 || r > other._size)
				throw index_out_of_bound();

			for (Rank i = l; i < r; ++i) {
				_elem[i - l] = other._elem[i];
			}

			_size = r - l;

			return;
		}

		void update_size(const Rank &new_size) {
			_size = new_size;
		}

		void clear() {
			_size = 0;
		}

		void file_read(FILE *&fp, const Rank &num) {
			_size = num;
			fread(_elem, sizeof(T), _size, fp);
		}

		void file_write(FILE *&fp) {
			fwrite(_elem, sizeof(T), _size, fp);
		}
	};


}

#endif //BTREE_MYVECTOR_HPP