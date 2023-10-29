


#ifndef _ZJ_PAIR_
#define _ZJ_PAIR_

namespace ZJ {
    template <class T1, class T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        pair() : first(), second() {}

        pair(const T1& t1, const T2& t2) : first(t1), second(t2) {}
    };
}

#endif


