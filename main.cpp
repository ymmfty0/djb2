#include <format>
#include <iostream>
#include <ranges>
#include <vector>

namespace hashing
{
    using hash_type = std::size_t;

    template <typename T>
    concept byte_like =
            std::is_integral_v<T> ||
            std::is_same_v<T, char> ||
            std::is_same_v<T, unsigned char> ||
            std::is_same_v<T, std::byte>;

    template <typename R>
    concept ranges_of_byte =
        std::ranges::range<R> ||
        byte_like<std::ranges::range_value_t<R>>;

    struct djb2
    {
    public:
        consteval djb2( std::string_view djb ) noexcept : _hash( calculate( djb ) )
        {
        };

        constexpr hash_type operator()() const noexcept
        {
            return _hash;
        }
        constexpr operator hash_type( ) const noexcept
        {
            return _hash;
        }
        template<ranges_of_byte Range>
        static constexpr hash_type hash( const Range& range, hash_type seed = 5381 ) noexcept
        {
            return calculate( range , seed );
        }

    private:
        hash_type _hash;

        template<ranges_of_byte Range>
        static constexpr hash_type calculate( const Range& range , hash_type seed = 5381) noexcept
        {
            hash_type h = seed;
            for ( const auto& element: range )
            {
                h = ( ( h << 5 ) + h ) + static_cast<unsigned char>( element );
            }
            return h;
        }
    };

    consteval hash_type operator"" _djb2( const char *str, size_t len ) noexcept
    {
        return djb2( std::string_view{ str, len } );
    }
}

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

    return 0;
}
