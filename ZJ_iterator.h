

#ifndef _ZJ_ITERATOR_
#define _ZJ_ITERATOR_

namespace ZJ {

    template    <typename Category,
                 typename T,
                 typename Distance = ptrdiff_t,
                 typename Pointer = T*,
                 typename Reference = T&>
    struct iterator_base {
        typedef T           value_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef Distance    difference_type;
        typedef Category    iterator_category;
    };

    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    /**
     * The following virtual classes will not be used.
     * They only serve as a template in the actual iterator design.
     * Plus, due to performance it is not a good practice to invlove
     * too many inheritance in iterator design.
    */

    template <typename T, typename Ptr, typename Ref, typename Diff>
    class input_iterator : public iterator_base<input_iterator_tag, T> {
        public : 
            virtual const T& operator* () = 0;
            virtual input_iterator& operator++ () = 0;
    };

    template <typename T>
    class output_iterator : public iterator_base<output_iterator_tag, T> {
        public : 
            virtual T& operator* () = 0;
            virtual output_iterator& operator++ () = 0;
    };

    template <typename T>
    class forward_iterator : public iterator_base<forward_iterator_tag, T> {
        public : 
            virtual T& operator* () = 0;
            virtual forward_iterator& operator++ () = 0;
    };

    template <typename T>
    class bidirectional_iterator : public iterator_base<bidirectional_iterator_tag, T> {
        public : 
            virtual T& operator* () = 0;
            virtual bidirectional_iterator& operator++ () = 0;
            virtual bidirectional_iterator& operator-- () = 0;
    };

    template <typename T>
    class random_access_iterator : public iterator_base<random_access_iterator_tag, T> {
        public : 
            virtual T& operator* () = 0;
            virtual random_access_iterator& operator++ () = 0;
            virtual random_access_iterator& operator-- () = 0;
            virtual random_access_iterator& operator+= (size_t n) = 0;
            virtual random_access_iterator& operator-= (size_t n) = 0;
            //virtual T& operator[] (size_t n) = 0;
    };

}

#endif
