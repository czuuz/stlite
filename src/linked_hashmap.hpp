/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */

    template<
            class Key,
            class T,
            class Hash = std::hash<Key>,
            class Equal = std::equal_to<Key>
    > class linked_hashmap {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::linked_hashmap as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        int capacity;
        double Loadfactor;
        int list_size;

        struct node{
            node *nex;
            node *pre;//维护链表
            node *hnex;//维护哈希表
            value_type *data;
            node(){
                hnex=nex=pre= NULL;
                data=NULL;
            }
            ~node()
            {
                nex=pre= NULL;
                if(data)
                {
                    delete data;
                    data=NULL;
                }
            }
        };
        node **map;//哈希表
        node *list_begin;//顺序访问
        node *list_tail;
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = linked_hashmap.begin(); --it;
         *       or it = linked_hashmap.end(); ++end();
         */
        class const_iterator;
        class iterator {
            friend class const_iterator;
            friend class linked_hashmap;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            node *pos;
            linked_hashmap *lhm;
        public:
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
            using difference_type = std::ptrdiff_t;
            using value_type = typename linked_hashmap::value_type;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator_category = std::output_iterator_tag;


            iterator() {
                // TODO
                pos= NULL;
            }
            iterator(const iterator &other) {
                // TODO
                pos=other.pos;
                lhm=other.lhm;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                iterator tmp(*this);
                pos=pos->nex;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                pos=pos->nex;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                if(pos== NULL||pos->pre== NULL) throw invalid_iterator();
                iterator tmp(*this);
                pos=pos->pre;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                if(pos== NULL||pos->pre== NULL) throw invalid_iterator();
                pos=pos->pre;
                return *this;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                return *(pos->data);
            }
            bool operator==(const iterator &rhs) const {
                return lhm==rhs.lhm&&pos==rhs.pos;
            }
            bool operator==(const const_iterator &rhs) const {
                return lhm==rhs.lhm&&pos==rhs.pos;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this==rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this==rhs);
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return pos->data;
            }
        };

        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
            friend class iterator;
            friend class linked_hashmap;
        private:
            // data members.
            const node *pos;
            const linked_hashmap *lhm;
        public:
            const_iterator() {
                // TODO
                pos= NULL;
                lhm= NULL;
            }
            const_iterator(const node *p,const linked_hashmap *l){
                pos=p;
                lhm=l;
            }
            const_iterator(const const_iterator &other) {
                // TODO
                pos=other.pos;
                lhm=other.lhm;
            }
            const_iterator(const iterator &other) {
                // TODO
                pos=other.pos;
                lhm=other.lhm;
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            const_iterator operator++(int) {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                const_iterator tmp(*this);
                pos=pos->nex;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            const_iterator & operator++() {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                pos=pos->nex;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                if(pos== NULL||pos->pre== NULL) throw invalid_iterator();
                const_iterator tmp(*this);
                pos=pos->pre;
                return tmp;
            }
            /**
             * TODO --iter
             */
            const_iterator & operator--() {
                if(pos== NULL||pos->pre== NULL) throw invalid_iterator();
                pos=pos->pre;
                return *this;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {
                if(pos== NULL||pos->nex== NULL) throw invalid_iterator();
                return *(pos->data);
            }
            bool operator==(const iterator &rhs) const {
                return lhm==rhs.lhm&&pos==rhs.pos;
            }
            bool operator==(const const_iterator &rhs) const {
                return lhm==rhs.lhm&&pos==rhs.pos;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this==rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this==rhs);
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return pos->data;
            }
        };

        /**
         * TODO two constructors
         */
        linked_hashmap() {
            capacity=100;
            Loadfactor=0.75;
            list_size=0;
            list_tail=list_begin= NULL;
            map= new node*[100];
            for(int i=0;i<100;i++) map[i]= NULL;
        }
        linked_hashmap(const linked_hashmap &other) {
            capacity=other.capacity;
            Loadfactor=other.Loadfactor;
            list_size=other.list_size;
            list_tail=list_begin= NULL;
            map= NULL;
            if(list_size==0) return;
            map=new node*[capacity];
            for(int i=0;i<capacity;i++) map[i]= NULL;
            node *p=other.list_begin;
            Hash f;
            while(p->nex){
                if(list_begin== NULL){
                    list_begin=new node;
                    list_begin->data=new value_type (*(p->data));
                    list_tail=new node;
                    list_begin->nex=list_tail;
                    list_tail->pre=list_begin;
                }
                else{
                    list_tail->nex=new node;
                    list_tail->data=new value_type (*(p->data));
                    list_tail->nex->pre=list_tail;
                    list_tail=list_tail->nex;
                }
                int idx=f(p->data->first)%capacity;
                if(map[idx]== NULL){
                    map[idx]=list_tail->pre;
                    map[idx]->hnex=NULL;
                }
                else{
                    node *tmp=map[idx];
                    map[idx]=list_tail->pre;
                    map[idx]->hnex=tmp;
                }
                p=p->nex;
            }

        }

        /**
         * TODO assignment operator
         */
        linked_hashmap & operator=(const linked_hashmap &other) {
            if(other.map==map) return *this;
            if(list_begin){
                node *tmp,*p;
                p=list_begin;
                while(p){
                    tmp=p;
                    p=p->nex;
                    delete tmp;
                }
            }
            if(map) delete [] map;
            capacity=other.capacity;
            Loadfactor=other.Loadfactor;
            list_size=other.list_size;
            list_tail=list_begin= NULL;
            map= NULL;
            if(list_size==0) return *this;
            map=new node*[capacity];
            for(int i=0;i<capacity;i++) map[i]= NULL;
            node *p=other.list_begin;
            Hash f;
            while(p->nex){
                if(list_begin== NULL){
                    list_begin=new node;
                    list_begin->data=new value_type (*(p->data));
                    list_tail=new node;
                    list_begin->nex=list_tail;
                    list_tail->pre=list_begin;
                }
                else{
                    list_tail->nex=new node;
                    list_tail->data=new value_type (*(p->data));
                    list_tail->nex->pre=list_tail;
                    list_tail=list_tail->nex;
                }//tail指针指向要插入的元素地址的后一个

                int idx=f(p->data->first)%capacity;

                if(map[idx]== NULL){
                    map[idx]=list_tail->pre;
                    map[idx]->hnex=NULL;
                }
                else{
                    node *tmp=map[idx];
                    map[idx]=list_tail->pre;
                    map[idx]->hnex=tmp;
                }
                p=p->nex;
            }
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~linked_hashmap() {
            if(list_begin){
                node *tmp,*p;
                p=list_begin;
                while(p){
                    tmp=p;
                    p=p->nex;//遍历链表删除
                    delete tmp;
                }
            }
            if(map) delete [] map;
            map= NULL;
            list_begin=list_tail= NULL;
            capacity=0;
            list_size=0;
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key) {
            if(capacity==0||list_size==0) throw index_out_of_bound();
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p!= NULL)
            {
                if(e(p->data->first,key)){
                    return p->data->second;
                }
                p=p->hnex;
            }
            throw index_out_of_bound();
        }
        const T & at(const Key &key) const {
            if(capacity==0||list_size==0) throw index_out_of_bound();
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p!= NULL)
            {
                if(e(p->data->first,key)){
                    return p->data->second;
                }
                p=p->hnex;
            }
            throw index_out_of_bound();
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key) {
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p!= NULL)
            {
                if(e(p->data->first,key)){
                    return p->data->second;//查找到后返回
                }
                p=p->hnex;
            }
            insert(value_type(key,T()));//未查找到插入;
            idx=f(key)%capacity;
            return map[idx]->data->second;
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const {
            if(capacity==0||list_size==0) throw index_out_of_bound();
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p!= NULL)
            {
                if(e(p->data->first,key)){
                    return p->data->second;
                }
                p=p->hnex;
            }
            throw index_out_of_bound();
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            iterator tmp;
            tmp.pos=list_begin;
            tmp.lhm=this;
            return tmp;
        }
        const_iterator cbegin() const {
            return const_iterator(list_begin,this);
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            iterator tmp;
            tmp.pos=list_tail;
            tmp.lhm=this;
            return tmp;
        }
        const_iterator cend() const {
            return const_iterator(list_tail,this);
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return list_size==0;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return list_size;
        }

        /**
         * clears the contents
         */
        void clear() {
            if(list_begin){
                node *tmp,*p;
                p=list_begin;
                while(p){

                    tmp=p;
                    p=p->nex;
                    delete tmp;
                }
            }
            for(int i=0;i<capacity;i++) map[i]= NULL;
            list_tail=list_begin= NULL;
            list_size=0;
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            Hash f;
            Equal e;
            int idx=f(value.first)%capacity;
            node *pp;
            pp=map[idx];
            while(pp!= NULL){
                if(e(pp->data->first,value.first))//插入重复，失败
                {
                    iterator it;
                    it.pos=pp;
                    it.lhm=this;
                    return pair<iterator,bool>(it,0);
                }
                pp=pp->hnex;
            }
            if(capacity*Loadfactor<(list_size+1)){

                if(map) delete [] map;
                map=new node *[2*capacity];
                capacity*=2;
                node *p;


                p=list_begin;

                for(int i=0;i<capacity;i++) map[i]= NULL;
                while(p!=list_tail&&p!= NULL){//重哈希
                    int idx=f(p->data->first)%capacity;
                    if(map[idx]== NULL)
                    {
                        map[idx]=p;
                        map[idx]->hnex=NULL;
                        p=p->nex;
                    }
                    else
                    {
                        node *tmp=map[idx];
                        map[idx]=p;
                        map[idx]->hnex=tmp;
                        p=p->nex;
                    }
                }
            }
            idx=f(value.first)%capacity;
            if(list_begin==NULL){
                list_begin=new node;
                list_begin->data=new value_type (value);
                list_tail=new node;
                list_tail->pre=list_begin;
                list_begin->nex=list_tail;
            }
            else{
                list_tail->data=new value_type (value);
                list_tail->nex=new node;
                list_tail->nex->pre=list_tail;
                list_tail=list_tail->nex;
            }
            if(map[idx]== NULL){
                map[idx]=list_tail->pre;
            }
            else
            {
                node *tmp=map[idx];
                map[idx]=list_tail->pre;
                map[idx]->hnex=tmp;
            }
            list_size++;
            iterator it;
            it.pos=list_tail->pre;
            it.lhm=this;
            return pair<iterator,bool>(it,1);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if(list_size==0)
                throw container_is_empty();
            if(pos.lhm!= this||pos.pos== NULL||pos.pos->nex== NULL) throw invalid_iterator();
            Hash f;
            Equal e;
            int idx=f(pos.pos->data->first)%capacity;
            node *p=map[idx];
            node *ppre;
            if(p== NULL) throw invalid_iterator();
            while(p!=NULL&&e(p->data->first,pos.pos->data->first)==0) {ppre=p;p=p->hnex;}
            if(p== NULL) throw invalid_iterator();
            if(p== map[idx]){
                map[idx]=p->hnex;
            }
            else {
                ppre->hnex= p->hnex;
            }
            if(p==list_begin){
                list_begin=list_begin->nex;
                if(p->nex==list_tail) {
                    delete list_tail;
                    list_tail=list_begin=NULL;
                }
            }
            else {
                if (p->pre) p->pre->nex = p->nex;
                if (p->nex) p->nex->pre = p->pre;
            }

            delete p;
            list_size--;
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         */
        size_t count(const Key &key) const {
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p){
                if(e(p->data->first,key)) return 1;
                p=p->hnex;
            }
            return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            Hash f;
            Equal e;
            if(list_size==0) return this->end();
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p!=NULL) {
                if (e(p->data->first, key)) {
                    iterator it;
                    it.pos=p;
                    it.lhm=this;
                    return it;
                }
                p = p->hnex;
            }
            return this->end();
        }
        const_iterator find(const Key &key) const {
            Hash f;
            Equal e;
            int idx=f(key)%capacity;
            node *p=map[idx];
            while(p) {
                if (e(p->data->first, key)) {
                    const_iterator it;
                    it.pos=p;
                    it.lhm=this;
                    return it;
                }
                p = p->hnex;
            }
            return this->cend();
        }
    };

}

#endif