


#ifndef _ZJ_DEQUE_
#define _ZJ_DEQUE_

#include <cstddef>
#include "ZJ_alloc.h"
#include "ZJ_utils.h"
#include "ZJ_iterator.h"
#include <iostream>


namespace ZJ {

    template <typename T, typename pointer, typename reference, size_t B_SIZE>
    class deque_iterator : public iterator_base<random_access_iterator_tag, T> {
        public : 
            T* cur;
            T* buffer_start;
            T* buffer_finish;
            T** node;
        public : 
            typedef deque_iterator<T, T*, T&, B_SIZE>               iterator;
            typedef deque_iterator<T, pointer, reference, B_SIZE>   self;
            typedef size_t                                          size_type;
            typedef ptrdiff_t                                       difference_type;

            static size_t buffer_size() {return B_SIZE != 0 ? B_SIZE : (sizeof(T) < 512 ? 512 / sizeof(T) : 1);}

            deque_iterator() : cur(0), buffer_start(0), buffer_finish(0), node(0) {}
           
            deque_iterator(T* c, T* bs, T* bf, T** n) : cur(c), buffer_start(bs), buffer_finish(bf), node(n) {}

            deque_iterator(const deque_iterator& it) :
                cur(it.cur),
                buffer_start(it.buffer_start),
                buffer_finish(it.buffer_finish),
                node(it.node)
            {}
            
            reference operator* () {return *cur;}

            pointer operator-> () {return &(operator*());}

            difference_type operator- (const self& rhs) const {
                if (node == rhs.node) {return cur - rhs.cur;}
                else if (node > rhs.node) {
                    size_t bs = buffer_size();
                    return ((node - rhs.node) - 1) * bs + (cur - buffer_start) + (rhs.buffer_finish - rhs.cur);
                }
                //else if(node < rhs.node) {
                    return -(rhs - *this);
                //}
            }
            
            self& operator++ () {
                ++cur;
                if(cur == buffer_finish) {
                    move_node(node + 1);
                    cur = buffer_start;
                }
                return *this;
            }

            self& operator-- () {
                if(cur == buffer_start) { 
                    move_node(node - 1);
                    cur = buffer_finish;
                }
                --cur;
                return *this;
            }

            self operator++ (int) {
                self it = *this;
                ++*this;
                return it;
            }

            self operator-- (int) {
                self it = *this;
                --*this;
                return it;
            }

            self& operator+= (difference_type n) {
                size_t bs = buffer_size();
                difference_type distance = (cur - buffer_start) + n;
                difference_type quotient = distance / bs;
                difference_type remainder = distance % bs;
                move_node(node + quotient);
                cur = buffer_start + remainder;
                return *this;
            }

            self& operator-= (difference_type n) { 
                size_t bs = buffer_size();
                difference_type distance = (cur - buffer_start) - n;
                if(distance >= 0) {
                    cur -= n;  // no node movement, still in the same buffer
                }
                else { // cannot  use / and % with negative numbers :(
                    //difference_type quotient = (distance + 1) / bs - 1;
                    //difference_type remainder = distance % bs;
                    difference_type quotient = -((-distance - 1) / bs) - 1;
                    difference_type remainder = -((-distance - 1) % bs) - 1;
                    move_node(node + quotient);
                    cur = buffer_finish + remainder;
                }
                return *this;
            }

            self operator+ (difference_type n) const {
                self it = *this;
                return it += n;
            }

            self operator- (difference_type n) const {
                self it = *this;
                return it -= n;
            }

            reference operator[] (difference_type n) const {
                return *(*this + n);
            }

            bool operator== (const self& rhs) const {
                return cur == rhs.cur;
            }

            bool operator!= (const self& rhs) const {
                return cur != rhs.cur;
            }

            bool operator< (const self& rhs) const {
                if (node == rhs.node) return cur < rhs.cur;
                return node < rhs.node;
            }
        
        protected : 
            void move_node (T** new_node) {
                node = new_node;
                buffer_start = *new_node;
                buffer_finish = buffer_start + buffer_size();
            }

    };

    template <typename T, size_t B_SIZE = 0, typename Alloc = allocator<T>>
    class deque {
        public : 
            typedef T                                               value_type;
            typedef T*                                              pointer;
            typedef deque_iterator<T, T*, T&, B_SIZE>               iterator;
            typedef deque_iterator<T, const T*, const T&, B_SIZE>   const_iterator;
            typedef T&                                              reference;
            typedef size_t                                          size_type;
            typedef ptrdiff_t                                       difference_type;

