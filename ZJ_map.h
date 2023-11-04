


#ifndef _ZJ_MAP_
#define _ZJ_MAP_

#include "ZJ_functional.h"
#include "ZJ_rb_tree.h"
#include "ZJ_pair.h"

namespace ZJ {

    template <class Key, class Value, class Compare = ZJ::less<Key>>
    class map {
        public : 
            typedef Key                     key_type;
            typedef Value                   data_type;
            typedef Value                   mapped_type;
            typedef pair<const Key, Value>  value_type;
            
            // key_compare: compares the key
            // value_compare here: actually compares first of the pair, which is the key
            typedef Compare                 key_compare;
            struct value_compare : public binary_function<value_type, value_type, bool> {
                Compare comp;
                value_compare(Compare c) : comp(c) {}
                bool operator() (const value_type& x, const value_type& y) const {
                    return comp(x.first, y.first);
                }
            };

        private :
            // value_type here is a pair, meaning both key and value are stored, not just value
            typedef rb_tree<key_type, value_type, select1st<value_type>, Compare> Container;
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

            map() : c() {}

            map(const map<Key, Value, Compare>& m) : c(m.c) {}

            key_compare key_comp() const {return c.key_comp();}

            value_compare value_comp() const {return value_compare(c.key_comp());}

            iterator begin() {return c.begin();}

            const_iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() {return c.end();}

            const_iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            bool empty() const {return c.empty();}

            size_type size() const {return c.size();}

            data_type& operator[] (const key_type& k) {
                return (*(insert(value_type(k, data_type())).first)).second;
            }

            pair<iterator, bool> insert(const pair<key_type, data_type>& value) {
                return c.insert_unique(value_type(value.first, value.second));
            }

            pair<iterator, bool> insert(const value_type& value) {
                return c.insert_unique(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_unique(first, last);
            }

            void erase(iterator it) {
                c.erase(it);
            }

            void erase(const key_type& k) {
                c.erase(k);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            void clear() {
                c.clear();
            }

            iterator find(const key_type& key) {
                return c.find(key);
            }

            const_iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }

            iterator upper_bound(const key_type& k) {
                return c.upper_bound(k);
            }

            const_iterator upper_bound(const key_type& k) const {
                return c.upper_bound(k);
            }

            iterator lower_bound(const key_type& k) {
                return c.lower_bound(k);
            }

            const_iterator lower_bound(const key_type& k) const {
                return c.lower_bound(k);
            }

            pair<iterator, iterator> equal_range(const key_type& k) {
                return c.equal_range(k);
            }

            pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
                return c.equal_range(k);
            }

            void swap(map& rhs) {
                c.swap(rhs.c);
            }
    };


    template <class Key, class Value, class Compare = ZJ::less<Key>>
    class multimap {
        public : 
            typedef Key                     key_type;
            typedef Value                   data_type;
            typedef Value                   mapped_type;
            typedef pair<const Key, Value>  value_type;

            typedef Compare                 key_compare;
            struct value_compare : public binary_function<value_type, value_type, bool> {
                Compare comp;
                value_compare(Compare c) : comp(c) {}
                bool operator() (const value_type& x, const value_type& y) const {
                    return comp(x.first, y.first);
                }
            };

        private :
            typedef rb_tree<key_type, value_type, select1st<value_type>, Compare> Container;
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

            multimap() : c() {}

            multimap(const multimap<Key, Value, Compare>& mm) : c(mm.c) {}

            key_compare key_comp() const {return c.key_comp();}

            value_compare value_comp() const {return value_compare(c.key_comp());}

            iterator begin() {return c.begin();}

            const_iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() {return c.end();}

            const_iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            bool empty() const {return c.empty();}

            size_type size() const {return c.size();}

            // [] operator is not supported for multimap
            /*
            data_type& operator[] (const key_type& k) {
                return (*(insert(value_type(k, data_type())).first)).second;
            }
            */

            iterator insert(const pair<key_type, data_type>& value) {
                return c.insert_equal(value_type(value.first, value.second));
            }

            iterator insert(const value_type& value) {
                return c.insert_equal(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_equal(first, last);
            }

            void erase(iterator it) {
                c.erase(it);
            }

            void erase(const key_type& k) {
                c.erase(k);
            }

            void erase(iterator first, iterator last) {
                c.erase(first, last);
            }

            void clear() {
                c.clear();
            }

            iterator find(const key_type& key) {
                return c.find(key);
            }

            const_iterator find(const key_type& key) const {
                return c.find(key);
            }

            size_type count(const key_type& key) const {
                return c.count(key);
            }

            iterator upper_bound(const key_type& k) {
                return c.upper_bound(k);
            }

            const_iterator upper_bound(const key_type& k) const {
                return c.upper_bound(k);
            }

            iterator lower_bound(const key_type& k) {
                return c.lower_bound(k);
            }

            const_iterator lower_bound(const key_type& k) const {
                return c.lower_bound(k);
            }

            pair<iterator, iterator> equal_range(const key_type& k) {
                return c.equal_range(k);
            }

            pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
                return c.equal_range(k);
            }

            void swap(multimap& rhs) {
                c.swap(rhs.c);
            }
    };

}

#endif 
