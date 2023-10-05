
#ifndef _ZJ_UTILS_
#define _ZJ_UTILS_


namespace ZJ {

    template <typename T>
    inline void ZJ_construct(T* p, const T& value);
    template <typename T>
    inline void ZJ_construct(T* first, T* last, const T& value);
    template <typename T>
    inline void ZJ_destroy(T* p);
    template <typename T>
    inline void ZJ_destroy(T* first, T* last);
    template <typename T>
    inline T* ZJ_uninitialized_copy(T* first, T* last, T* dest);
    template <typename T>
    inline void ZJ_uninitialized_fill(T* first, T* last, const T& value);
    template <typename T>
    inline T* ZJ_uninitialized_fill_n(T* first, size_t n, const T& value);


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

    template <typename Iter, typename Value>
    inline void ZJ_construct(Iter p, const Value& value) {
        new(const_cast<Value*>(&*p)) Value(value);
    }

    template <typename Iter, typename Value>
    inline void ZJ_construct(Iter first, Iter last, const Value& value) {
        for(; first != last; ++first)
            ZJ_construct(first, value);
    }

    template <typename Iter>
    inline void __ZJ_destroy(Iter first, Iter last, TRUE_TAG) {}

    template <typename Iter>
    inline void __ZJ_destroy(Iter first, Iter last, FALSE_TAG) {
        for(; first != last; ++first)
            ZJ_destroy(first);
    }

    template <typename Iter>
    inline void __ZJ_destroy(Iter p, TRUE_TAG) {}

    template <typename Iter>
    inline void __ZJ_destroy(Iter p, FALSE_TAG) {
        typedef typename Iter::value_type value_type;
        p->~value_type();
    }

    template <typename Iter>
    inline void ZJ_destroy(Iter p) {
        typedef typename traits<typename Iter::value_type>::IS_POD IS_POD;
        //p->~value_type();
    }

    template <typename Iter>
    inline void ZJ_destroy(Iter first, Iter last) {
        typedef typename traits<typename Iter::value_type>::IS_POD IS_POD;
        __ZJ_destroy(first, last, IS_POD());
    }

    template <typename Iter>
    inline Iter __ZJ_uninitialized_copy(Iter first, Iter last, Iter dest, TRUE_TAG) {
        return copy(first, last, dest);
    }

    template <typename Iter>
    inline Iter __ZJ_uninitialized_copy(Iter first, Iter last, Iter dest, FALSE_TAG) {
        for(; first != last; ++first, ++dest)
            ZJ_construct(dest, *first);
        return dest;
    }

    template <typename Iter>
    inline Iter ZJ_uninitialized_copy(Iter first, Iter last, Iter dest) {
        typedef typename traits<typename Iter::value_type>::IS_POD IS_POD;
        return __ZJ_uninitialized_copy(first, last, dest, IS_POD());
    }

    template <typename Iter, typename Value>
    inline void __ZJ_uninitialized_fill(Iter first, Iter last, const Value& value, TRUE_TAG) {
        fill(first, last, value);
    }

    template <typename Iter, typename Value>
    inline void __ZJ_uninitialized_fill(Iter first, Iter last, const Value& value, FALSE_TAG) {
        for(; first != last; ++first)
            ZJ_construct(first, value);
    }

    template <typename Iter, typename Value>
    inline void ZJ_uninitialized_fill(Iter first, Iter last, const Value& value) {
        typedef typename traits<typename Iter::value_type>::IS_POD IS_POD;
        __ZJ_uninitialized_fill(first, last, value, IS_POD());
    }

    template<typename Iter, typename Value>
    inline Iter __ZJ_uninitialized_fill_n(Iter dest, size_t n, const Value& value, TRUE_TAG) {
        return fill_n(dest, n, value);
    }

    template<typename Iter, typename Value>
    inline Iter __ZJ_uninitialized_fill_n(Iter dest, size_t n, const Value& value, FALSE_TAG) {
        for(; n > 0; ++dest, --n)
            ZJ_construct(dest, value);
        return dest;
    }

    template <typename Iter, typename Value>
    inline Iter ZJ_uninitialized_fill_n(Iter dest, size_t n, const Value& value) {
        typedef typename traits<typename Iter::value_type>::IS_POD IS_POD;
        return __ZJ_uninitialized_fill_n(dest, n, value, IS_POD());
    }
}


#endif

