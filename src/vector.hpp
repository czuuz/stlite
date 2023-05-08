#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include<cmath>
#include<iostream>
#include<cstring>

namespace sjtu {
    /**
     * a data container like std::vector
     * store data in a successive memory and support random access.
     */
    template<typename T>
    class vector {
    private:
        T* data;
        long long max_size;//最大容纳量
        int len;//当前数组元素个数
        std::allocator<T> allo;//内存分配器
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
            friend class vector;
            friend class const_iterator;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            int idx;
            vector<T>* vec;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator(const vector<T>* v, const int _idx)
            {
                vec = v;
                idx = _idx;
            }
            iterator(vector<T>* v, const int _idx)
            {
                vec = v;
                idx = _idx;
            }
            iterator operator+(const int& n) const {
                //TODO
                iterator newiter(vec, idx + n);
                return newiter;
            }
            iterator operator-(const int& n) const {
                //TODO            
                iterator newiter(vec, idx - n);
                return newiter;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator& rhs) const {
                //TODO
                if (vec != rhs.vec) throw invalid_iterator();
                return abs(idx - rhs.idx);
            }
            iterator& operator+=(const int& n) {
                //TODO
                idx += n;
                return *this;
            }
            iterator& operator-=(const int& n) {
                //TODO
                idx -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator originit(vec, idx);
                idx++;
                return originit;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                idx++;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator originit(vec, idx);
                idx--;
                return originit;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                idx--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                return *((vec->data) + idx);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator& rhs) const {
                return vec == rhs.vec && idx == rhs.idx;
            }
            bool operator==(const const_iterator& rhs) const {
                return vec == rhs.vec && idx == rhs.idx;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator& rhs) const {
                return !(*this == rhs);
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend class vector;
            friend class iterator;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            int idx;
            const vector<T>* vec;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            const_iterator(const vector<T>* v, const int _idx)
            {
                vec = v;
                idx = _idx;
            }
            const_iterator operator+(const int& n) const {
                //TODO
                const_iterator newiter(vec, idx + n);
                return newiter;
            }
            const_iterator operator-(const int& n) const {
                //TODO            
                const_iterator newiter(vec, idx - n);
                return newiter;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator& rhs) const {
                //TODO
                if (vec != rhs.vec) throw invalid_iterator();
                return abs(idx - rhs.idx);
            }
            const_iterator& operator+=(const int& n) {
                //TODO
                idx += n;
                return *this;
            }
            const_iterator& operator-=(const int& n) {
                //TODO
                idx -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator originit(vec, idx);
                idx++;
                return originit;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                idx++;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator originit(vec, idx);
                idx--;
                return originit;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                idx--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                return *((vec->data) + idx);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator& rhs) const {
                return vec == rhs.vec && idx == rhs.idx;
            }
            bool operator==(const const_iterator& rhs) const {
                return vec == rhs.vec && idx == rhs.idx;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator& rhs) const {
                return !(*this == rhs);
            }



        };
        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        void resize(int newsize)
        {
            if (max_size == 0)
            {
                data = allo.allocate(newsize);
                max_size = newsize;
                return;
            }
            T* newdata = allo.allocate(newsize);
            for (int i = 0; i < len; i++)
            {
                new(newdata + i)T(data[i]);
            }
            for (int i = 0; i < len; i++) data[i].~T();//析构防止leak
            allo.deallocate(data, len);
            data = newdata;
            max_size = newsize;
        }
        void doublespace()
        {
            if (max_size == 0)
            {
                this->resize(1);
            }
            else this->resize(2 * max_size);
        }
        vector() {
            data = NULL;
            max_size = 0;
            len = 0;
        }
        vector(const vector& other) {
            len = other.len;
            max_size = other.max_size;
            data = allo.allocate(max_size);//兼容无默认构造函数类
            for (int i = 0; i < len; i++)
            {
                new(data + i)T(other.data[i]);
            }
        }
        /**
         * TODO Destructor
         */
        ~vector() {
            for (int i = 0; i < len; i++) data[i].~T();//析构防止leak
            allo.deallocate(data, max_size);
            len = max_size = 0;
            data = NULL;
        }
        /**
         * TODO Assignment operator
         */
        vector& operator=(const vector& other) {
            if (data == other.data) return *this;//自我赋值的情况
            if (other.max_size > max_size)
            {
                this->resize(other.max_size);
            }
            for (int i = 0; i < other.len; i++)
            {
                new(data + i)T(other.data[i]);
            }
            len = other.len;
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T& at(const size_t& pos) {
            if (pos >= len) throw index_out_of_bound();
            return *(data + pos);
        }
        const T& at(const size_t& pos) const {
            if (pos >= len) throw index_out_of_bound();
            return *(data + pos);
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T& operator[](const size_t& pos) {
            if (pos >= len) throw index_out_of_bound();
            return *(data + pos);
        }
        const T& operator[](const size_t& pos) const {
            if (pos >= len) throw index_out_of_bound();
            return *(data + pos);
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T& front() const {
            if (len == 0) throw container_is_empty();
            return *data;
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T& back() const {
            if (len == 0) throw container_is_empty();
            return *(data + len - 1);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(this, 0);
        }
        const_iterator cbegin() const {
            return const_iterator(this, 0);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(this, len);
        }
        const_iterator cend() const {
            return const_iterator(this, len);
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return len == 0;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return size_t(len);
        }
        /**
         * clears the contents
         */
        void clear() {
            for (int i = 0; i < len; i++) data[i].~T();
            len = 0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T& value) {
            if (pos.vec->len + 1 > pos.vec->max_size) pos.vec->doublespace();
            for (int i = pos.vec->len; i > pos.idx; i--)
            {
                new(pos.vec->data + i)T(pos.vec->data[i - 1]);
            }
            new(pos.vec->data + pos.idx)T(value);
            pos.vec->len++;
            return pos;
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t& ind, const T& value) {
            if (ind > len) throw index_out_of_bound();
            if (len + 1 > max_size) this->doublespace();
            for (int i = len; i > ind; i--)
            {
                new(data + i)T(data[i - 1]);
            }
            new(data + ind)T(value);
            len++;
            return iterator(this, ind);
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            if (len == 0) throw container_is_empty();
            int ind = pos.idx;
            for (int i = ind; i < pos.vec->len - 1; i++)
            {
                new(pos.vec->data + i)T(pos.vec->data[i + 1]);
            }
            pos.vec->data[pos.vec->len - 1].~T();//析构最后一个元素防止leak
            pos.vec->len--;
            return pos;
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t& ind) {
            if (ind >= len) throw index_out_of_bound();
            for (int i = ind; i < len - 1; i++)
            {
                new(data + i)T(data[i + 1]);
            }
            data[len - 1].~T();//记得析构！！
            len--;
            return iterator(this, ind);
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T& value) {
            if (len + 1 >= max_size) this->doublespace();
            new(data + len)T(value);
            len++;
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (len == 0) throw container_is_empty();
            else {
                data[len - 1].~T();//注意析构最后一个元素
                len--;
            }
        }
    };


}

#endif