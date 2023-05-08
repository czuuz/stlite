#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
namespace sjtu {
    /**
     * a data container like std::list
     * allocate random memory addresses for data and they are doubly-linked in a list.
     */
    template<typename T>
    class list {
    protected:
        class node {
            friend list;
        public:
            /**
             * add data members and constructors & destructor
             */
            node* nex;
            T* data;
            node* pre;
            std::allocator<T> allo;
            node()
            {
                nex = pre = NULL;
                data = NULL;
            }
            node(const T& _data, node* _nex = NULL, node* _pre = NULL)
            {
                
                data = allo.allocate(1);
                new(data)T(_data);
                nex = _nex;
                pre = _pre;
                if (_pre) _pre->nex = this;
                if (_nex) _nex->pre = this;
            }
            ~node()
            {
                pre = nex = NULL;
                
                if (data)
                {
                    data->~T();
                    allo.deallocate(data, 1);//possible leak reason!!!!!!!
                }
                data = NULL;
            }
            void loaddata(const T& _data)
            {               
                data = allo.allocate(1);
                new(data)T(_data);
            }
        };

    protected:
        /**
         * add data members for linked list as protected members
         */
        node* head, * tail;
        int length;
        /**
         * insert node cur before node pos
         * return the inserted node cur
         */
        node* insert(node* pos, node* cur) {
            if (length == 0)
            {
                length++;
                head = cur;
                tail = new node;
                head->nex = tail;
                tail->pre = head;
                head->pre = NULL;
                return cur;
            }
            node* p = pos->pre;
            cur->pre = p;
            cur->nex = pos;
            if (p)
            {
                p->nex = cur;
            }
            else head = cur;
            pos->pre = cur;
            length++;
            return cur;
        }
        /**
         * remove node pos from list (no need to delete the node)
         * return the removed node pos
         */
        node* erase(node* pos) {
            if (length == 0||pos==NULL) return pos;
            length--;
            if (length == 0)
            {
                if(tail) delete tail;
                head = tail = NULL;
                return pos;
            }
            if (pos == head)
            {
                head = pos->nex;
            }
            if (pos->pre)
            {
                pos->pre->nex = pos->nex;
            }
            if (pos->nex)
            {
                pos->nex->pre = pos->pre;
            }
            return pos;
        }

