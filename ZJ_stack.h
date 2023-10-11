

#ifndef _ZJ_STACK_
#define _ZJ_STACK_

#include "ZJ_deque.h"
#include <iostream>
using namespace std;

namespace ZJ {

    template <typename T, typename Sequence = deque<T>>
    class stack {

        protected : 
            Sequence c;

        public : 
            typedef typename Sequence::value_type        value_type;
            typedef typename Sequence::reference         reference;
            typedef typename Sequence::const_reference   const_reference;
            typedef typename Sequence::size_type         size_type;
            typedef typename Sequence::difference_type   difference_type;

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


