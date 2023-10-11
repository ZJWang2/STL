

#ifndef _ZJ_QUEUE_
#define _ZJ_QUEUE_

#include "ZJ_deque.h"
#include <iostream>
using namespace std;

namespace ZJ {

    template <typename T, typename Sequence = deque<T>>
    class queue {

        protected : 
            Sequence c;

        public : 
            typedef typename Sequence::value_type        value_type;
            typedef typename Sequence::reference         reference;
            typedef typename Sequence::const_reference   const_reference;
            typedef typename Sequence::size_type         size_type;
            typedef typename Sequence::difference_type   difference_type;

            queue() : c() {}

            bool empty() const { return c.empty(); }

            size_type size() const { return c.size(); }

            reference front() { return c.front(); }

            const_reference front() const { return c.front(); }

            reference back() { return c.back(); }

            const_reference back() const { return c.back(); }

            void push(const value_type& value) { c.push_back(value); }

            void pop() { c.pop_front(); }
        
    };

}


#endif