        protected : 
            typedef Alloc               deque_allocator;
            typedef pointer*            map_pointer;
            typedef Alloc               data_allocator;
            typedef allocator<pointer>  map_allocator;
            
            static size_t buffer_size() {return B_SIZE != 0 ? B_SIZE : (sizeof(T) < 512 ? 512 / sizeof(T) : 1);}

            iterator start;
            iterator finish;
            map_pointer map;
            size_type map_size;
            

        public : 
            deque() {fill_initialize();}

            deque(size_type n) {fill_initialize(n, value_type());}

            deque(size_type n, const value_type& value) {fill_initialize(n, value);}

            iterator begin() {return start;}

            const_iterator begin() const {return const_iterator(start);}

            const_iterator cbegin() const {return const_iterator(start);}

            iterator end() {return finish;}

            const_iterator end() const {return const_iterator(finish);}

            const_iterator cend() const {return const_iterator(finish);}

            reference front() {return *start;}

            reference back() {return *(finish - 1);}

            reference operator[] (size_type n) {return start[difference_type(n)];}

            size_type size() const {return finish - start;}

            bool empty() const {return start == finish;}

            void push_back(const value_type& value) {
                if(finish.cur == finish.buffer_finish - 1) {
                    if(finish.node == map + map_size - 1) map_expand(1, false);
                    *(finish.node + 1) = data_allocator::allocate(buffer_size());
                }
                ZJ_construct(finish, value);
                ++finish;
            }

            void push_front(const value_type& value) { 
                if(start.cur == start.buffer_start) {
                    if(start.node == map) map_expand(1, true);
                    *(start.node - 1) = data_allocator::allocate(buffer_size());
                }
                --start;
                ZJ_construct(start, value);  
            }

            void pop_back() {
                if(finish.cur == finish.buffer_start) {
                    map_pointer tmp = finish.node;
                    --finish;
                    data_allocator::deallocate(*tmp, sizeof(*tmp));
                    *tmp = nullptr;
                    ZJ_destroy(finish);
                }
                else {
                    --finish;
                    ZJ_destroy(finish);
                }
            }

            void pop_front() {
                if(start.cur == start.buffer_finish - 1) {
                    map_pointer tmp = start.node;
                    ++start;
                    ZJ_destroy(start - 1);
                    data_allocator::deallocate(*tmp, sizeof(*tmp));
                    *tmp = nullptr;
                }
                else {
                    ++start;
                    ZJ_destroy(start - 1);
                }
            }

            iterator insert(iterator pos, const value_type& value) {
                difference_type index = pos - start;
                if(index < size() / 2) {
                    push_front(front());
                    move_data(start + 1, start + 1 + index, start);
                    ZJ_construct(start + index, value);
                    return start + index;
                }
                else {
                    index = finish - pos;
                    push_back(back());
                    move_data(finish - 1 - index, finish - 1, finish - index);
                    ZJ_construct(finish - 1 - index, value);
                    return finish - 1 - index;
                }
            }

            iterator insert(iterator pos, size_type n, const value_type& value) {
                difference_type index = pos - start;
                size_t bs = buffer_size();
                if(index < size() / 2) {
                    size_type remaining = left_space();
                    if(remaining <= n) 
                        map_expand((n - remaining) / bs + 1, true);
                    alloc_buffer_left((n - (start.cur - start.buffer_start)) / bs + 1); 
                    move_data(start, start + index, start - n);
                    ZJ_uninitialized_fill_n(start + index - n, n, value);
                    start -= n;
                    return start + index + n;
                } 
                else { index = finish - pos;
                    size_type remaining = right_space();
                    if(remaining <= n) 
                        map_expand((n - remaining) / bs + 1, false);
                    alloc_buffer_right((n - (finish.buffer_finish - finish.cur) - 1) / bs + 1);
                    move_data(finish - index, finish, finish - index + n);
                    ZJ_uninitialized_fill_n(finish - index, n, value);
                    finish += n;
                    return finish - index;
                }
            }

            // to be implemented: allow general iterator input
            //iterator insert(iterator first, iterator last, iterator dest) {}

            iterator erase(iterator pos) { // does not shrink 
                difference_type index = pos - start;
                if(index < size() / 2) {
                    move_data(start, pos, start + 1);
                    ++start;
                    ZJ_destroy(start - 1);
                }
                else {
                    move_data(pos + 1, finish, pos);
                    --finish;
                    ZJ_destroy(finish);
                }
            }

            iterator erase(iterator first, iterator last) {
                difference_type left_part = first - start;
                difference_type right_part = finish - last;
                difference_type n = last - first;
                iterator res;
                if(left_part < right_part) {
                    res = ZJ_copy(start, first, start + n, false);
                    res += n;
                    start += n;
                }
                else {
                    res = ZJ_copy(last, finish, last - n, true);
                    finish -= n;
                }
                return res;
            }

