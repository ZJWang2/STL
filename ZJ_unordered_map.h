

#ifndef _ZJ_UNORDERED_SET_
#define _ZJ_UNORDERED_SET_

#include "ZJ_hashtable.h"
#include "ZJ_functional.h"
#include "ZJ_pair.h"


namespace ZJ {

    template <class Key, class Value, class HashFcn = hash<Key>, class KeyEquals = equal<Key>>
    class unordered_map {
        public : 
            typedef Key                    key_type;
            typedef Value                  data_type;
            typedef pair<const Key, Value> value_type;
            typedef HashFcn                hasher;
            typedef KeyEquals              key_equal;
        
        private :
            typedef hashtable<key_type, value_type, hasher, select1st<value_type>, key_equal> Container;
            Container c;
        
        public : 
            typedef typename Container::pointer            pointer;
            typedef typename Container::const_pointer      const_pointer;
            typedef typename Container::reference          reference;
            typedef typename Container::const_reference    const_reference;
            typedef typename Container::iterator           iterator;
            typedef typename Container::const_iterator     const_iterator;
            typedef typename Container::size_type          size_type;
            typedef typename Container::difference_type    difference_type;

            unordered_map() : c() {}

            unordered_map(size_type n) : c(n, hasher(), key_equal()) {}

            unordered_map(size_type n, const hasher& hf) : c(n, hf, key_equal()) {}

            unordered_map(size_type n, const hasher& hf, const key_equal& eql) : c(n, hf, eql) {}

            unordered_map(const unordered_map& um) : c(um.c) {}

            data_type& operator[] (const key_type& k) {
                return (*(insert(value_type(k, data_type())).first)).second;
            }

            hasher hash_function() const {return c.hash_function();}

            key_equal key_eq() const {return c.key_eq();}

            iterator begin() {return c.begin();}

            const_iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() {return c.end();}

            const_iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            size_type size() const {return c.size();}

            bool empty() const {return c.empty();}

            void swap(unordered_map& um) {c.swap(um.c);}
            
            pair<iterator, bool> insert(const value_type& value) {
                return c.insert_unique(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_unique(first, last);
            }

            void erase(const key_type& k) {
                c.erase(k);
            }

            void erase(iterator it) {
                c.erase(it);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            void clear() {
                c.clear();
            }

            // it doesnt make any sense to have equal_range() for unordered_map

            iterator find(const key_type& key) {
                return c.find(key);
            }

            iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }
    };

    template <class Key, class Value, class HashFcn = hash<Key>, class KeyEquals = equal<Key>>
    class unordered_multimap {
        public : 
            typedef Key                    key_type;
            typedef Value                  data_type;
            typedef pair<const Key, Value> value_type;
            typedef HashFcn                hasher;
            typedef KeyEquals              key_equal;
        
        private :
            typedef hashtable<key_type, value_type, hasher, select1st<value_type>, key_equal> Container;
            Container c;
        
        public : 
            typedef typename Container::pointer            pointer;
            typedef typename Container::const_pointer      const_pointer;
            typedef typename Container::reference          reference;
            typedef typename Container::const_reference    const_reference;
            typedef typename Container::iterator           iterator;
            typedef typename Container::const_iterator     const_iterator;
            typedef typename Container::size_type          size_type;
            typedef typename Container::difference_type    difference_type;

            unordered_multimap() : c() {}

            unordered_multimap(size_type n) : c(n, hasher(), key_equal()) {}

            unordered_multimap(size_type n, const hasher& hf) : c(n, hf, key_equal()) {}

            unordered_multimap(size_type n, const hasher& hf, const key_equal& eql) : c(n, hf, eql) {}

            unordered_multimap(const unordered_multimap& um) : c(um.c) {}

            // [] operator is not supported for unordered_multimap

            hasher hash_function() const {return c.hash_function();}

            key_equal key_eq() const {return c.key_eq();}

            iterator begin() {return c.begin();}

            const_iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() {return c.end();}

            const_iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            size_type size() const {return c.size();}

            bool empty() const {return c.empty();}

            void swap(unordered_multimap& um) {c.swap(um.c);}
            
            iterator insert(const pair<key_type, data_type>& value) {
                return c.insert_equal(value_type(value.first, value.second));
            }
            
            iterator insert(const value_type& value) {
                return c.insert_equal(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_equal(first, last);
            }

            void erase(const key_type& k) {
                c.erase(k);
            }

            void erase(iterator it) {
                c.erase(it);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            void clear() {
                c.clear();
            }

            pair<iterator, iterator> equal_range(const key_type& key) {
                return c.equal_range(key);
            }

            pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
                return c.equal_range(key);
            }

            iterator find(const key_type& key) {
                return c.find(key);
            }

            iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }
    };

}


#endif

