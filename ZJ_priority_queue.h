

#ifndef _ZJ_PRIORITY_QUEUE_
#define _ZJ_PRIORITY_QUEUE_

#include <cstddef>
#include "ZJ_heap.h"
#include "ZJ_vector.h"
#include "ZJ_utils.h"
#include <iostream>
using namespace ZJ;

namespace ZJ {
    
    template <typename T, typename Container = vector<int>>
    class priority_queue {
        public : 
            typedef typename Container::value_type          value_type;
            typedef typename Container::pointer             pointer;
            typedef typename Container::const_pointer       const_pointer;
            typedef typename Container::reference           reference;
            typedef typename Container::const_reference     const_reference;
            typedef typename Container::size_type           size_type;
            typedef typename Container::difference_type     difference_type;

        protected : 
            //vector<int> c;
            Container c;
        
        public : 
            priority_queue() : c(0) {}

            bool empty() const { return c.empty(); }

            size_type size() const { return c.size(); }

            const_reference top() const { return c.front(); }

            void push(const value_type& value) {
                c.push_back(value);
                push_heap(c.begin(), c.end());
            }

            void pop() {
                pop_heap(c.begin(), c.end());
                c.pop_back();
            }
            
            /*
            void print() {
                cout << "vector size = " << c.size() << endl;
                for(int i=0; i<c.size(); i++)
                    cout << c[i] << " " ;
                cout << "\n\n" ;
            }
            */

    };
    
}


#endif

