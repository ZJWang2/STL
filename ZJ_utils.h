
#ifndef _ZJ_UTILS_
#define _ZJ_UTILS_


namespace ZJ {

    template <typename OutputIter, typename Value>
    inline void ZJ_construct(OutputIter p, const Value& value);
    template <typename OutputIter, typename Value>
    inline void ZJ_construct(OutputIter first, OutputIter last, const Value& value);
    template <typename OutputIter>
    inline void ZJ_destroy(OutputIter p);
    template <typename OutputIter>
    inline void ZJ_destroy(OutputIter first, OutputIter last);
    template <typename InputIter, typename OutputIter>
    inline OutputIter ZJ_uninitialized_copy(InputIter first, InputIter last, OutputIter dest);
    template <typename OutputIter, typename Value>
    inline void ZJ_uninitialized_fill(OutputIter first, OutputIter last, const Value& value);
    template <typename OutputIter, typename Value>
    inline OutputIter ZJ_uninitialized_fill_n(OutputIter dest, size_t n, const Value& value);
    template <typename InputIter, typename OutputIter>
    inline OutputIter ZJ_copy(InputIter first, InputIter last, OutputIter dest, bool copy2left = true);
    template <typename T>
    inline void ZJ_swap(T& a, T& b);
    template<typename Iter>
    inline typename Iter::value_type get_value_type(Iter iter);
    template<typename Iter>
    inline typename Iter::size_type get_size_type(Iter iter);
    template<typename Iter>
    inline typename Iter::difference_type get_difference_type(Iter iter);

    struct TRUE_TAG {};
    struct FALSE_TAG {};

    template <typename T>
    class traits {
        public : 
            typedef FALSE_TAG IS_POD;
    };

    template <>
    class traits<int> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<float> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<double> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<long> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<short> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<bool> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <>
    class traits<char> {
        public : 
            typedef TRUE_TAG IS_POD;
    };

    template <typename OutputIter, typename Value>
    inline void ZJ_construct(OutputIter p, const Value& value) {
        //new(const_cast<Value*>(&*p)) Value(value);
        new(&*p) Value(value);
    }

    template <typename OutputIter, typename Value>
    inline void ZJ_construct(OutputIter first, OutputIter last, const Value& value) {
        for(; first != last; ++first)
            ZJ_construct(first, value);
    }

    template <typename OutputIter>
    inline void __ZJ_destroy(OutputIter first, OutputIter last, TRUE_TAG) {}

    template <typename OutputIter>
    inline void __ZJ_destroy(OutputIter first, OutputIter last, FALSE_TAG) {
        typedef typename OutputIter::value_type value_type;
        for(; first != last; ++first)
            first->~value_type();
    }

    template <typename OutputIter>
    inline void __ZJ_destroy(OutputIter p, TRUE_TAG) {}

    template <typename OutputIter>
    inline void __ZJ_destroy(OutputIter p, FALSE_TAG) {
        typedef typename OutputIter::value_type value_type;
        p->~value_type();
    }

    template <typename OutputIter>
    inline void ZJ_destroy(OutputIter p) {
        typedef typename traits<typename OutputIter::value_type>::IS_POD IS_POD;
        //p->~value_type();
        __ZJ_destroy(p, IS_POD());
    }

    template <typename OutputIter>
    inline void ZJ_destroy(OutputIter first, OutputIter last) {
        typedef typename traits<typename OutputIter::value_type>::IS_POD IS_POD;
        __ZJ_destroy(first, last, IS_POD());
    }

    template <typename InputIter, typename OutputIter>
    inline OutputIter __ZJ_uninitialized_copy(InputIter first, InputIter last, OutputIter dest, TRUE_TAG) {
        return copy(first, last, dest);
    }

    template <typename InputIter, typename OutputIter>
    inline OutputIter __ZJ_uninitialized_copy(InputIter first, InputIter last, OutputIter dest, FALSE_TAG) {
        for(; first != last; ++first, ++dest)
            ZJ_construct(dest, *first);
        return dest;
    }

    template <typename InputIter, typename OutputIter>
    inline OutputIter ZJ_uninitialized_copy(InputIter first, InputIter last, OutputIter dest) {
        typedef typename traits<typename InputIter::value_type>::IS_POD IS_POD;
        return __ZJ_uninitialized_copy(first, last, dest, IS_POD());
    }

    template <typename OutputIter, typename Value>
    inline void __ZJ_uninitialized_fill(OutputIter first, OutputIter last, const Value& value, TRUE_TAG) {
        fill(first, last, value);
    }

    template <typename OutputIter, typename Value>
    inline void __ZJ_uninitialized_fill(OutputIter first, OutputIter last, const Value& value, FALSE_TAG) {
        for(; first != last; ++first)
            ZJ_construct(first, value);
    }

    template <typename OutputIter, typename Value>
    inline void ZJ_uninitialized_fill(OutputIter first, OutputIter last, const Value& value) {
        typedef typename traits<typename OutputIter::value_type>::IS_POD IS_POD;
        __ZJ_uninitialized_fill(first, last, value, IS_POD());
    }

    template<typename OutputIter, typename Value>
    inline OutputIter __ZJ_uninitialized_fill_n(OutputIter dest, size_t n, const Value& value, TRUE_TAG) {
        return fill_n(dest, n, value);
    }

    template<typename OutputIter, typename Value>
    inline OutputIter __ZJ_uninitialized_fill_n(OutputIter dest, size_t n, const Value& value, FALSE_TAG) {
        for(; n > 0; ++dest, --n)
            ZJ_construct(dest, value);
        return dest;
    }

    template <typename OutputIter, typename Value>
    inline OutputIter ZJ_uninitialized_fill_n(OutputIter dest, size_t n, const Value& value) {
        typedef typename traits<typename OutputIter::value_type>::IS_POD IS_POD;
        return __ZJ_uninitialized_fill_n(dest, n, value, IS_POD());
    }

    template <typename InputIter, typename OutputIter>
    inline OutputIter ZJ_copy(InputIter first, InputIter last, OutputIter dest, bool copy2left) {
        if(first == last) return dest;
        if (copy2left) { 
            for(; first != last; ++first, ++dest)
                *dest = *first;
        }
        else {
            dest += (last - first);
            --dest, --last;
            for(; last != first; --last, --dest)
                *dest = *last;
            *dest = *last;
        }
        return dest;
    }

    template <typename T>
    inline void ZJ_swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template<typename Iter>
    inline typename Iter::value_type get_value_type(Iter iter) {
        return typename Iter::value_type();
    }

    template<typename Iter>
    inline typename Iter::size_type get_size_type(Iter iter) {
        return typename Iter::size_type();
    }

    template<typename Iter>
    inline typename Iter::difference_type get_difference_type(Iter iter) {
        return typename Iter::difference_type();
    }
}


#endif

