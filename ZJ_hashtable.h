


#ifndef _ZJ_HASHTABLE_
#define _ZJ_HASHTABLE_

#include "ZJ_alloc.h"
#include "ZJ_vector.h"
#include "ZJ_pair.h"

namespace ZJ {

    // hash functions
    /**
     * Customized hash function has to support the following operation:
     * () operator: return the hash value of the key 
    */
    template <class Key>
    struct hash {};

    template <>
    struct hash<int> {
        size_t operator() (int x) const {return x;}
    };

    template <>
    struct hash<char> {
        size_t operator() (int x) const {return x;}
    };

    /*....*/

    static const int NUM_PRIMES = 28;
    static const unsigned long PRIME_LIST[NUM_PRIMES] = {
        53,        97,         193,        389,        769,
        1543,      3079,       6151,       12289,      24593,
        49157,     98317,      196613,     393241,     786433,
        1572869,   3145739,    6291469,    12582917,   25165843,
        50331653,  100663319,  201326611,  402653189,  805306457,
        1610612741,3221225473ul,4294967291ul
    };
    // binary seach to find next prime
    inline unsigned long next_prime(unsigned long n) {
        int left = 0, right = NUM_PRIMES - 1;
        while(left <= right) {
            int mid = (left + right) / 2;
            if(PRIME_LIST[mid] < n) left = mid + 1;
            else if(PRIME_LIST[mid] > n) right = mid - 1;
            else return PRIME_LIST[mid];
        }
        return PRIME_LIST[left];
    }

    template <class Value>
    struct hashtable_node {
        hashtable_node* next;
        Value data;
    };

    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals, class Alloc = allocator<hashtable_node<Value>>>
    class hashtable;
    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals>
    class hashtable_iterator;
    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals>
    class hashtable_const_iterator;

    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals>
    class hashtable_iterator {

        friend class hashtable_const_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>;
        public : 
            typedef hashtable_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals> self;
            typedef hashtable_node<Value>       node;
            typedef Value                       value_type;
            typedef Value&                      reference;
            typedef Value*                      pointer;
            typedef ptrdiff_t                   difference_type;
            typedef size_t                      size_type;

            //typedef node*                       node_pointer;

        private :
            node* cur;
            hashtable<Key, Value, HashFunc, KeyOfValue, KeyEquals>* ht;
        
        public : 
            hashtable_iterator() {}

            hashtable_iterator(node* c, hashtable<Key, Value, HashFunc, KeyOfValue, KeyEquals>* h) : cur(c), ht(h) {}

            hashtable_iterator(const self& h_it) : cur(h_it.cur), ht(h_it.ht) {}

            reference operator*() {return cur->data;}

            pointer operator->() {return &(operator*());}

            self operator++() {
                size_type k = ht->bucket_num(cur->data);
                cur = cur->next;
                if(!cur) {
                    size_type n = ht->bucket_size();
                    while(!cur && ++k < n) {
                        cur = ht->buckets[k];
                        //cout << k << " ";
                    }// cout << endl;
                } //else cout << "down" << endl;
                return *this;
            }

            self operator++(int) {
                self res = *this;
                ++(*this);
                return res;
            }

            bool operator== (const self& rhs) const {
                return cur == rhs.cur;
            }

            bool operator!= (const self& rhs) const {
                return cur != rhs.cur;
            }
    };

    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals>
    class hashtable_const_iterator {

        public : 
            typedef hashtable_const_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals> self;
            typedef hashtable_node<Value>       node;
            typedef const Value                 value_type;
            typedef const Value&                reference;
            typedef const Value*                pointer;
            typedef ptrdiff_t                   difference_type;
            typedef size_t                      size_type;

            //typedef node*                       node_pointer;

        private :
            const node* cur;
            const hashtable<Key, Value, HashFunc, KeyOfValue, KeyEquals>* ht;

            //vector<node_pointer>* buckets;
        
