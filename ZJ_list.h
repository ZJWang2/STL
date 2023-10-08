

#ifndef _ZJ_LIST_
#define _ZJ_LIST_

#include <cstddef>
#include "ZJ_alloc.h"
#include "ZJ_utils.h"
#include "ZJ_iterator.h"
#include <iostream>

namespace ZJ {
    
    template <typename T>
    struct list_node {
        public : 
            //typedef list_node<T>* node_pointer;
            typedef list_node* node_pointer;

            node_pointer prev;
            node_pointer next;
            T data;

            list_node() : data(0) {
                prev = this;
                next = this;
            }

            list_node(const T& d) : data(d) {}

            template <typename U>
            list_node(const list_node<U>& node) : 
                prev(node.prev), 
                next(node.next), 
                data(node.data) 
            {}
    };

    template <typename T, typename Alloc>
    class list;

    template <typename T, typename pointer, typename reference>
    class list_iterator : public iterator_base<bidirectional_iterator_tag, T> {
        protected : 
            list_node<T>* iter;
        public : 
            typedef list_iterator<T, T*, T&>                iterator;
            typedef list_iterator<T, pointer, reference>    self;
            // such design allows conversion from iter to const_iter
        
        //template <typename , typename >
        //friend class list;

        public : 
            list_iterator() : iter(0) {}

            list_iterator(list_node<T>* ptr) : iter(ptr) {}

            list_iterator(const list_iterator& it)  : iter(it.iter) {}

            list_node<T>* get_raw_pointer() const {return iter;}

            self& operator++ () {
                iter = (*iter).next;
                return *this;
            }

            self& operator-- () {
                iter = (*iter).prev;
                return *this;
            }

            reference operator* () const {return (*iter).data;}

            //pointer operator-> () const {return &((*iter).data);}
            list_node<T>* operator-> () const {return iter;}

            self& operator= (const list_iterator& rhs) {
                iter = rhs.iter;
                return *this;
            }

            self& operator= (const list_node<T>* rhs) {
                iter = rhs;
                return *this;
            }

            self operator++ (int) {
                self res(*this);
                ++(*this);
                return res;
            }

            self operator-- (int) {
                self res(*this);
                --(*this);
                return res;
            }

            bool operator== (const self& rhs) const {
                return iter == rhs.iter;
            }

            bool operator!= (const self& rhs) const {
                return iter != rhs.iter;
            }
    };

    template <typename T, typename Alloc = ZJ::allocator<list_node<T>>>
    class list {
        public : 
            typedef T                                       value_type;
            typedef T*                                      pointer;
            typedef list_node<T>*                           node_pointer;
            typedef list_iterator<T, T*, T&>                iterator;
            typedef list_iterator<T, const T*, const T&>    const_iterator;
            typedef T&                                      reference;
            typedef size_t                                  size_type;
            typedef ptrdiff_t                               difference_type;

        protected : 
            typedef Alloc list_allocator;
            node_pointer node;
        
        //friend class list_iterator<T, T*, T&>;
        //friend class list_iterator<T, const T*, const T&>;
        
        public : 
            list() {
                node = new list_node<T>();
            }

            iterator begin() {
                return iterator((*node).next);
            }

            const_iterator begin() const {
                return const_iterator((*node).next);
            }

            const_iterator cbegin() const {
                return const_iterator((*node).next);
            }

            iterator end() {
                return iterator(node);
            }

            const_iterator end() const {
                return const_iterator(node);
            }

            const_iterator cend() const {
                return const_iterator(node);
            }

            bool empty() const {
                return begin() == end();
            }

            size_type size() const {
                size_type n = 0;
                const_iterator it = cbegin();
                while(it != cend()) {
                    ++it; ++n;
                }
                return n;
            }

            reference front() {return *begin();}

            reference back() {return *(--end());}

            iterator insert(iterator pos, const T& value) {
                node_pointer tmp = pos->prev;
                node_pointer new_node = create_node(value);
                tmp->next = new_node;
                new_node->next = pos.get_raw_pointer();
                pos->prev = new_node;
                new_node->prev = tmp;
                return new_node;
            }

            iterator erase(iterator pos) {
                if (pos == end()) return pos;
                iterator res(pos); ++res;
                node_pointer before = pos->prev;
                node_pointer after  = pos->next;
                before->next = after;
                after->prev = before;
                destroy_node(pos);
                return res;
            }

            void push_back(const T& value) {
                insert(end(), value);
            }

            void push_front(const T& value) {
                insert(begin(), value);
            }

            iterator pop_back() {
                return erase(--end());
            }

            iterator pop_front() {
                return erase(begin());
            }

            void clear() {
                while(!empty()) 
                    erase(begin());
            }

            void remove(const T& value) {
                for(iterator it = begin(); it != end();){
                    if(*it == value) it = erase(it);
                    else ++it;
                }
            }
/*  stl implementation
            void _unique() { 
                iterator first = begin(), last = end();
                iterator next = first;
                while(++next != last) {
                    if(*next == *first) erase(next);
                    else first = next;
                    next = first;
                }
            }
*/
            void unique() { 
                iterator first = begin(), last = end();
                iterator next = ++begin();
                while(next != last) {
                    if(*next == *first) next = erase(next);
                    else ++first, ++next;
                }
            }

            void splice(iterator pos, list& lst) {
                if(lst.empty()) return ;
                transfer(pos, lst.begin(), lst.end());
            }

            void splice(iterator pos, iterator i) {
                iterator j = i;
                ++j;
                if(pos == i || pos == j) return ;
                transfer(pos, i, j);
            }

            void splice(iterator pos, iterator first, iterator last) {
                // pos should be in [first, last)
                if(first == last) return ;
                transfer(pos, first, last);
            }

            void merge(list<T, Alloc>& lst) {
                iterator it1 = begin(), it2 = lst.begin();
                while(it1 != end() && it2 != lst.end()) {
                    if(*it1 <= *it2) ++it1;
                    else {
                        iterator tmp = it2; 
                        ++tmp;
                        transfer(it1, it2, tmp);
                        it2 = tmp;
                    }
                }
                if(it2 != lst.end()) transfer(it1, it2, lst.end());
            }

            void reverse() {
                if(begin() == end() || begin()->next == end().get_raw_pointer()) return ;
                iterator it = ++begin();
                while(it != end()) {
                    iterator tmp = it; 
                    ++tmp;
                    transfer(begin(), it, tmp);
                    it = tmp;
                }
            }

            void sort() {

            }

        protected : 
            node_pointer create_node(const T& value) {
                node_pointer ptr =  list_allocator::allocate(1);
                ZJ_construct(&(ptr->data), value);
                return ptr;
            }

            void destroy_node(iterator it) {
                ZJ_destroy(it);
                Alloc::deallocate(it.get_raw_pointer(), sizeof(it.get_raw_pointer()));
            }
            public : 
            void transfer(iterator pos, iterator first, iterator last) {
                iterator first_prev(first->prev);
                iterator last_prev(last->prev);
                iterator pos_prev(pos->prev);

                last_prev->next = pos.get_raw_pointer();
                pos->prev = last_prev.get_raw_pointer();
                
                pos_prev->next = first.get_raw_pointer();
                first->prev = pos_prev.get_raw_pointer();

                first_prev->next = last.get_raw_pointer();
                last->prev = first_prev.get_raw_pointer();
            }

    };

}

#endif
