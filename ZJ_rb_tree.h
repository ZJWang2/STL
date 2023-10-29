

#ifndef _ZJ_RB_TREE_
#define _ZJ_RB_TREE_

#include "ZJ_iterator.h"
#include "ZJ_alloc.h"
#include "ZJ_utils.h"
#include "ZJ_pair.h"
#include "ZJ_queue.h"
#include "ZJ_pair.h"

namespace ZJ {

    typedef bool COLOR_TYPE;
    const COLOR_TYPE RED = false;
    const COLOR_TYPE BLACK = true;

    struct rb_node_base {
        typedef rb_node_base*   node_pointer_base;
        
        COLOR_TYPE color;
        node_pointer_base parent;
        node_pointer_base left;
        node_pointer_base right;

        rb_node_base(COLOR_TYPE c, node_pointer_base p, node_pointer_base l, node_pointer_base r) : 
        color(c), parent(p), left(l), right(r) {}

        rb_node_base(const rb_node_base& rbnb) {
            color = rbnb.color;
            parent = rbnb.parent;
            left = rbnb.left;
            right = rbnb.right;
        }

        static node_pointer_base minimum(node_pointer_base x) {
            while (x->left != 0) x = x->left;
            return x;
        }

        static node_pointer_base maximum(node_pointer_base x) {
            while (x->right != 0) x = x->right;
            return x;
        }

    };

    template <typename T>
    struct rb_node : public rb_node_base{
        typedef rb_node<T>*     node_pointer;
        T data;

        rb_node(COLOR_TYPE c, node_pointer_base p, node_pointer_base l, node_pointer_base r, T d) : 
        rb_node_base(c, p, l, r), data(d) {}

        rb_node(rb_node<T>& rbn) {
            color = rbn.color;
            parent = rbn.parent;
            left = rbn.left;
            right = rbn.right;
            data = rbn.data;
        }
    };

    struct rb_iterator_base {
        typedef rb_node_base::node_pointer_base     node_pointer_base;
        typedef bidirectional_iterator_tag          iterator_category;

        node_pointer_base node;

        void increment() {
            if(node->right != 0) {
                node = node->right;
                while(node->left != 0) node = node->left;
            } 
            else {
                node_pointer_base p = node->parent;
                while(p->right == node) {
                    node = p;
                    p = p->parent;
                }
                if(node->right != p) // brilliant design (header node)
                node = p;
            }
        }

        void decrement() {
            if(node->color == RED && node->parent->parent == node) node = node->right;
            else if(node->left != 0) {
                node = node->left;
                while(node->right != 0) node = node->right;
            }
            else {
                node_pointer_base p = node->parent;
                while(p->left == node) {
                    node = p;
                    p = p->parent;
                }
                //if(node->left != p) 
                node = p;
            }
        }

    };

    template <typename T, typename pointer, typename reference>
    struct rb_iterator : rb_iterator_base {
        typedef T                                   value_type;
        typedef rb_iterator<T, T*, T&>              iterator;
        typedef rb_iterator<T, const T*, const T&>  const_iterator;
        typedef rb_iterator<T, pointer, reference>  self;
        typedef rb_node<T>*                         node_pointer;

        rb_iterator() {}
        rb_iterator(node_pointer ptr) { node = ptr; }
        rb_iterator(const iterator& it) { node = it.node; }

        reference operator*() {return node_pointer(node)->data; }
        //pointer operator->() {return &(operator*()); }

        self operator++() {
            increment();
            return *this;
        }

        self operator++(int) {
            self res = *this;
            increment();
            return res;
        }

        self operator--() {
            decrement();
            return *this;
        }

        self operator--(int) {
            self res = *this;
            decrement();
            return res;
        }

        bool operator==(const self& it) {
            return node == it.node; 
        }

        bool operator!=(const self& it) {
            return node != it.node;
        }

    };

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = allocator<rb_node<Value>>>
    class rb_tree {