        public : 
            hashtable_const_iterator() {}

            hashtable_const_iterator(const node* c, const hashtable<Key, Value, HashFunc, KeyOfValue, KeyEquals>* h) : cur(c), ht(h) {}

            hashtable_const_iterator(const hashtable_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>& h_it) : cur(h_it.cur), ht(h_it.ht) {}
            
            hashtable_const_iterator(const self& h_it) : cur(h_it.cur), ht(h_it.ht) {}

            reference operator*() {return cur->data;}

            pointer operator->() {return &(operator*());}

            self operator++() {
                size_type k = ht->bucket_num(cur->data);
                cur = cur->next;
                if(!cur) {
                    size_type n = ht->bucket_size();
                    while(!cur && ++k < n) {
                        cur = *(ht->buckets.begin() + k);
                    }
                }
                return *this;
            }

            self operator++(int) {
                self res = *this;
                ++(*this);
                return res;
            }

            bool operator== (const self& rhs) const {
                return cur == rhs.cur;
            }

            bool operator!= (const self& rhs) const {
                return cur != rhs.cur;
            }
    };

    template <class Key, class Value, class HashFunc, class KeyOfValue, class KeyEquals, class Alloc>
    class hashtable {
        public : 
            typedef HashFunc        hasher;
            typedef KeyEquals       key_equal;
            typedef size_t          size_type;
            typedef ptrdiff_t       difference_type;
            typedef Value           value_type;
            typedef Key             key_type;
            typedef Value*          pointer;
            typedef const Value*    const_pointer;
            typedef Value&          reference;
            typedef const Value&    const_reference;

            typedef hashtable_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>         iterator;
            typedef hashtable_const_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>   const_iterator;
        
        friend class hashtable_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>;
        friend class hashtable_const_iterator<Key, Value, HashFunc, KeyOfValue, KeyEquals>;
        
        private : 
            hasher hash;
            key_equal equals;
            KeyOfValue get_key;

            typedef hashtable_node<Value> node;
            typedef Alloc node_allocator;

            vector<node*> buckets;
            size_type num_elements;
        
        public : 
            hashtable() : hash(hasher()), equals(key_equal()), get_key(KeyOfValue()), num_elements(0) {
                initialize_buckets(PRIME_LIST[0]);
            }

            hashtable(size_type n, const HashFunc& hf, const KeyEquals& ke) : hash(hf), equals(ke), get_key(KeyOfValue()), num_elements(0) {
                initialize_buckets(n);
            }

            hashtable(const hashtable& rhs) : hash(rhs.hash), equals(rhs.equals), get_key(rhs.get_key), num_elements(rhs.num_elements) {
                buckets.reserve(rhs.buckets.capacity());
                for(const_iterator it = rhs.begin(); it != rhs.end(); ++it) 
                    insert_equal(*it);
            }

            hasher hash_function() const {return hash;}

            key_equal key_eq() const {return equals;}

            iterator begin() {
                for(size_type i = 0; i < bucket_size(); ++i)
                    if(*(buckets.begin() + i)) return iterator(*(buckets.begin() + i), this);
                return iterator(0, this);
            }

            const_iterator begin() const {
                for(size_type i = 0; i < bucket_size(); ++i)
                    if(*(buckets.begin() + i)) return const_iterator(*(buckets.begin() + i), this);
                return const_iterator(0, this);
            }

            const_iterator cbegin() const {
                return begin();
            }

            iterator end() {
                return iterator(0, this);
            }

            const_iterator end() const {
                return const_iterator(0, this);
            }

            const_iterator cend() const {
                return end();
            }

            size_type size() const {
                return num_elements;
            }

            bool empty() const {
                return size() == 0;
            }

            size_type bucket_size() const {
                return buckets.size();
            }

