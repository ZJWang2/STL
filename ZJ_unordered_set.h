


#ifndef _ZJ_UNORDERED_SET_
#define _ZJ_UNORDERED_SET_

#include "ZJ_hashtable.h"
#include "ZJ_functional.h"
#include "ZJ_pair.h"

namespace ZJ {

    template <class Key, class HashFcn = hash<Key>, class KeyEquals = equal<Key>>
    class unordered_set {
        private : 
            typedef hashtable<Key, Key, HashFcn, identity<Key>, KeyEquals> Container;
            Container c;
        
        public : 
            typedef typename Container::key_type    key_type;
            typedef typename Container::value_type  value_type;
            typedef typename Container::hasher      hasher;
            typedef typename Container::key_equal   key_equal;

            typedef typename Container::const_iterator  iterator; // read-only
            typedef typename Container::const_iterator  const_iterator;
            typedef typename Container::const_pointer   pointer;
            typedef typename Container::const_pointer   const_pointer;
            typedef typename Container::const_reference reference;
            typedef typename Container::const_reference const_reference;
            typedef typename Container::size_type       size_type;
            typedef typename Container::difference_type difference_type;

            unordered_set() : c() {}

            unordered_set(size_type n) : c(n, HashFcn(), KeyEquals()) {}

            unordered_set(size_type n, const hasher& hf) : c(n, hf, KeyEquals()) {}

            unordered_set(size_type n, const hasher& hf, const key_equal& eql) : c(n, hf, eql) {}

            unordered_set(const unordered_set& us) : c(us.c) {}

            hasher hash_function() const {return c.hash_function();}

            key_equal key_eq() const {return c.key_eq();}

            iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            size_type size() const {return c.size();}

            bool empty() const {return c.empty();}

            void swap(unordered_set& us) {c.swap(us.c);}

            pair<iterator, bool> insert(const value_type& value) {
                pair<typename Container::iterator, bool> p = c.insert_unique(value);
                return pair<iterator, bool>(p.first, p.second);
            }

            void insert(iterator first, iterator last) {
                c.insert_unique(first, last);
            }

            void erase(const key_type& key) {
                c.erase(key);
            }

            void erase(iterator pos) {
                c.erase(pos);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            // it doesnt make any sense to have equal_range() for unordered_set

            iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }

            void clear() {
                c.clear();
            }
    };

    template <class Key, class HashFcn = hash<Key>, class KeyEquals = equal<Key>>
    class unordered_multiset {
        private : 
            typedef hashtable<Key, Key, HashFcn, identity<Key>, KeyEquals> Container;
            Container c;
        
        public : 
            typedef typename Container::key_type    key_type;
            typedef typename Container::value_type  value_type;
            typedef typename Container::hasher      hasher;
            typedef typename Container::key_equal   key_equal;

            typedef typename Container::const_iterator  iterator; // read-only
            typedef typename Container::const_iterator  const_iterator;
            typedef typename Container::const_pointer   pointer;
            typedef typename Container::const_pointer   const_pointer;
            typedef typename Container::const_reference reference;
            typedef typename Container::const_reference const_reference;
            typedef typename Container::size_type       size_type;
            typedef typename Container::difference_type difference_type;

            unordered_multiset() : c(93, HashFcn(), KeyEquals()) {}

            unordered_multiset(size_type n) : c(n, HashFcn(), KeyEquals()) {}

            unordered_multiset(size_type n, const hasher& hf) : c(n, hf, KeyEquals()) {}

            unordered_multiset(size_type n, const hasher& hf, const key_equal& eql) : c(n, hf, eql) {}

            unordered_multiset(const unordered_multiset& us) : c(us.c) {}

            iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            size_type size() const {return c.size();}

            bool empty() const {return c.empty();}

            void swap(unordered_multiset& us) {c.swap(us.c);}

            iterator insert(const value_type& value) {
                return c.insert_equal(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_equal(first, last);
            }

            void erase(const key_type& key) {
                c.erase(key);
            }

            void erase(iterator pos) {
                c.erase(pos);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            pair<iterator, iterator> equal_range(const key_type& key) {
                return c.equal_range(key);
            }

            pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
                return c.equal_range(key);
            }

            iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }

            void clear() {
                c.clear();
            }
    };

}

#endif 

