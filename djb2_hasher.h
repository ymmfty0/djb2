
#ifndef DJB2_HASHER_H
#define DJB2_HASHER_H

#include <ranges>
#include <string_view>

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


#endif //DJB2_HASHER_H
