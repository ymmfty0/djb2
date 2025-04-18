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
