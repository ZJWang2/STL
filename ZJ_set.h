


#ifndef _ZJ_SET_
#define _ZJ_SET_

#include "ZJ_functional.h"
#include "ZJ_rb_tree.h"
#include "ZJ_pair.h"

namespace ZJ {
    
    template <class Key, class Compare = ZJ::less<Key>>
    class set {
        public : 
            typedef Key     key_type;
            typedef Key     value_type;
            typedef Compare key_compare;
            typedef Compare value_compare;

            typedef rb_tree<key_type, value_type, ZJ::identity<value_type>, Compare> Container;

            typedef typename Container::const_pointer   pointer; // read-only
            typedef typename Container::const_pointer   const_pointer; // read-only
            typedef typename Container::const_iterator  iterator; // read-only
            typedef typename Container::const_iterator  const_iterator; // read-only
            typedef typename Container::const_reference reference; // read-only
            typedef typename Container::const_reference const_reference; // read-only
            typedef typename Container::size_type       size_type;
            typedef typename Container::difference_type difference_type;

        private : 
            Container c;
        
        public : 
            set() : c() {}

            set(const set<Key, Compare>& s) : c(s.c) {}

            key_compare key_comp() const {return c.key_comp();}

            value_compare value_comp() const {return c.key_comp();}

            iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            bool empty() const {return c.empty();}

            size_type size() const {return c.size();}

            pair<iterator, bool> insert(const value_type& value) {
                pair<typename Container::iterator, bool> p = c.insert_unique(value);
                return pair<iterator, bool>(p.first, p.second);
                //return c.insert_unique(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_unique(first, last);
            }

            void erase(iterator it) {
                typedef typename Container::iterator iter;
                c.erase((iter&)it);
            }

            void erase(iterator first, iterator last) {
                typedef typename Container::iterator iter;
                c.erase((iter&)first, (iter&)last);
            }

            void erase(const value_type& value) {
                c.erase(value);
            }

            void clear() {
                c.clear();
            }

            iterator find(const value_type& value) {
                return c.find(value);
            }

            const_iterator find(const value_type& value) const {
                return c.find(value);
            }

            size_type count(const value_type& value) const {
                return c.count(value);
            }

            iterator lower_bound(const value_type& value) {
                return c.lower_bound(value);
            }

            const_iterator lower_bound(const value_type& value) const {
                return c.lower_bound(value);
            }

            iterator upper_bound(const value_type& value) {
                return c.upper_bound(value);
            }

            const_iterator upper_bound(const value_type& value) const {
                return c.upper_bound(value);
            }

            pair<iterator, iterator> equal_reange(const value_type& value) {
                return c.equal_range(value);
            }

            pair<const_iterator, const_iterator> equal_reange(const value_type& value) const {
                return c.equal_range(value);
            }

            void swap(set& rhs) {
                c.swap(rhs.c);
            }
    };


    template <class Key, class Compare = less<Key>>
    class multiset {
        public : 
            typedef Key    key_type;
            typedef Key     value_type;
            typedef Compare key_compare;
            typedef Compare value_compare;

            typedef rb_tree<key_type, value_type, ZJ::identity<value_type>, Compare> Container;

            typedef typename Container::const_pointer   pointer; // read-only
            typedef typename Container::const_pointer   const_pointer; // read-only
            typedef typename Container::const_iterator  iterator; // read-only
            typedef typename Container::const_iterator  const_iterator; // read-only
            typedef typename Container::const_reference reference; // read-only
            typedef typename Container::const_reference const_reference; // read-only
            typedef typename Container::size_type       size_type;
            typedef typename Container::difference_type difference_type;
        
        private :
            Container c;
        
        public : 
            multiset() : c() {}

            multiset(const multiset<Key, Compare>& ms) : c(ms.c) {}

            key_compare key_comp() const {return c.key_comp();}

            value_compare value_comp() const {return c.key_comp();}

            iterator begin() const {return c.begin();}

            const_iterator cbegin() const {return c.cbegin();}

            iterator end() const {return c.end();}

            const_iterator cend() const {return c.cend();}

            bool empty() const {return c.empty();}

            size_type size() const {return c.size();}

            iterator insert(const value_type& value) {
                return c.insert_equal(value);
            }

            void insert(iterator first, iterator last) {
                c.insert_equal(first, last);
            }

            void erase(iterator it) {
                typedef typename Container::iterator iter;
                c.erase((iter&)it);
            }

            void erase(iterator first, iterator last) {
                typedef typename Container::iterator iter;
                c.erase((iter&)first, (iter&)last);
            }

            void erase(const value_type& value) {
                c.erase(value);
            }

            void clear() {
                c.clear();
            }

            iterator find(const value_type& value) {
                return c.find(value);
            }

            const_iterator find(const value_type& value) const {
                return c.find(value);
            }

            size_type count(const value_type& value) const {
                return c.count(value);
            }

            iterator lower_bound(const value_type& value) {
                return c.lower_bound(value);
            }

            const_iterator lower_bound(const value_type& value) const {
                return c.lower_bound(value);
            }

            iterator upper_bound(const value_type& value) {
                return c.upper_bound(value);
            }

            const_iterator upper_bound(const value_type& value) const {
                return c.upper_bound(value);
            }

            pair<iterator, iterator> equal_reange(const value_type& value) {
                return c.equal_range(value);
            }

            pair<const_iterator, const_iterator> equal_reange(const value_type& value) const {
                return c.equal_range(value);
            }

            void swap(multiset& rhs) {
                c.swap(rhs.c);
            }

    };
}

#endif
