
#ifndef _ZJ_ALLOC_
#define _ZJ_ALLOC_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

using namespace std;

/*
namespace ZJ_0 { // for experiment purposes

    template<typename T>
    T* _allocate(ptrdiff_t size) {
        set_new_handler(0);
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if(tmp == 0){
            cerr << "out of memory" << endl;
            exit(1);
        }
        return tmp;
    }

    template<typename T>
    void _deallocate(T* buffer) {
        ::operator delete(buffer);
    }

    template<typename T1, typename T2>
    void _construct(T1* p, const T2& value) {
        new(p) T1(value);
    }

    template<typename T>
    void _destroy(T* ptr) {
        ptr->~T();
    }

    template<typename T>
    class allocator {
        public :
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            template<typename U>
            struct rebind {
                typedef allocator<U> other;
            };

            pointer allocate(size_type n, void* hint = 0) {
                //cout << "Allocate called!!!   " << n << " allocated!" << endl;
                return _allocate<value_type>((difference_type)n);
            }

            void deallocate(pointer p, size_type n) {
                _deallocate<value_type>(p);
            }

            void construct(pointer p, const value_type& value) {
                _construct<value_type, value_type>(p, value);
            }

            void destroy(pointer p) {
                _destroy<value_type>(p);
            }

            pointer address(reference x) {
                return (pointer) &x;
            }

            const_pointer const_address(const_reference x) {
                return (const_pointer) &x;
            }

            size_type max_size() const {
                return size_type(UINT_MAX / sizeof(value_type));
            }

    };

}
*/
namespace ZJ {

    // malloc
    class __allocator_malloc {

        private :
            static void (*oom_handler)();
        
        public: 
            static void* allocate(size_t n) {
                void* __res = malloc(n);
                if(__res == 0) {
                    __res = oom_malloc(n);
                }
                return __res;
            }

            static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
                void* __res = realloc(p, new_sz);
                if(__res == 0) __res = oom_realloc(p, new_sz);
                return __res;
            }

            static void deallocate(void* p, size_t n) {
                free(p);
            }

            void set_oom_handler(void (*_handler)()) {
                oom_handler = _handler;
            }

            static void* oom_malloc(size_t n) {
                if(0 == oom_handler) {
                    cerr << "out of memory and no hanlder given" << endl;
                    exit(1);
                }
                while(true) {
                    (*oom_handler)();
                    void* __res = malloc(n);
                    if(__res) return __res;                
                }
            }

            static void* oom_realloc(void* p, size_t n) {
                if(0 == oom_handler) {
                    cerr << "out of memory and no hanlder given" << endl;
                    exit(1);
                }
                while(true) {
                    (*oom_handler)();
                    void* __res = realloc(p, n);
                    if(__res) return __res;
                }
            }
    };

    // memory pool - to be implemented :(
    class __allocator_mp {};

    #define __ALLOCATOR__MALLOC__  // here which allocator is specify
    //#define __ALLOCATOR__MP__

    #ifdef __ALLOCATOR__MALLOC__
        typedef __allocator_malloc Alloc;
    #else
        typedef __allocator_mp Alloc;
    #endif


    // allocator wrapper
    template<typename T>
    class allocator {
        public :
            typedef T           value_type;
            typedef T*          pointer;
            typedef const T*    const_pointer;
            typedef T&          reference;
            typedef const T&    const_reference;
            typedef size_t      size_type;
            typedef ptrdiff_t   difference_type;

            template<typename U>
            struct rebind {
                typedef allocator<U> other;
            };

        public :
            static T* allocate(size_t n, void* hint = 0) {
                return (T*)Alloc::allocate(n * sizeof(T));
            }

            static void deallocate(T* p, size_t n) {
                Alloc::deallocate(p, n);
            }
    };

    void (*ZJ::__allocator_malloc::oom_handler)() = nullptr;

/*
    template <typename T>
    class traits {
        public : 
            typedef typename T::value_type value_type;
    }

    template <typename T>
    class traits<T*> {
        public : 
            typedef T value_type;
    }
*/
}

#endif


