/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {

    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
    private:
        struct node{
            node *lson,*rson;
            int h;
            value_type *data;
            node(const node &other){
                data=new value_type(*(other.data));
                lson=rson=NULL;
                h=0;
            }
            node(){
                lson=rson=NULL;
                data=NULL;
                h=1;
            }
            ~node(){
                if(data) delete data;
            }
        };
        node *root;
        int sz;
        Compare comp;
        bool equal(const Key &k1,const Key &k2){
            return (!comp(k1,k2))&&(!comp(k2,k1));
        }
        void geth(node *x){
            x->h=std::max(hl(x),hr(x))+1;
        }
        inline int hl(node *x){
            return x->lson==NULL?0:x->lson->h;
        }
        inline int hr(node *x){
            return x->rson==NULL?0:x->rson->h;
        }
        node *rotateL(node *x)
        {
            node *tmp=x->rson;
            x->rson=tmp->lson;
            tmp->lson=x;
            geth(x);
            geth(tmp);
            return tmp;
        }
        node *rotateR(node *x)
        {
            node *tmp=x->lson;
            x->lson=tmp->rson;
            tmp->rson=x;
            geth(x);
            geth(tmp);
            return tmp;
        }
        node *adjust(node *x)
        {
            int xhl=hl(x);
            int xhr=hr(x);
            if(-1<=xhl-xhr&&xhl-xhr<=1) return x;
            node *son;
            if(xhl-xhr==2){//最后一次插入在左子树导致失衡
                son=x->lson;
                if(hl(son)-hr(son)==1)//LL情况，一次右旋
                    return rotateR(x);
                if(hl(son)-hr(son)==-1)//LR情况，先左再右
                {
                    x->lson=rotateL(son);
                    geth(x);
                    return rotateR(x);
                }
            }
            if(xhl-xhr==-2){//最后一次插入右子树
                son=x->rson;
                if(hr(son)-hl(son)==1)//RR情况,一次左旋
                    return rotateL(x);
                if(hr(son)-hl(son)==-1){//RL情况,先左后右
                    x->rson= rotateR(son);
                    geth(x);
                    return rotateL(x);
                }
            }
            return x;
        }
        node *find_suc(node *x){//找后继
            if(x->rson==NULL){
                return x;
            }
            node *tmp=x->rson;
            while(tmp->lson!=NULL) tmp=tmp->lson;
            return tmp;
        }
        node *find_prev(node *x){//找前驱
            if(x->lson==NULL){
                return x;
            }
            node *tmp=x->lson;
            while(tmp->rson!=NULL) tmp=tmp->rson;
            return tmp;
        }
        node *_insert(node *now,const value_type &data){
            if(now==NULL){
                now=new node;
                now->data=new value_type(data);
                return now;
            }
            if(comp(data.first,now->data->first)){
                now->lson= _insert(now->lson,data);
                geth(now);
            }
            else if(comp(now->data->first,data.first)){
                now->rson= _insert(now->rson,data);
                geth(now);
            }
            return adjust(now);
        }
        node *_erase(node *now,const value_type &data)
        {
            if(now==NULL) return now;
            if(comp(now->data->first,data.first)){
                now->rson= _erase(now->rson,data);
            }
            else if(comp(data.first,now->data->first)){
                now->lson= _erase(now->lson,data);
            }
            else{
                if(now->rson==NULL&&now->lson==NULL){//叶节点情况
                    delete now;
                    now=NULL;
                    return now;
                }
                if(now->rson==NULL){//有左子树无右子树，用左子树root代替
                    node *tmp=now->lson;
                    delete now;
                    now=NULL;
                    return tmp;
                }
                if(now->lson==NULL){//有右子树无左子树，用右子树root代替
                    node *tmp=now->rson;
                    delete now;
                    now=NULL;
                    return tmp;
                }
//                node *suc= find_suc(now);//左右子树都有，找到后驱结点将其值复制/更换后驱结点和now的位置，然后删除后驱结点
//                delete now->data;
//                now->data=new value_type(suc->data->first,suc->data->second);
//                now->rson= _erase(now->rson,*(now->data));

                node *suc,*suc_fa;
                suc_fa=now;suc=now->rson;
                while(suc->lson!=NULL){
                    suc_fa=suc;
                    suc=suc->lson;
                }
                node *tmpl=now->lson,*tmpr=now->rson;
                if(suc_fa!=now)
                {
                    now->lson=suc->lson,now->rson=suc->rson;
                    suc->lson=tmpl,suc->rson=tmpr;
                    if(suc_fa->rson==suc) suc_fa->rson=now;
                    else suc_fa->lson=now;
                }
                else{
                    now->lson=suc->lson,now->rson=suc->rson;
                    suc->lson=tmpl;suc->rson=now;
                }
                value_type val(*(now->data));
                int tmph;
                tmph=now->h;
                now->h=suc->h;
                suc->h=tmph;
                now=suc;
                now->rson= _erase(now->rson,val);

            }
            geth(now);
            return adjust(now);
        }
        node *_find(node *now,const Key &key) const{
            if(now==NULL) return NULL;
            if(comp(now->data->first,key)){
                return _find(now->rson,key);
            }
            if(comp(key,now->data->first)){
                return _find(now->lson,key);
            }
            return now;
        }
        node *nex(node *now,const value_type &data) const
        {
            if(now==NULL) return now;
            if(comp(data.first,now->data->first)){
                node *tmp=nex(now->lson,data);
                return tmp==NULL?now:tmp;
            }
            return nex(now->rson,data);
        }
        node *pre(node *now,const value_type &data) const{
            if(now==NULL) return now;
            if(comp(now->data->first,data.first)){
                node *tmp=pre(now->rson,data);
                return tmp==NULL?now:tmp;
            }
            return pre(now->lson,data);
        }
        node *max_element() const{
            if(root==NULL) return NULL;
            node *p=root;
            while(p->rson!=NULL) p=p->rson;
            return p;
        }
        node *min_element() const{
            if(root==NULL) return NULL;
            node *p=root;
            while(p->lson!=NULL) p=p->lson;
            return p;
        }
        node *copy(node *&now,node *copied_tree){
            if(copied_tree==NULL) return NULL;
            now=new node;
            now->data=new value_type(*(copied_tree->data));
            now->h=copied_tree->h;
            now->lson=copy(now->lson,copied_tree->lson);
            now->rson=copy(now->rson,copied_tree->rson);
            return now;
        }
        void node_clear(node *now){
            if(now== NULL) return;
            node_clear(now->lson);
            node_clear(now->rson);
            delete now;
            now=NULL;
        }
    public:
        class const_iterator;
        class iterator;
        friend iterator;
        friend const_iterator;
        class iterator {
            friend const_iterator;
            friend map;
            friend node;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            const map *itmap;
            node *now;
        public:
            iterator() {
                // TODO
                itmap=NULL;
                now=NULL;
            }
            iterator(const iterator &other) {
                // TODO
                itmap=other.itmap;
                now=other.now;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if(now==NULL) throw invalid_iterator();
                iterator tmp(*this);
                now=itmap->nex(itmap->root,*(now->data));
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                if(now==NULL) throw invalid_iterator();
                now=itmap->nex(itmap->root,*(now->data));
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(*this);
                operator--();
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                now=(now== NULL?itmap->max_element():itmap->pre(itmap->root,*(now->data)));
                if(now== NULL) throw invalid_iterator();
                return *this;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {return *(now->data);}
            bool operator==(const iterator &rhs) const {return itmap==rhs.itmap&&now==rhs.now;}
            bool operator==(const const_iterator &rhs) const {return itmap==rhs.itmap&&now==rhs.now;}
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return !(*this==rhs);}
            bool operator!=(const const_iterator &rhs) const {return !(*this==rhs);}

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return now->data;
            }
        };
        class const_iterator :public iterator{
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
        public:
            const_iterator()=default;
            const_iterator(const const_iterator &other):iterator(other) {
                // TODO
            }
            const_iterator(const iterator &other):iterator(other) {
                // TODO
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
        };
        /**
         * TODO two constructors
         */
        map() {
            sz=0;
            root=NULL;
        }
        map(const map &other) {
            if(other.root==NULL){
                sz=0;
                root=NULL;
                return;
            }
            root= copy(root,other.root);
            sz=other.sz;
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other) {
            if(other.root==root){
                return *this;
            }
            clear();
            copy(root,other.root);
            sz=other.sz;
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() {
            clear();
            sz=0;
            root=NULL;
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key) {
            node *tmp= _find(root,key);
            if(tmp==NULL) throw index_out_of_bound();
            return tmp->data->second;
        }
        const T & at(const Key &key) const {
            node *tmp= _find(root,key);
            if(tmp==NULL) throw index_out_of_bound();
            return tmp->data->second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key) {
            node *tmp= _find(root,key);
            if(tmp==NULL){
                root=_insert(root,pair<Key,T>(key,T()));
                sz++;
                return _find(root,key)->data->second;
            }
            return tmp->data->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const {
            node *tmp= _find(root,key);
            if(tmp==NULL) throw index_out_of_bound();
            return tmp->data->second;
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            node *tmp=min_element();
            iterator it;
            it.itmap=this;
            it.now=tmp;
            return it;
        }
        const_iterator cbegin() const {
            node *tmp=min_element();
            const_iterator it;
            it.itmap=this;
            it.now=tmp;
            return it;
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            iterator it;
            it.itmap=this;
            it.now=NULL;
            return it;
        }
        const_iterator cend() const {
            iterator it;
            it.itmap= this;
            it.now=NULL;
            return it;
        }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return sz==0;
        }
        /**
         * returns the number of elements.
         */
        size_t size() const {
            return sz;
        }
        /**
         * clears the contents
         */
        void clear() {
            if(root==NULL) return;
            sz=0;
            node_clear(root);
            root=NULL;
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            node *tmp= _find(root,value.first);
            if(tmp!=NULL){
                iterator it;
                it.itmap=this;
                it.now=tmp;
                return pair<iterator,bool>(it, false);
            }
            root=_insert(root,value);
            sz++;
            tmp= _find(root,value.first);
            iterator it;
            it.itmap=this;
            it.now=tmp;
            return pair<iterator,bool>(it, true);
        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if(pos.now==NULL||pos.itmap!=this) throw invalid_iterator();
            node *tmp= _find(root,pos.now->data->first);
            if(tmp==NULL) throw invalid_iterator();
            root= _erase(root,*(pos.now->data));
            sz--;
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            node *tmp= _find(root,key);
            return tmp==NULL?0:1;
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            node *tmp= _find(root,key);
            iterator it;
            it.itmap=this;
            it.now=tmp;
            return it;
        }
        const_iterator find(const Key &key) const {
            node *tmp= _find(root,key);
            const_iterator it;
            it.itmap=this;
            it.now=tmp;
            return it;
        }
    };

}

#endif