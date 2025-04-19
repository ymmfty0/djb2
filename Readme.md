# C++20 Compile-Time DJB2 Hasher

## Description

A small C++20 library/implementation for calculating the DJB2 hash both at compile-time (`consteval`) and runtime. It utilizes templates and C++20 concepts to work with various **narrow (`char`) and wide (`wchar_t`)** character ranges (strings, vectors, arrays) and provides convenient User-Defined Literals (UDLs) for compile-time string hashing.

This project was created as a mini-project to learn and demonstrate modern C++ features.

## Features

* **Compile-Time Hashing:** Calculates DJB2 hash at compile time using `consteval` (for constructors and UDLs) and `constexpr` (for hashing functions).
* **Runtime Hashing:** Calculates hash at runtime (or `constexpr`) for dynamic data using the static `hash` method.
* **Range Support:** Templated functions handle various ranges (like `std::string_view`, **`std::wstring_view`**, `std::vector`, `std::array`, C-style arrays) thanks to C++20 Ranges and Concepts.
* **Wide Character Support:** Correctly handles ranges of `wchar_t` (and other character types like `char8_t`, `char16_t`, `char32_t`) by hashing their actual values.
* **User-Defined Literals (UDLs):**
    * Convenient `_djb2` suffix for compile-time hashing of narrow string literals (`"MyString"_djb2`).
    * Convenient `_djb2w` suffix for compile-time hashing of wide string literals (`L"MyWideString"_djb2w`).
* **Modern C++:** Uses C++20 features (concepts, ranges, `constexpr`, `consteval`, `std::format` in examples).
* **Namespace:** All hasher code resides within the `hashing` namespace.
* **Hash Type:** Uses `std::size_t` (or the configured `hashing::hash_type`) for the hash result.

## Requirements

* A C++ compiler supporting the **C++20 standard** (GCC 10+, Clang 10+, MSVC 19.29+ or later).
* C++20 standard library enabled (for `<ranges>`, `<concepts>`, `<format>`, `<string_view>`, etc.).

## Building

The project primarily consists of header files (if the hasher is header-only, like `djb2_hasher.h`) or a single `.cpp` file with `main` for demonstration.

**Example compilation (g++):**

```bash
g++ main.cpp -o hasher_demo -std=c++20 -Wall -Wextra
```

*(Note: Replace `main.cpp` with your source file name if different)*

## How to Use

1.  **Include Header:** Add the necessary header file to your source code.
    ```c++
    #include "djb2_hasher.h" // Replace with your actual header file name
    ```
2.  **Namespace:** Bring the necessary components into scope.
    ```c++
    using hashing::hash_type;
    using hashing::djb2;
    using hashing::operator""_djb2;  // UDL for narrow strings
    using hashing::operator""_djb2w; // UDL for wide strings
    ```
3.  **Hashing:**
    * **Compile-Time String Hashing:** Use the `_djb2` (narrow) or `_djb2w` (wide) UDLs directly on string literals.
        ```c++
        constexpr hash_type narrow_hash = "Narrow Key"_djb2;
        constexpr hash_type wide_hash   = L"Wide Key"_djb2w; // Note the L prefix
        ```
    * **Compile-Time Range Hashing:** Use the `static consteval calculate` method (or create a `constexpr djb2` object using the `consteval` constructor) for ranges known at compile time.
        ```c++
        constexpr std::array<char, 4>    narrow_data = {'N','A','R','W'};
        constexpr hash_type narrow_array_hash = hashing::djb2::calculate(narrow_data);

        constexpr std::array<wchar_t, 4> wide_data = {L'W',L'I',L'D',L'E'};
        constexpr hash_type wide_array_hash = hashing::djb2::calculate(wide_data);
        // Or via constructor:
        // constexpr hashing::djb2 wide_hasher_obj(std::wstring_view{L"WIDE"});
        // constexpr hash_type wide_array_hash = wide_hasher_obj();
        ```
    * **Runtime (or `constexpr`) Range Hashing:** Use the `static constexpr hash` method. It's templated and works for both narrow and wide character ranges.
        ```c++
        // Narrow
        std::string narrow_str = get_narrow_string();
        hash_type h1 = hashing::djb2::hash(std::string_view{narrow_str});

        // Wide
        std::wstring wide_str = get_wide_string();
        hash_type h2 = hashing::djb2::hash(std::wstring_view{wide_str});

        // Note: Passing raw literals directly like hash(L"...") might fail
        // due to pointer types not satisfying range concepts.
        // Always wrap literals/pointers in std::string_view or std::wstring_view
        // when calling the static hash/calculate methods directly.
        hash_type h_literal = hashing::djb2::hash(std::wstring_view{ L"Literal" });
        ```

**Examples:**

```c++
#include <iostream>
#include <string>        // For std::wstring
#include <string_view>
#include <vector>
#include <array>
#include <format>        // C++20
#include <cstddef>
#include "djb2_hasher.h" // Assumed header file name

// Make UDLs and type visible
using hashing::operator""_djb2;
using hashing::operator""_djb2w;
using hashing::hash_type;

int main() {
    // --- Narrow Examples ---
    constexpr hash_type ct_narrow = "Narrow Compile Time"_djb2;
    std::cout << std::format("CT Narrow Hash: {:#x}\n", ct_narrow);

    std::string rt_narrow = "Narrow Runtime";
    hash_type rt_narrow_hash = hashing::djb2::hash(std::string_view{rt_narrow});
    std::cout << std::format("RT Narrow Hash for \"{}\": {:#x}\n", rt_narrow, rt_narrow_hash);

    // --- Wide Examples ---
    constexpr hash_type ct_wide = L"Wide Compile Time"_djb2w; // Note L prefix and _djb2w
    std::cout << std::format("CT Wide Hash:   {:#x}\n", ct_wide);

    std::wstring rt_wide = L"Wide Runtime";
    hash_type rt_wide_hash = hashing::djb2::hash(std::wstring_view{rt_wide}); // Same hash function works!
    // Use wcout for wide strings if std::format struggles
    std::wcout << L"RT Wide Hash for \"" << rt_wide << L"\": " << std::hex << std::showbase << rt_wide_hash << std::endl;

    // --- Verification ---
    static_assert("Test"_djb2 == hashing::djb2::calculate(std::string_view{"Test"}));
    static_assert(L"TestW"_djb2w == hashing::djb2::calculate(std::wstring_view{L"TestW"}));

    return 0;
}
```