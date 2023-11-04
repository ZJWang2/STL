

#ifndef _ZJ_FUNCTIONAL_
#define _ZJ_FUNCTIONAL_


namespace ZJ {

    template <class Arg, class Res>
    struct unary_function {
        typedef Arg argument_type;
        typedef Res result_type;
    };

    template <class Arg1, class Arg2, class Res>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Res result_type;
    };

    

    template <class T>
    struct negate : public unary_function<T, T> {
        T operator()(const T& x) const {return -x;}
    };
/*
    template <class T>
    struct not : public unary_function<T, T> {
        T operator()(const T& x) const {return !x;}
    };
*/
    template <class T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& x1, const T& x2) const {return x1 + x2;}
    };

    template <class T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T& x1, const T& x2) const {return x1 - x2;}
    };

    template <class T>
    struct equal : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 == x2;}
    };

    template <class T>
    struct not_equal : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 != x2;}
    };

    template <class T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 > x2;}
    };

    template <class T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 < x2;}
    };

    template <class T>
    struct greater_equal : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 >= x2;}
    };

    template <class T>
    struct less_equal : public binary_function<T, T, bool> {
        bool operator()(const T& x1, const T& x2) const {return x1 <= x2;}
    };

    template <class T>
    struct identity : public unary_function<T, T> {
        const T& operator()(const T& x) const {return x;}
    };

    template <class Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type& operator() (const Pair& x) const {
            return x.first;
        }
    };

    template <class Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type& operator() (const Pair& x) const {
            return x.second;
        }
    };

}


#endif