    public:
        class const_iterator;
        class iterator {
            friend class const_iterator;
            friend class list;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            const list* l;
            node* pos;
        public:
            iterator()
            {
                l = NULL;
                pos = NULL;
            }
            iterator(const list* _l, node* _pos)
            {
                l = _l;
                pos = _pos;
            }
            /**
             * iter++
             */
            iterator operator++(int) {
                if (l == NULL || pos == NULL || pos->nex == NULL) throw invalid_iterator();
                iterator nexit(l, pos);
                pos = pos->nex;
                return nexit;
            }
            /**
             * ++iter
             */
            iterator& operator++() {
                if (l == NULL || pos == NULL || pos->nex == NULL) throw invalid_iterator();
                pos = pos->nex;
                return *this;
            }
            /**
             * iter--
             */
            iterator operator--(int) {
                if (l == NULL || pos == NULL || pos->pre == NULL) throw invalid_iterator();
                iterator preit(l, pos);
                pos = pos->pre;
                return preit;
            }
            /**
             * --iter
             */
            iterator& operator--() {
                if (l==NULL||pos == NULL || pos->pre == NULL) throw invalid_iterator();
                pos = pos->pre;
                return *this;
            }
            /**
             * TODO *it
             * remember to throw if iterator is invalid
             */
            T& operator *() const {
                if (l==NULL||pos == NULL || pos == l->tail) throw invalid_iterator();//判断可能需要补全！！！
                return *(pos->data);
            }
            /**
             * TODO it->field
             * remember to throw if iterator is invalid
             */
            T* operator ->() const noexcept {
                if (l == NULL || pos == NULL || pos == l->tail) throw invalid_iterator();//判断可能需要补全！！！
                return pos->data;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator& rhs) const {
                return l == rhs.l && pos == rhs.pos;
            }
            bool operator==(const const_iterator& rhs) const {
                return l == rhs.l && pos == rhs.pos;
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
         * should be able to construct from an iterator.
         */
        class const_iterator {
            friend class iterator;
            friend class list;
        private:
            const list* l;
            node* pos;
        public:
            const_iterator(const list* _l, node* _pos)
            {
                l = _l;
                pos = _pos;
            }
            const_iterator(const iterator& i)
            {
                l = i.l;
                pos = i.pos;
            }
            const_iterator operator++(int)
            {
                if (pos==NULL||pos->nex == NULL) throw invalid_iterator();
                const_iterator nexit(l, pos);
                pos = pos->nex;
                return nexit;
            }
            const_iterator& operator++()
            {
                if (pos==NULL||pos->nex == NULL) throw invalid_iterator();
                pos = pos->nex;
                return *this;
            }
            const_iterator operator--(int)
            {
                if (pos==NULL||pos->pre == NULL) throw invalid_iterator();
                const_iterator preit(l, pos);
                pos = pos->pre;
                return preit;
            }
            const_iterator& operator--()
            {
                if (pos==NULL||pos->pre == NULL) throw invalid_iterator();
                pos = pos->pre;
                return *this;
            }
            T& operator*() const
            {
                if (l == NULL || pos == NULL || pos == l->tail) throw invalid_iterator();
                return *(pos->data);
            }
            bool operator==(const iterator& rhs) const
            {
                return l == rhs.l && pos == rhs.pos;
            }
            bool operator==(const const_iterator& rhs) const
            {
                return l == rhs.l && pos == rhs.pos;
            }
            bool operator!=(const iterator& rhs) const
            {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator& rhs) const
            {
                return !(*this == rhs);
            }
        };
        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        list() {
            head = tail = NULL;
            length = 0;
        }
        list(const list& other) {
            head = tail = NULL;
            length = other.length;
            node* cur = other.head;
            for (int i = 0; i < length; i++)
            {
                if (!head)
                {
                    node* temp = new node;
                    tail = temp;
                    head = new node(*(cur->data), temp);
                    temp->pre = head;
                    tail = temp;
                    cur = cur->nex;
                }
                else
                {
                    tail->loaddata(*(cur->data));
                    node* temp = new node;
                    temp->pre = tail;
                    tail->nex = temp;
                    tail = temp;
                    cur = cur->nex;
                }
            }
        }
        /**
         * TODO Destructor
         */
        virtual ~list() {
            
            node* cur = head;
            while (cur != tail)
            {
                node* temp = cur;
                cur = cur->nex;
                delete temp;
            }
            if(tail) delete tail;
            head = tail = NULL;
            length = 0;
        }
        /**
         * TODO Assignment operator
         */
        list& operator=(const list& other) {
            if (this == &other) return *this;
            clear();
            length = other.length;
            node* cur = other.head;
            for (int i = 0; i < length; i++)
            {
                if (!head)
                {
                    node* temp = new node;
                    tail = temp;
                    head = new node(*(cur->data), temp);
                    temp->pre = head;
                    tail = temp;
                    cur = cur->nex;
                }
                else
                {
                    tail->loaddata(*(cur->data));
                    node* temp = new node;
                    temp->pre = tail;
                    tail->nex = temp;
                    tail = temp;
                    cur = cur->nex;
                }
            }
            return *this;
        }
        /**
         * access the first / last element
         * throw container_is_empty when the container is empty.
         */
        const T& front() const {
            if (length == 0) throw container_is_empty();
            return *(head->data);
        }
        const T& back() const {
            if (length == 0) throw container_is_empty();
            return *(tail->pre->data);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(this, head);
        }
        const_iterator cbegin() const {
            return const_iterator(this, head);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(this, tail);
        }
        const_iterator cend() const {
            return const_iterator(this, tail);
        }
        /**
         * checks whether the container is empty.
         */
        virtual bool empty() const {
            return length == 0;
        }
        /**
         * returns the number of elements
         */
        virtual size_t size() const {
            return size_t(length);
        }

        /**
         * clears the contents
         */
        virtual void clear() {           
            while (tail != head)
            {
                node* nexone = head->nex;
                delete head;
                head = nexone;
            }
            if(tail) delete tail;
            tail = head = NULL;
            length = 0;
        }
        /**
         * insert value before pos (pos may be the end() iterator)
         * return an iterator pointing to the inserted value
         * throw if the iterator is invalid
         */
        virtual iterator insert(iterator pos, const T& value) {           
            if (pos.l != this || ((!pos.pos)&&length>0) || !pos.l) {
                throw invalid_iterator(); 
            }
            node* ins = new node(value);
            return iterator(this, insert(pos.pos, ins));
        }
        /**
         * remove the element at pos (the end() iterator is invalid)
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid
         */
        virtual iterator erase(iterator pos) {
            if (length == 0) throw container_is_empty();
            if (pos == end()) throw invalid_iterator();
            iterator nexit(this, pos.pos->nex);
            node* erapos = erase(pos.pos);
            delete erapos;
            return nexit;
        }
        /**
         * adds an element to the end
         */
        void push_back(const T& value) {
            node* _back = new node(value);
            insert(tail, _back);
        }
        /**
         * removes the last element
         * throw when the container is empty.
         */
        void pop_back() {
            if (length == 0) throw container_is_empty();
            node* _back = erase(tail->pre);
            delete _back;
        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T& value) {
            node* front = new node(value);
            insert(head, front);
        }
        /**
         * removes the first element.
         * throw when the container is empty.
         */
        void pop_front() {
            if (length == 0) throw container_is_empty();
            node* _front = erase(head);
            delete _front;
        }
        /**
         * sort the values in ascending order with operator< of T
         */
        void sort() {
            if (length == 0) return;
            T* vec;
            std::allocator<T> allo;
            vec = allo.allocate(length);
            iterator i = begin();
            int ii = 0;
            while (ii <= length - 1)
            {
                new(vec + ii)T(*i);
                ii++;
                i++;
            }
            sjtu::sort<T>(vec, vec + length, [](const T& r1, const T& r2) {return r1 < r2; });
            i = begin();
            ii = 0;
            while (ii <= length - 1)
            {
                i.pos->data->~T();
                new(i.pos->data)T(vec[ii]);
                ii++;
                i++;
            }
            for (int k = 0; k < length; k++)
            {
                vec[k].~T();
            }
            allo.deallocate(vec, length);
        }
        /**
         * merge two sorted lists into one (both in ascending order)
         * compare with operator< of T
         * container other becomes empty after the operation
         * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
         * the order of equivalent elements of *this and other does not change.
         * no elements are copied or moved
         */
        void merge(list& other) {
            
            /*iterator i1 = begin(), i2 = other.begin();
            node* temp;
            while (i2 != other.end())
            {
                while (i1 != end() && (*i1 < *i2 || *i1 == *i2))
                {
                    i1++;
                }
                temp = other.erase(i2.pos);
                i2++;
                insert(i1.pos, temp);
            }
            other.length = 0;
            delete other.tail;
            other.head = other.tail = NULL;*/
            node* i1 = head, * i2 = other.head,*temp;
            while (i2!=other.tail)
            {
                while (i1 != tail && (*(i1->data) < *(i2->data) || *(i1->data) == *(i2->data)))
                {
                    i1 = i1->nex;
                }
                temp = i2;
                i2 = i2->nex;
                insert(i1, temp);
            }
            other.length = 0;
            if(other.tail) delete other.tail;
            other.head = other.tail = NULL;

        }
        /**
         * reverse the order of the elements
         * no elements are copied or moved
         */
        void reverse() {
            if (length == 0) return;
            node* p = head, * temp;
            while (p != tail)
            {
                temp = (p->pre);
                (p->pre) = (p->nex);
                (p->nex) = temp;
                p = (p->pre);
            }
            node* originhead = head;
            head = p->pre;
            head->pre = NULL;
            if(tail) delete tail;
            node* newtail = new node;
            originhead->nex = newtail;
            newtail->pre = originhead;
            tail = newtail;
        }
        /**
         * remove all consecutive duplicate elements from the container
         * only the first element in each group of equal elements is left
         * use operator== of T to compare the elements.
         */
        void unique() {
            if (length <= 1) return;
            node* p = head->nex, * tmp;
            while (p != tail)
            {
                if (*(p->data) == *(p->pre->data))
                {
                    tmp = erase(p->pre);
                    delete tmp;
                }
                p = p->nex;
            }
        }
    };

};

#endif //SJTU_LIST_HPP