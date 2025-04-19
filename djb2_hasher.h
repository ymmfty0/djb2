#ifndef DBJ2_HASHER_H
#define DBJ2_HASHER_H

#include <string_view>
#include <type_traits>
#include <ranges>

namespace hashing
{
    using hash_type = std::size_t;

    template <typename T>
    concept character_like = std::is_integral_v<T> ||
                             std::is_same_v<T, char> ||
                             std::is_same_v<T, wchar_t> ||
                             std::is_same_v<T, unsigned char> ||
                             std::is_same_v<T, char8_t> ||
                             std::is_same_v<T, char16_t> ||
                             std::is_same_v<T, char32_t>;

    template <typename R>
    concept character_range = std::ranges::range<R> &&
                              character_like<std::ranges::range_value_t<R> >;

    struct djb2
    {
    public:
        consteval djb2( std::string_view djb ) noexcept : _hash( calculate( djb ) )
        {
        };

        consteval djb2( std::wstring_view wdjb ) noexcept : _hash( calculate( wdjb ) )
        {
        };

        constexpr hash_type operator()( ) const noexcept
        {
            return _hash;
        }

        constexpr operator hash_type( ) const noexcept
        {
            return _hash;
        }

        template <character_range Range>
        static constexpr hash_type hash( const Range &range, hash_type seed = 5381 ) noexcept
        {
            return calculate( range, seed );
        }

        static constexpr hash_type hash( const wchar_t *str, hash_type seed = 5381 ) noexcept
        {
            return calculate( std::wstring_view( str ), seed );
        }

        static constexpr hash_type hash( const char *str, hash_type seed = 5381 ) noexcept
        {
            return calculate( std::string_view( str ), seed );
        }

    private:
        hash_type _hash;

        template <character_range Range>
        static constexpr hash_type calculate( const Range &range, hash_type seed = 5381 ) noexcept
        {
            hash_type h = seed;
            for ( const auto &element: range )
            {
                h = ( ( h << 5 ) + h ) + element;
            }
            return h;
        }
    };

    consteval hash_type operator"" _djb2( const char *str, size_t len ) noexcept
    {
        return djb2( std::string_view{ str, len } );
    }

    consteval hash_type operator"" _djb2w( const wchar_t *str, size_t len ) noexcept
    {
        return djb2( std::wstring_view{ str, len } );
    }
}

using hashing::operator""_djb2;
using hashing::operator""_djb2w;

#endif //DBJ2_HASHER_H