        public :
            typedef Key             key_type;
            typedef Value           value_type;
            typedef Value*          pointer;
            typedef const Value*    const_pointer;
            typedef Value&          reference;
            typedef const Value&    const_reference;
            typedef ptrdiff_t       difference_type;
            typedef size_t          size_type;

            typedef rb_iterator<value_type, pointer, reference>                 iterator;
            typedef rb_iterator<value_type, const_pointer, const_reference>     const_iterator;

            typedef rb_node_base*   base_pointer;
            typedef rb_node<Value>  node;
            typedef rb_node<Value>* node_pointer;

        protected : 
            size_type node_count;
            node_pointer header;
            Compare key_compare;

        public : 
            rb_tree() : node_count(0), key_compare(Compare()) {
                header = get_node();
                root() = 0;
                header->color = RED;
                header->left = header;
                header->right = header;
            }

            rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rbt) {
                header = get_node();
                root() = 0;
                header->color = RED;
                header->left = header;
                header->right = header;
                key_compare = rbt.key_compare;
                if(rbt.root() != 0) {
                    copy_tree((base_pointer)rbt.root(), (base_pointer&)root());
                    root()->parent = header;
                    header->parent = root();
                    leftmost() = (node_pointer)rb_node_base::minimum(base_pointer(root()));
                    rightmost() = (node_pointer)rb_node_base::maximum(base_pointer(root()));
                }
                node_count = rbt.node_count;
            }

            ~rb_tree() {
                clear();
                put_node(header);
            }

            iterator begin() {return leftmost();}

            iterator end() {return header;}
            
            bool empty() {return node_count == 0;}
            
            size_type size() {return node_count;}

            void clear() {
                if(node_count != 0) {
                    //erase(begin(), end());
                    erase_aux(root());
                    leftmost() = header;
                    root() = 0;
                    rightmost() = header;
                    node_count = 0;
                }
            }

            iterator insert_equal(const value_type& value) {
                node_pointer x = root();
                node_pointer y = header;
                Key kov = KeyOfValue()(value);
                while(x != 0) { 
                    y = x;
                    x = key_compare(kov, key(x)) ? left(x) : right(x);
                }
                return __insert(x, y, value);
            }

            pair<iterator, bool> insert_unique(const value_type& value) {
                node_pointer x = root();
                node_pointer y = header;
                Key kov = KeyOfValue()(value);
                bool cmp = true;
                while(x != 0) {
                    y = x;
                    cmp = key_compare(kov, key(x));
                    x = cmp ? left(x) : right(x);
                }
                iterator j = y;
                if(cmp)
                    if(j == begin())
                        return pair<iterator, bool>(__insert(x, y, value), true);
                    else
                        --j;
                if(key_compare(key(j.node), kov))
                    return pair<iterator, bool>(__insert(x, y, value), true);
                return pair<iterator, bool>(j, false);
            }

            void erase(iterator pos) {
                node_pointer y = (node_pointer)rebalance_for_erase(pos.node);
                destroy_node(y);
                --node_count;
            }

            void erase(iterator first, iterator last) {
                if(first == begin() && last == end()) clear();
                else {
                    while(first != last) 
                        erase(first++);
                }
            }

            void erase(const Key& k) {
                iterator it_upper = upper_bound(k);
                iterator it_lower = lower_bound(k);
                erase(it_lower, it_upper);
            }

            iterator find(const Key& k) { 
                node_pointer y = header;
                node_pointer x = root();
                while(x != 0) {
                    if(!key_compare(key(x), k)) {
                        y = x;
                        x = left(x);
                    }
                    else x = right(x);
                }
                iterator j = y;
                return (j == end() || key_compare(k, key(j.node))) ? end() : j;
            }

            size_type count(const Key& k) {
                iterator it_upper = upper_bound(k);
                iterator it_lower = lower_bound(k);
                // compute the distance between two iterators
                // could use distance(), which is O(n)
                // this go-through is O(n) as well, same as distance()
                size_type res = 0;
                while(it_lower != it_upper) {
                    ++res;
                    ++it_lower;
                }
                return res;
            }

