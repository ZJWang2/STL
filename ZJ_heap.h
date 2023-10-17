

#ifndef _ZJ_HEAP_
#define _ZJ_HEAP_

#include <cstddef>

namespace ZJ {
    template <typename RAIter>
    inline void push_heap(RAIter first, RAIter last);
    template <typename RAIter>
    inline void pop_heap(RAIter first, RAIter last);


    template <typename RAIter>
    inline void push_heap(RAIter first, RAIter last) {
        __push_heap(first, last, get_difference_type(first), get_value_type(first));
    }

    template <typename RAIter>
    inline void pop_heap(RAIter first, RAIter last) {
        __pop_heap(first, last, get_difference_type(first), get_value_type(first));
    }

    template <typename RAIter, typename Distance, typename Value>
    inline void __push_heap(RAIter first, RAIter last, Distance, Value) {
        Distance hole = (last - first) - 1;
        Distance parent = (hole - 1) / 2;
        Value val = *(last - 1);
        while(hole > 0 && *(first + parent) < val) {
            *(first + hole) = *(first + parent);
            hole = parent;
            parent = (hole - 1) / 2;
        }
        *(first + hole) = val;
    }

    template <typename RAIter, typename Distance, typename Value>
    inline void __pop_heap(RAIter first, RAIter last, Distance, Value) {
        Distance hole = Distance(0);
        Distance len = last - first, leftChild = 2 * hole + 1;
        Value val = *(last - 1);
        while(leftChild + 1 < len) {
            if(*(first + leftChild) > *(first + leftChild + 1)) {
                *(first + hole) = *(first + leftChild);
                hole = leftChild;
            }
            else {
                *(first + hole) = *(first + leftChild + 1);
                hole = leftChild + 1;
            }
            leftChild = 2 * hole + 1;
        }
        if(leftChild + 1 == len) {
            *(first + hole) = *(first + leftChild);
            hole = leftChild;
        }
        *(first + hole) = val;
    }
}

#endif


