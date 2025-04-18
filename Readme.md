# C++20 Compile-Time DJB2 Hasher

## Description

A small C++20 library/implementation for calculating the DJB2 hash both at compile-time (`consteval`) and runtime. It utilizes templates and C++20 concepts to work with various byte ranges (strings, vectors, arrays) and provides a convenient User-Defined Literal (UDL) for compile-time string hashing.

This project was created as a mini-project to learn and demonstrate modern C++ features.

## Features

* **Compile-Time Hashing:** Calculates DJB2 hash at compile time using `consteval` (for the constructor and UDL) and `constexpr` (for the `calculate` function).
* **Runtime Hashing:** Calculates hash at runtime (or `constexpr`) for dynamic data using the static `hash` method.
* **Range Support:** Templated functions handle various ranges (`std::string_view`, `std::vector`, `std::array`, C-style arrays) thanks to C++20 Ranges and Concepts.
* **User-Defined Literal (UDL):** Convenient `_djb2` suffix for compile-time hashing of string literals (e.g., `"MyString"_djb2`).
* **Modern C++:** Uses C++20 features (concepts, ranges, `constexpr`, `consteval`, `std::format` in examples).
* **Namespace:** All hasher code resides within the `hashing` namespace.
* **Hash Type:** Uses `std::size_t` (or the configured `hashing::hash_type`) for the hash result.

## Requirements

* A C++ compiler supporting the **C++20 standard** (GCC 10+, Clang 10+, MSVC 19.29+ or later).
* C++20 standard library enabled (for `<ranges>`, `<concepts>`, `<format>`, etc.).

## Building

**Example compilation (g++):**

```bash
g++ main.cpp -o hasher_demo -std=c++20 -Wall -Wextra
```
Note: Replace `main.cpp` with your source file name if different*

## How to Use

1.  **Include Header:** Add the necessary header file to your source code.
    ```c++
    #include "djb2_hasher.h" // Replace with your actual header file name
    ```
2.  **Namespace:** Bring the necessary components into scope. You can either import the whole namespace or specific parts:
    ```c++
    // Option A: Import everything (simpler, use with caution in headers)
    // using namespace hashing;

    // Option B: Import specific components (safer)
    using hashing::hash_type;
    using hashing::djb2;
    using hashing::operator""_djb2; // Needed to use the _djb2 literal suffix
    ```
3.  **Hashing:**
    * **Compile-Time String Hashing:** Use the `_djb2` User-Defined Literal directly on string literals. The result is a `constexpr hash_type`.
        ```c++
        constexpr hash_type my_hash = "My Secret Key"_djb2;
        ```
    * **Compile-Time Range Hashing:** Use the `static constexpr` `calculate` method for ranges known at compile time (like `constexpr std::array`). Note: In your code, `calculate` is `constexpr`, while the `djb2` constructor and UDL are `consteval`.
        ```c++
        constexpr std::array<char, 4> data = {'A','B','C','D'};
        // constexpr hash_type array_hash = hashing::djb2::calculate(data); // Direct calculate call example
        // OR create a constexpr object using the consteval constructor:
        constexpr hashing::djb2 data_hasher(std::string_view{"ABCD"});
        constexpr hash_type array_hash = data_hasher();

        ```
    * **Runtime (or `constexpr`) Range Hashing:** Use the `static constexpr` `hash` method for ranges whose values might only be known at runtime (like `std::vector`, `std::string_view` from runtime data, C-style arrays).
        ```c++
        std::vector<unsigned char> bytes = { 0xDE, 0xAD, 0xBE, 0xEF };
        hash_type vec_hash = hashing::djb2::hash(bytes);

        std::string_view sv = get_some_string_at_runtime();
        hash_type sv_hash = hashing::djb2::hash(sv);
        ```

**Examples:**

```c++
#include <format>
#include <iostream>
#include <vector>
#include "djb2_hasher.h"

using hashing::operator""_djb2;

int main( )
{


    auto hash_str1 = hashing::djb2::hash( "hello world" );
    std::cout << std::format("Hash string with 1st method : {} (dec), {:#x} (hex)\n", hash_str1 , hash_str1);

    auto hash_str2 = "hello world"_djb2;
    std::cout << std::format("Hash string with 2nd method : {} (dec), {:#x} (hex)\n", hash_str2, hash_str2);

    hashing::djb2 hasher{ "hello world" };
    std::cout << std::format("Hash string with 3nd method : {} (dec), {:#x} (hex)\n", hasher() , hasher());

    auto hash_vec = hashing::djb2::hash( std::vector<unsigned char>{'h','e','l','l','o'} );
    std::cout << std::format("Hash vec: {} (dec), {:#x} (hex)\n", hash_vec , hash_vec);

    auto hash_arr = hashing::djb2::hash( std::array<unsigned char,5>{'h','e','l','l','o'} );
    std::cout << std::format("Hash arr: {} (dec), {:#x} (hex)\n", hash_arr , hash_arr);

    unsigned char c_arr[] = {0xCA, 0xFE};
    auto hash_c_arr = hashing::djb2::hash(c_arr);
    std::cout << std::format("Hash c-stle arr: {} (dec), {:#x} (hex)\n", hash_c_arr , hash_c_arr);

    static_assert("Test"_djb2 == hashing::djb2::hash(std::string_view{"Test"}));

    return 0;
}

```
