
#ifndef _ZJ_VECTOR_
#define _ZJ_VECTOR_

#include <cstddef>
#include "ZJ_alloc.h"
#include "ZJ_utils.h"
#include "ZJ_iterator.h"
#include <iostream>

namespace ZJ {

    template <typename T>
    //class vector_iterator : public random_access_iterator<T> {
    class vector_iterator : public iterator_base<random_access_iterator_tag, T> {
        private : 
            T* iter;
        
        public : 
            template <typename U>
            friend class vector_iterator;

            vector_iterator() : iter(0) {}

            vector_iterator(T* ptr) : iter(ptr) {}

            template <typename U>
            vector_iterator(const vector_iterator<U>& it) : iter(it.iter) {}

            T& operator*() {return *iter;}

            vector_iterator<T>& operator++ () { //covariant return type :D
                ++iter;
                return *this;
            }

            vector_iterator<T>& operator-- () {
                --iter;
                return *this;
            }

            vector_iterator<T>& operator+= (size_t n) {
                iter += n;
                return *this;
            }

            vector_iterator<T>& operator-= (size_t n) {
                iter -= n;
                return *this;
            }

            T& operator*() const {return *iter;}

            T* operator->() const {return iter;}

            vector_iterator<T>& operator= (const vector_iterator& rhs) {
                iter = rhs.iter;
                return *this;
            }

            vector_iterator<T>& operator=(T* rhs) {
                iter = rhs;
                return *this;
            }

            vector_iterator<T> operator+ (size_t n) const {
                return vector_iterator<T>(iter + n);
            }

            vector_iterator<T> operator- (size_t n) const {
                return vector_iterator<T>(iter - n);
            }

            size_t operator- (const vector_iterator<T>& rhs) const {
                return (size_t)(iter - rhs.iter);
            }

            vector_iterator<T> operator++ (int) {
                vector_iterator<T> res(*this);
                ++iter;
                return res;
            }

            vector_iterator<T> operator-- (int) {
                vector_iterator<T> res(*this);
                --iter;
                return res;
            }

            bool operator== (const vector_iterator<T>& rhs) const {
                return iter == rhs.iter;
            }

            bool operator!= (const vector_iterator<T>& rhs) const {
                return iter != rhs.iter;
            }
    };

    template <typename T, typename Alloc = ZJ::allocator<T>>
    class vector {
        public : 
            typedef T                           value_type;
            typedef T*                          pointer;
            typedef const T*                    const_pointer;
            typedef vector_iterator<T>          iterator;
            typedef vector_iterator<const T>    const_iterator;
            typedef T&                          reference;
            typedef const T&                    const_reference;
            typedef size_t                      size_type;
            typedef ptrdiff_t                   difference_type;
        
        protected : 
            typedef Alloc vector_allocator;
            iterator start;
            iterator finish;
            iterator storage_end;
        
        public : 
            vector() : start(), finish(), storage_end() {}

            vector(size_type n) {alloc_construct((size_type)n, T()); }
 
            vector(size_type n, const T& value) {alloc_construct((size_type)n, value); }

            vector(const_iterator first, const_iterator last) {
                size_type n = last - first;
                start = vector_allocator::allocate(n);
                ZJ_uninitialized_copy(first, last, begin());
                finish = start + n;
                storage_end = start + n;
            }
            
            vector(const vector<T>& v) {
                start = vector_allocator::allocate(v.capacity());
                ZJ_uninitialized_copy(v.begin(), v.end(), begin());
                finish = start + v.size();
                storage_end = start + v.capacity();
            }

            ~vector() {
                ZJ_destroy(start, finish);
                vector_allocator::deallocate(&*start, size());
            }

            iterator begin() {
                return start;
            }

            const_iterator begin() const {
                return const_iterator(start);
            }

            const_iterator cbegin() const {
                return const_iterator(start);
            }

            iterator end() {
                return finish;
            }

            const_iterator end() const {
                return const_iterator(finish);
            }

            const_iterator cend() const {
                return const_iterator(finish);
            }

            reference front() {return *begin();}

            const_reference front() const {return *begin();}

            reference back() {return *(--end());}

            const_reference back() const {return *(--end());}

            size_type size() const {
                return finish - start;
            }

            size_type capacity() const {
                return storage_end - start;
            }

            bool empty() const {
                return finish == start;
            }

            reference operator[] (size_type idx) {
                return *(start + idx);
            }

            void resize(size_type new_size) {
                if(new_size < size()) {  // shrinking
                    ZJ_destroy(start + new_size, finish);
                    finish = start + new_size;
                }
                else { // extending
                    iterator new_start = allocator<value_type>::allocate(new_size);
                    size_type n = size();
                    ZJ_uninitialized_copy(start, finish, new_start);
                    ZJ_destroy(start, start + n);
                    start = new_start;
                    finish = start + n;
                    storage_end = start + new_size;
                }
            }

            void push_back(const value_type& value) {
                if(finish == storage_end) resize(2 * capacity() + 1);
                ZJ_construct(finish, value);
                ++finish;
            }

            void pop_back() {
                --finish;
                ZJ_destroy(finish);
            }

            void insert(iterator pos, const T& value) {
                insert(pos, 1, value);
            }
            
            void insert(iterator pos, size_type n, const T& value) { 
                if (n + size() > capacity()) {
                    iterator new_start = vector_allocator::allocate(2 * (n + size()));
                    iterator it = ZJ_uninitialized_copy(start, pos, new_start);
                    finish = ZJ_uninitialized_copy(pos, finish, it + n);
                    ZJ_uninitialized_fill_n(it, n, value);
                    start = new_start;
                    storage_end = start + 2 * (finish - start);
                }
                else {
                    finish = ZJ_uninitialized_copy(pos, finish, pos + n);
                    ZJ_uninitialized_fill_n(pos, n, value);
                }
            }
            
            // TO BE FIXED: cannot handle iterator of different type
            // cannot handle iterator from current vector - iterator invalidation
            // use a template 
            // tempalte <typename InputIter, typename OutputIter>
            // void insert(OutputIter pos, InputIter first, InputIter last)
            void insert(iterator pos, iterator first, iterator last) { 
                size_type n = last - first;
                if (n + size() > capacity()) {
                    iterator new_start = vector_allocator::allocate(2 * (n + size()));
                    iterator it = ZJ_uninitialized_copy(start, pos, new_start);
                    finish = ZJ_uninitialized_copy(pos, finish, it + n);
                    ZJ_uninitialized_copy(first, last, it);
                    start = new_start;
                    storage_end = start + 2 * (finish - start);
                } 
                else {
                    finish = ZJ_uninitialized_copy(pos, finish, pos + n);
                    ZJ_uninitialized_copy(first, last, pos);
                }
            }

            iterator erase(iterator pos) {
                if(pos + 1 != end())
                    copy(pos + 1, finish, pos);
                --finish;
                destroy(finish);
                return pos;
            }

            iterator erase(iterator first, iterator last) {
                iterator new_finish = ZJ_uninitialized_copy(last, finish, first);
                ZJ_destroy(new_finish, finish);
                finish -= (last - first);
                return first;
            }

            void clear() {erase(start, finish); }
        
        protected : 
            void alloc_construct(size_type n, const T& value) {
                start = vector_allocator::allocate(n);
                finish = start + n;
                storage_end = finish;
                ZJ_construct(start, finish, value);
            }
    };

}

#endif