            iterator lower_bound(const Key& k) {
                node_pointer y = header;
                node_pointer x = root();
                while(x != 0) {
                    if(!key_compare(key(x), k)) {
                        y = x;
                        x = left(x);
                    }
                    else x = right(x);
                }
                return iterator(y);
            }

            iterator upper_bound(const Key& k) {
                node_pointer y = header;
                node_pointer x = root();
                while(x != 0) {
                    if(key_compare(k, key(x))) {
                        y = x;
                        x = left(x);
                    }
                    else x = right(x);
                }
                return iterator(y);
            }

            void print() { //return ;
                node_pointer cur = header;
                cout << "size: " << size() << endl;
                cout << "header: " << cur << "  left: " << cur->left << "  right: " << cur->right << "  parent: " << cur->parent << "  color: " << cur->color << "  data: " << cur->data << endl;
                queue<pair<node_pointer, int>> q;
                q.push(pair<node_pointer, int>(root(), 1));
                while(!q.empty()) {
                    int level = q.front().second;
                    cur = q.front().first; q.pop();
                    cout << level << "-node: " << cur << "  left: " << cur->left << "  right: " << cur->right << "  parent: " << cur->parent << "  color: " << cur->color << "  data: " << cur->data << endl;
                    if(cur->left) q.push(pair<node_pointer, int>((node_pointer)cur->left, level + 1));
                    if(cur->right) q.push(pair<node_pointer, int>((node_pointer)cur->right, level + 1));
                } cout << "\n\n";
            }

        protected :
            iterator __insert(base_pointer x_, base_pointer y_, const value_type& value) {
                node_pointer x = (node_pointer)x_;
                node_pointer y = (node_pointer)y_;
                node_pointer z; 
                if(x != 0 || key_compare(KeyOfValue()(value), key(y))){
                    z = create_node(value);
                    left(y) = z;
                    if(y == header) {
                        root() = z;
                        rightmost() = z;
                        leftmost() = z;
                    }
                    else if(y == leftmost()) leftmost() = z;
                }
                else {
                    z = create_node(value);
                    right(y) = z;
                    if(y == rightmost()) rightmost() = z;
                }
                parent(z) = y;
                left(z) = 0;
                right(z) = 0;
                rebalance(z);
                ++node_count;
                return iterator(z);
            }

            inline void rebalance(base_pointer x) {
                x->color = RED;
                while(x != root() && x->parent->color == RED) {
                    if(x->parent == x->parent->parent->left) {
                        base_pointer y = x->parent->parent->right; // y is x's uncle
                        if(y && y->color == RED) {
                            x->parent->color = BLACK;
                            y->color = BLACK;
                            x->parent->parent->color = RED;
                            x = x->parent->parent;
                        }
                        else {
                            if(x == x->parent->right) { // left-right: inside
                                x = x->parent;
                                rotate_left(x);
                            }
                            x->parent->color = BLACK;
                            x->parent->parent->color = RED;
                            rotate_right(x->parent->parent);
                        }
                    }
                    else {
                        base_pointer y = x->parent->parent->left; // y is x's uncle
                        if(y && y->color == RED) {
                            x->parent->color = BLACK;
                            y->color = BLACK;
                            x->parent->parent->color = RED;
                            x = x->parent->parent;
                        }
                        else {
                            if(x == x->parent->left) { // right-left: inside
                                x = x->parent;
                                rotate_right(x);
                            }
                            x->parent->color = BLACK;
                            x->parent->parent->color = RED;
                            rotate_left(x->parent->parent);
                        }
                    }
                }
                root()->color = BLACK;
            }