            void clear() {
                erase(start, finish);
                size_type n = size();
                while(n--) pop_back();
            }
        
        protected : 
            void fill_initialize() {
                // default map size 5
                size_t bs = buffer_size();
                map_size = 5;
                map = create_map(map_size);
                pointer res = data_allocator::allocate(bs);
                for(int i=0; i<5; i++) 
                    map[i] = nullptr; // init
                map[2] = res;
                start = iterator(res, res, res + bs, map + 2);
                finish = iterator(res, res, res + bs, map + 2);
            }

            void fill_initialize(size_type n, const value_type& value) {
                size_t bs = buffer_size();
                difference_type map_start = 2;
                map_size = n / bs + 1 + 2 * map_start;
                map = create_map(map_size);
                for(int i=0; i<map_size; i++) 
                    map[i] = nullptr; // init
                for(int i=0; i<=n/bs; i++){
                    map[map_start + i] = data_allocator::allocate(bs);
                }
                start = iterator(map[map_start], map[map_start], map[map_start] + bs, map + map_start);
                finish = iterator(map[map_start+n/bs] + n % bs, map[map_start+n/bs], map[map_start+n/bs] + bs, map + map_start+n/bs);
                ZJ_uninitialized_fill_n(start, n, value);
            }

            map_pointer create_map(size_type n_nodes) {
                return map_allocator::allocate(n_nodes);
            }

            void map_expand(size_type n_nodes = 1, bool insert_to_left = true) {
                size_type new_map_size = (finish.node - start.node) + n_nodes + 1;
                difference_type map_start;
                map_pointer new_map;
                if(new_map_size * 2 < map_size) {
                    map_start = (map_size - new_map_size) / 2;
                    new_map = map;
                    ZJ_copy(start.node, finish.node + 1, map + map_start, (start.node - (map + map_start)) > 0);
                }
                else {
                    map_start = new_map_size / 2; // make space for data to be inserted
                    if(insert_to_left) map_start += n_nodes;
                    new_map = map_allocator::allocate(2 * new_map_size);
                    for(int i=0; i<2*new_map_size; i++) 
                        new_map[i] = nullptr;
                    ZJ_copy(start.node, finish.node + 1, new_map + map_start);
                    __ZJ_destroy(map, map + map_size, TRUE_TAG());
                    map_allocator::deallocate(map, map_size);
                    map = new_map;
                    map_size = new_map_size * 2;
                }
                finish.node = new_map + map_start + (finish.node - start.node);
                start.node = new_map + map_start;
            }
        
        public : 
            void move_data(iterator first, iterator last, iterator dest) {
                difference_type n = dest - first;
                if(n > 0) ZJ_copy(first, last, dest, false);
                else ZJ_copy(first, last, dest, true);
            }

            size_type left_space() const {
                size_t bs = buffer_size();
                return (start.node - map) * bs + (start.cur - start.buffer_start);
            }

            size_type right_space() const {
                size_t bs = buffer_size();
                return (map_size - 1 - (finish.node - map)) * bs + (finish.buffer_finish - finish.cur);
            }

            void alloc_buffer_left(size_t n) {
                size_t bs = buffer_size();
                map_pointer mptr = start.node - 1;
                for(; n > 0; --n){
                    *mptr = data_allocator::allocate(bs);
                    --mptr;
                }
            }

            void alloc_buffer_right(size_t n) { 
                size_t bs = buffer_size();
                map_pointer mptr = finish.node + 1;
                for(; n > 0; n--){
                    *mptr = data_allocator::allocate(bs);
                    ++mptr;
                }
            }
            
            size_type ms() const {return map_size;}

            void debug(const char* str = "") {
                size_t bs = buffer_size();
                cout << str << endl;
                cout << "size: " << size() << endl;
                cout << "map size: " << ms() << endl;
                cout << "left remaining: " << left_space() << endl;
                cout << "right remaining: " << right_space() << endl;
                cout << "start: " << start.node - map << " - " << start.cur - start.buffer_start << endl;
                cout << "finish: "  << finish.node - map << " - " << finish.cur -finish.buffer_start << endl;
                for(int i=0; i<ms(); i++){
                    cout << "ms[] " << i << " = " << map[i];
                    if(map[i] != nullptr) {
                        cout << " : ";
                        for(int j=0; j<bs; j++)
                            cout << map[i][j] << " ";
                    }
                    cout << endl;
                }
            }

    };

}



#endif


