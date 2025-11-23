# Lumen
Experimental pointer for those who enjoy living on the edge of memory safety.

*Just trying to make it look like std::unique_ptr… but without all the polish.*
*Handmade, experimental, occasionally smart, definitely honest.*

*I did this this morning in 2 hours for fun on this Saturday, November 22, 2025 :)*
```cpp
// Copyright (c) October 2025 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#include <iostream>

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


    template<typename T>
    class Pointer {
    private:
        T* _raw;

    public:
        //Pointer(T* ptr = nullptr) : _value(ptr) {}

        Pointer(void) { std::cout << "[Pointer] Creating a new smart pointer" << std::endl; }
        Pointer(T&& value) : _raw(new T(nex::move(value))) {}

        template<typename... Args>
        Pointer(Args... args) : _raw(new T(nex::forward<Args>(args)...)) {}

        ~Pointer(void) {
            std::cout << "[~Pointer] Destroying the smart pointer" << std::endl;
            delete _raw; _raw = nullptr;
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
    inline static Pointer<T> make_ptr(Ts&&... args) {
        std::cout << "[make_ptr] Making a new pointer of " << typeid(T).name() << std::endl;
        return Pointer<T>(nex::forward<Ts>(args)...);
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

    auto a = nex::make_ptr<Object>();
    auto b = nex::make_ptr<Object>(10, 10);

    auto c = nex::make_ptr<int>(10);

    b->test();



    //auto b = std::make_unique<Object>(42, "foo");

    


}
```