            base_pointer rebalance_for_erase(base_pointer z) {
                base_pointer y = z;
                base_pointer x = 0;
                base_pointer x_parent = 0;
                if(y->left == 0) x = y->right;
                else if(y->right == 0) x = y->left;
                else {
                    iterator it((node_pointer)y);
                    ++it;
                    y = (node_pointer)it.node;
                    x = y->right;
                    //y = y->right;
                    //while(y->left != 0) y = y->left;
                    //x = y->right;
                }

                if(y != z) { 
                    // this is the case that y is z's successor
                    // delete the successor 
                    z->left->parent = y;
                    y->left = z->left;
                    if(y != z->right) {
                        x_parent = y->parent;
                        if(x) x->parent = y->parent;
                        y->parent->left = x;
                        y->right = z->right;
                        z->right->parent = y;
                    }
                    else x_parent = y;
                    if(root() == (node_pointer)z) root() = (node_pointer)y;
                    else if(z->parent->left == z) z->parent->left = y;
                    else z->parent->right = y;
                    y->parent = z->parent;
                    ZJ_swap(y->color, z->color);
                    y = z;
                }
                else { 
                    // this is the case that z has only one child
                    x_parent = y->parent;
                    if(x) x->parent = y->parent;
                    if(root() == (node_pointer)z) root() = (node_pointer)x;
                    else if(z->parent->left == z) z->parent->left = x;
                    else z->parent->right = x;
                    if(leftmost() == (node_pointer)z) {
                        if(z->right == 0) leftmost() = node_pointer(z->parent);
                        else leftmost() = node_pointer(rb_node_base::minimum(x));
                    }
                    if(rightmost() == (node_pointer)z) {
                        if(z->left == 0) rightmost() = node_pointer(z->parent);
                        else rightmost() = node_pointer(rb_node_base::maximum(x));
                    }
                }   
                
                if(y->color != RED) { 
                    // y here is actually z, the node to be deleted                        
                    while((node_pointer)x != root() && (x == 0 || x->color == BLACK)) {
                        if(x == x_parent->left) {
                            base_pointer w = x_parent->right;
                            if(w->color == RED) {
                                w->color = BLACK;
                                x_parent->color = RED;
                                rotate_left(x_parent);
                                w = x_parent->right;
                            }
                            if((w->left == 0 || w->left->color == BLACK) && (w->right == 0 || w->right->color == BLACK)) {
                                w->color = RED;
                                x = x_parent;
                                x_parent = x_parent->parent;
                            }
                            else {
                                if(w->right == 0 || w->right->color == BLACK) {
                                    if(w->left) w->left->color = BLACK;
                                    w->color = RED;
                                    rotate_right(w);
                                    w = x_parent->right;
                                }
                                w->color = x_parent->color;
                                x_parent->color = BLACK;
                                if(w->right) w->right->color = BLACK;
                                rotate_left(x_parent);
                                break;
                            }
                        }
                        else {
                            base_pointer w = x_parent->left;
                            if(w->color == RED) {
                                w->color = BLACK;
                                x_parent->color = RED;
                                rotate_right(x_parent);
                                w = x_parent->left;
                            }
                            if((w->right == 0 || w->right->color == BLACK) && (w->left == 0 || w->left->color == BLACK)) {
                                w->color = RED;
                                x = x_parent;
                                x_parent = x_parent->parent;
                            }
                            else {
                                if(w->left == 0 || w->left->color == BLACK) {
                                    if(w->right) w->right->color = BLACK;
                                    w->color = RED;
                                    rotate_left(w);
                                    w = x_parent->left;
                                }
                                w->color = x_parent->color;
                                x_parent->color = BLACK;
                                if(w->left) w->left->color = BLACK;
                                rotate_right(x_parent);
                                break;
                            }
                        }
                    }
                    if(x) x->color = BLACK;
                }
                return y;
            }

            
/*          // this is an iterative version of erase_aux
            void erase_aux(base_pointer x) {
                // erase without rebalancing
                while(x != 0) {
                    erase_aux(right(x));
                    base_pointer y = left(x);
                    destroy_node((node_pointer)x);
                    x = y;
                }
            }
*/

