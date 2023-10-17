

#ifndef _ZJ_STACK_
#define _ZJ_STACK_

#include "ZJ_deque.h"
#include <iostream>
using namespace std;

namespace ZJ {

    template <typename T, typename Container = deque<T>>
    class stack {

        protected : 
            Container c;

        public : 
            typedef typename Container::value_type        value_type;
            typedef typename Container::reference         reference;
            typedef typename Container::const_reference   const_reference;
            typedef typename Container::size_type         size_type;
            typedef typename Container::difference_type   difference_type;

            stack() : c() {}

            bool empty() const { return c.empty(); }

            size_type size() const { return c.size(); }

            reference top() { return c.back(); }

            const_reference top() const { return c.back(); }

            void push(const value_type& value) { c.push_back(value); }

            void pop() { c.pop_back(); }
        
    };

}


#endif