            pair<iterator, bool> insert_unique(const value_type& x) {
                resize(num_elements + 1);
                
                Key kox = get_key(x);
                size_type n = bucket_num_key(kox);
                node* first = buckets[n];
                for(node* cur = first; cur; cur = cur->next) {
                    if(equals(get_key(cur->data), kox)) {
                        return pair<iterator, bool>(iterator(cur, this), false);
                    }
                }
                node* tmp = create_node(x);
                tmp->next = first;
                buckets[n] = tmp;
                ++num_elements;
                return pair<iterator, bool>(iterator(tmp, this), true);
            }

            void insert_unique(iterator first, iterator last) {
                for(; first != last; ++first) {
                    insert_unique(*first);
                }
            }

            iterator insert_equal(const value_type& x) {
                resize(num_elements + 1);
                
                Key kox = get_key(x);
                size_type n = bucket_num_key(kox);
                node* first = buckets[n];
                for(node* cur = first; cur; cur = cur->next) {
                    // instead of inserting right here
                    // which is also a valid way (and easier)
                    // we find the first element that is equal to x
                    // to maintain the stable ordering
                    // plus this can make other functions easier (erase, equal_range, etc.)
                    if(equals(get_key(cur->data), kox)) { 
                        node* tmp = create_node(x);
                        tmp->next = cur->next;
                        cur->next = tmp;
                        ++num_elements;
                        return iterator(tmp, this);
                    }
                }
                node* tmp = create_node(x);
                tmp->next = first;
                buckets[n] = tmp;
                ++num_elements;
                return iterator(tmp, this);
            }

            void insert_equal(iterator first, iterator last) {
                for(; first != last; ++first) {
                    insert_equal(*first);
                }
            }

            void erase(iterator it) {
                node* ptr = it.cur;
                if(ptr) {
                    size_type n = bucket_num(ptr->data);
                    node* first = buckets[n];
                    if(first == ptr) {
                        buckets[n] = first->next;
                        delete_node(first);
                        --num_elements;
                    } else {
                        node* cur = first;
                        node* next = cur->next;
                        while(next) {
                            if(next == ptr) {
                                cur->next = next->next;
                                delete_node(next);
                                --num_elements;
                                break;
                            } else {
                                cur = next;
                                next = cur->next;
                            }
                        }
                    }
                }
            }

            void erase(iterator first, iterator last) {
                while(first != last) erase(first++);
            }

            void erase(const key_type& k) {
                size_type n = bucket_num_key(k);
                node* first = buckets[n];
                if(first) {
                    node* cur = first;
                    node* next = cur->next;
                    while(next) {
                        if(equals(get_key(next->data), k)) {
                            cur->next = next->next;
                            delete_node(next);
                            next = cur->next;
                            --num_elements;
                        } else {
                            cur = next;
                            next = cur->next;
                        }
                    }
                    if(equals(get_key(first->data), k)) {
                        buckets[n] = first->next;
                        delete_node(first);
                        --num_elements;
                    }
                }
            }

            pair<iterator, iterator> equal_range(const key_type& k) {
                size_type n = bucket_num_key(k);
                for(node* cur = *(buckets.begin() + n); cur; cur = cur->next) 
                    if(equals(get_key(cur->data), k)){
                        for(node* next = cur->next; next; next = next->next) 
                            if(!equals(get_key(next->data), k))
                                return pair<iterator, iterator>(iterator(cur, this), iterator(next, this));
                        for(size_type i = n + 1; i < bucket_size(); ++i)
                            if(*(buckets.begin() + i))
                                return pair<iterator, iterator>(iterator(cur, this), iterator(*(buckets.begin() + i), this));
                        return pair<iterator, iterator>(iterator(cur, this), end());
                    }
                return pair<iterator, iterator>(end(), end());
            }

            pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
                size_type n = bucket_num_key(k);
                for(node* cur = *(buckets.begin() + n); cur; cur = cur->next) 
                    if(equals(get_key(cur->data), k)){
                        for(node* next = cur->next; next; next = next->next) 
                            if(!equals(get_key(next->data), k))
                                return pair<const_iterator, const_iterator>(const_iterator(cur, this), const_iterator(next, this));
                        for(size_type i = n + 1; i < bucket_size(); ++i)
                            if(*(buckets.begin() + i))
                                return pair<const_iterator, const_iterator>(const_iterator(cur, this), const_iterator(*(buckets.begin() + i), this));
                        return pair<const_iterator, const_iterator>(const_iterator(cur, this), end());
                    }
                return pair<const_iterator, const_iterator>(end(), end());
            }

            iterator find(const key_type& k) {
                size_type n = bucket_num_key(k);
                for(node* cur = *(buckets.begin() + n); cur; cur = cur->next) 
                    if(equals(get_key(cur->data), k))
                        return iterator(cur, this);
                return end();
            }

            const_iterator find(const key_type& k) const {
                size_type n = bucket_num_key(k);
                for(node* cur = *(buckets.begin() + n); cur; cur = cur->next) 
                    if(equals(get_key(cur->data), k))
                        return const_iterator(cur, this);
                return end();
            }

            size_type count(const key_type& k) const {
                size_type n = bucket_num_key(k);
                size_type res = 0;
                for(node* cur = *(buckets.begin() + n); cur; cur = cur->next)
                    if(equals(get_key(cur->data), k))
                        ++res;
                return res;
            }

            void clear() {
                for(size_type i = 0; i < bucket_size(); ++i) {
                    node* cur = buckets[i];
                    while(cur) {
                        node* next = cur->next;
                        delete_node(cur);
                        cur = next;
                    }
                    buckets[i] = 0;
                }
                num_elements = 0;
            }

            void swap(hashtable<Key, Value, HashFunc, KeyOfValue, KeyEquals>& rhs) {
                buckets.swap(rhs.buckets);
                ZJ_swap(num_elements, rhs.num_elements);
            }

            void print() {
                cout << "bucket_size: " << bucket_size() << endl; 
                cout << "size: " << size() << endl;
                for(size_type i = 0; i < bucket_size(); ++i) {
                    cout << i << ":(";
                    for(node* cur = buckets[i]; cur; cur = cur->next) {
                        //cout << cur << ":" << cur->data << " ";
                        cout << cur->data << " ";
                    }
                    cout << ") ";
                } cout << endl;
            }

        protected : 
            node* create_node(const value_type& x) {
                node* res = node_allocator::allocate(1);
                ZJ_construct(&(res->data), x);
                return res;
            }

            void delete_node(node* ptr) {
                ZJ_destroy(iterator(ptr, this));
                node_allocator::deallocate(ptr, sizeof(node));
            }

            void initialize_buckets(size_type n) {
                size_type bucket_num = next_prime(n);
                buckets.reserve(bucket_num);
                buckets.insert(buckets.end(), bucket_num, (node*)0);
                num_elements = 0;
            }

            size_type bucket_num(const value_type& x) const {
                return bucket_num_key(get_key(x));
            }

            size_type bucket_num(const value_type& x, size_type n) const {
                return bucket_num_key(get_key(x), n);
            }

            size_type bucket_num_key(const key_type& key) const {
                return bucket_num_key(key, bucket_size());
            }

            size_type bucket_num_key(const key_type& key, size_type n) const {
                return hash(key) % n;
            }

            void resize(size_type num) {
                if(num > bucket_size()) {
                    size_type new_bucket_size = next_prime(num);
                    vector<node*> new_buckets(new_bucket_size, (node*)0);
                    for(size_type i = 0; i < bucket_size(); ++i) {
                        node* first = buckets[i];
                        while(first) {
                            size_type new_bucket = bucket_num(first->data, new_bucket_size);
                            buckets[i] = first->next;
                            first->next = new_buckets[new_bucket];
                            new_buckets[new_bucket] = first;
                            first = buckets[i];
                        }
                    }
                    buckets.swap(new_buckets);
                }
            }
    };

}

#endif 