            // this is a recursive version of erase_aux
            void erase_aux(base_pointer x) {
                // erase without rebalancing
                if(x != 0) {
                    erase_aux(right(x));
                    erase_aux(left(x));
                    destroy_node((node_pointer)x);
                }
            }

            /** left rotate
             *          X                 Y
             *         / \               / \             
             *        A   Y    ==>      X   C
             *            /\           / \
             *           B  C         A   B
            */
            inline void rotate_left(base_pointer x) {// cout << "left ";
                base_pointer y = x->right;
                //base_pointer A = x->left, B = y->left, C = y->right;
                base_pointer B = y->left;

                x->right = B;
                if(B) B->parent = x;
                y->parent = x->parent;

                if((node_pointer)x == root()) root() = (node_pointer)y;
                else if(x == x->parent->left) x->parent->left = y;
                else x->parent->right = y;

                y->left = x;
                x->parent = y;
            }

            /** right rotate
             *          X                 Y
             *         / \               / \             
             *        Y   C    ==>      A   X
             *       /\                    / \
             *      A  B                  B   C
            */
            inline void rotate_right(base_pointer x) {//cout << "right ";
                base_pointer y = x->left;
                //base_pointer A = y->left, B = y->right, C = x->left;
                base_pointer B = y->right;

                x->left = B;
                if(B) B->parent = x;
                y->parent = x->parent;

                if((node_pointer)x == root()) root() = (node_pointer)y;
                else if(x == x->parent->left) x->parent->left = y;
                else x->parent->right = y;

                y->right = x;
                x->parent = y;
            }

            node_pointer& root() const {return (node_pointer&)(header->parent);}
            node_pointer& leftmost() const {return (node_pointer&)(header->left);}
            node_pointer& rightmost() const {return (node_pointer&)(header->right);}

            static node_pointer& left(node_pointer x) {return (node_pointer&)(x->left); }
            static node_pointer& left(base_pointer x) {return (node_pointer&)(x->left); }
            static node_pointer& right(node_pointer x) {return (node_pointer&)(x->right); }
            static node_pointer& right(base_pointer x) {return (node_pointer&)(x->right); }
            static node_pointer& parent(node_pointer x) {return (node_pointer&)(x->parent); }
            static node_pointer& parent(base_pointer x) {return (node_pointer&)(x->parent); }
            static reference value(node_pointer x) {return x->data; }
            static reference value(base_pointer x) {return ((node_pointer)x)->data;}
            static const Key& key(node_pointer x) {return KeyOfValue()(value(x));}
            static const Key& key(base_pointer x) {return KeyOfValue()(value(x));}
            static COLOR_TYPE& color(node_pointer x) {return x->color;}
            static COLOR_TYPE& color(base_pointer x) {return ((node_pointer)x)->color;}
        
            node_pointer get_node() {
                return Alloc::allocate(1);
            }

            void put_node(node_pointer ptr) {
                Alloc::deallocate(ptr, sizeof(ptr));
            }

            node_pointer create_node(const value_type& value) {
                node_pointer res = get_node();
                ZJ_construct(&(res->data), value);
                return res;
            }

            node_pointer clone_node(node_pointer ptr) {
                node_pointer res = create_node(ptr->data);
                res->color = ptr->color;
                res->parent = 0;
                res->left = 0;
                res->right = 0;
                return res;
                
                /*node_pointer res = get_node();
                ZJ_construct(res, *ptr);
                return res;*/
            }

            void copy_tree(base_pointer from, base_pointer& to) {
                if(from == 0) return ;
                to = clone_node((node_pointer)from);
                if(from->left != 0) {
                    copy_tree(from->left, to->left);
                    to->left->parent = to;
                }
                if(from->right != 0) {
                    copy_tree(from->right, to->right);
                    to->right->parent = to;
                }
            }

            void destroy_node(node_pointer ptr) {
                ZJ_destroy(iterator(ptr));
                Alloc::deallocate(ptr, sizeof(ptr));
            }

};

}

#endif
