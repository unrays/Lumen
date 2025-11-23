// Copyright (c) November 2025 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#include <iostream>

#define NEX_DEBUG
#define NEX_VERBOSE_LOG
#define NEX_PROFILE
#define ENABLE_FEATURE_X

#ifdef NEX_DEBUG
    #ifdef NEX_VERBOSE_LOG
        #define NEX_LOG(msg) std::cout << "[Verbose]" << msg << std::endl
    #else
        #define NEX_LOG(msg) std::cout << msg << std::endl
    #endif
#else
    #define NEX_LOG(msg)
#endif

namespace nex {
    template<typename T>
    struct is_lvalue_reference { static constexpr bool value = false; };

    template<typename T>
    struct is_lvalue_reference<T&> { static constexpr bool value = true; };

    template<typename T>
    struct is_rvalue_reference { static constexpr bool value = false; };

    template<typename T>
    struct is_rvalue_reference<T&&> { static constexpr bool value = true; };

    template<typename T>
    static inline T&& move(T& lvalue) { return static_cast<T&&>(lvalue); }

    template<typename T>
    static inline T&& forward(T& t) noexcept {
        return static_cast<T&&>(t);
    }

    //faire des structs a un moment donné

    template<typename T>
    class weak_ptr {
    public:

    protected:

    private:

    };

    template<typename T>
    class shared_ptr {
        unsigned int* _refCount = nullptr;

    private:
        T* _raw;


    public:
        shared_ptr(void) {
            NEX_LOG("[shared_ptr] Creating a new shared pointer");
            _raw = nullptr;
            _refCount = new unsigned int(1); //redondant
            std::cout << *_refCount << std::endl;
        }

        shared_ptr(T&& value)
            : _raw(new T(nex::move(value))),
            _refCount(new unsigned int(1)) {}

        template<typename U = T, typename... Args>
        explicit shared_ptr(Args&&... args)
            : _raw(new U(nex::forward<Args>(args)...)),
            _refCount(new unsigned int(1)) {
            std::cout << *_refCount << std::endl;
        }

        ~shared_ptr(void) {
            NEX_LOG("[~shared_ptr] Destroying the shared pointer");

            if (--(*_refCount) == 0) {
                NEX_LOG("[~shared_ptr] Refcount at 0, Destroying the shared value");
                delete _refCount;
                delete _raw;
            }
            _raw = nullptr;
        }

        shared_ptr<T>& operator=(const shared_ptr<T>& other) {
            std::cout << "Assignment operator called." << std::endl;

            if (this != &other) {
                if (_refCount && --(*_refCount) == 0) {
                    delete _raw;
                    delete _refCount;
                }

                _raw = other._raw;
                _refCount = other._refCount;

                if (_refCount) {
                    ++(*_refCount);
                }
            }
            return *this;
        }

        #ifndef __INTELLISENCE__
            T* operator->() { return _raw; }
            const T* operator->() const { return _raw; }
            T& operator*() { return *_raw; }
            T* get() const { return _raw; }
        #else
            T* get() const { return nullptr; }
        #endif

        shared_ptr(const shared_ptr& other) noexcept {
            NEX_LOG("[shared_ptr] Copying shared_ptr values to a new shared_ptr");

            _refCount = other._refCount;
            _raw = other._raw;

            if (_refCount)
                ++(*_refCount);
        }
    };


    template<typename T>
    class scoped_ptr {
    private:
        T* _raw;

    public:
        //Pointer(T* ptr = nullptr) : _value(ptr) {}

        scoped_ptr(void) {
            _raw = nullptr;
            NEX_LOG("[scoped_ptr] Creating a new scope pointer");
        }
        scoped_ptr(T&& value) : _raw(new T(nex::move(value))) {}

        scoped_ptr(scoped_ptr&& other) noexcept {
            NEX_LOG("[scoped_ptr] Transferring scoped_ptr property");
            _raw = other._raw;
            other._raw = nullptr;
        }

        template<typename... Args>
        scoped_ptr(Args&&... args) : _raw(new T(nex::forward<Args>(args)...)) {}

        ~scoped_ptr(void) {
            NEX_LOG("[~scoped_ptr] Destroying the smart pointer");
            delete _raw; _raw = nullptr;
        }

        scoped_ptr<T>& operator=(scoped_ptr<T>&& other) {
            _raw = other._raw;
            other._raw = nullptr;

            return *this;
        }

        #ifndef __INTELLISENCE__
            T* operator->() { return _raw; }
            const T* operator->() const { return _raw; }
            T& operator*() { return *_raw; }
            T* get() const { return _raw; }
        #else
            T* get() const { return nullptr; }
        #endif
    };

    template<typename T, typename... Ts>
    inline static scoped_ptr<T> make_scoped_ptr(Ts&&... args) {
        std::cout << "[make_scoped_ptr] Making a new scoped pointer of " << typeid(T).name() << std::endl;
        return scoped_ptr<T>(nex::forward<Ts>(args)...);
    }

    //illégalement redondant

    template<typename T, typename... Ts>
    inline static shared_ptr<T> make_shared_ptr(Ts&&... args) {
        std::cout << "[make_shared_ptr] Making a new shared pointer of " << typeid(T).name() << std::endl;
        return shared_ptr<T>(nex::forward<Ts>(args)...);
    }
}

class Object {
public:
    Object() { std::cout << "[Object] Constructing object" << std::endl; }
    Object(int a, int b) { std::cout << a * b << std::endl; }

    void test() {
        std::cout << "test" << std::endl;
    }
};

int main() {
    //Object* objPtr = new Object;
    //delete objPtr;

    //nex::Pointer<int> ptr(10);

    //nex::Pointer<Object> objPtr;

    //auto objPtr = Pointer<Object>;
    //make unique est comme une factory

    //auto a = nex::make_scoped_ptr<Object>();
    //auto b = nex::make_scoped_ptr<Object>(10, 10);


    /*
    nex::scoped_ptr<int> c = nex::make_scoped_ptr<int>(10);
    nex::scoped_ptr<int> d = nex::make_scoped_ptr<int>(50);
    //b->test();

    nex::scoped_ptr<int> e = nex::move(d);

    std::cout << *(e.get()) << std::endl;

    e = nex::move(c);

    std::cout << *(e.get()) << std::endl;

    if (c.get() == nullptr)
        std::cout << "C is equal to nullptr" << std::endl;

    */

    //nex::shared_ptr<int> f = nex::make_shared_ptr<int>(50);




    nex::shared_ptr<int> g = nex::make_shared_ptr<int>(20);

    nex::shared_ptr<int> h = nex::make_shared_ptr<int>(40);

    h = g;

    auto i = nex::make_shared_ptr<Object>();

    i->test();




    /*if (c.get() == nullptr)
        std::cout << "C rawptr is equal to nullptr" << std::endl;
    if (d.get() == nullptr)
        std::cout << "D rawptr is equal to nullptr" << std::endl;*/



    auto ptr = nex::make_ptr<int>(42);
    std::cout << *ptr << std::endl; // prints 42
}
