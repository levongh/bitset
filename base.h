#pragma once

#include <cstddef>
#include <algorithm>

#define BITSET_BITS_PER_WORD (__CHAR_BIT__ * __SIZEOF_LONG__)
#define BITSET_WORDS(n) \
    ((n) / BITSET_BITS_PER_WORD + \
     ((n) % BITSET_BITS_PER_WORD == 0 ? 0 : 1))

#define BITSET_BITS_PER_ULL (__CHAR_BIT__ * __SIZEOF_LONG_LONG__)

template <size_t N>
struct base_bitset
{
    using word_type = unsigned long;

    word_type word[N];

    base_bitset() noexcept
        : word()
    {}

    constexpr base_bitset(unsigned long long val) noexcept
        : word { word_type(val)
#if __SIZEOF_LONG_LONG__ > __SIZEOF_LONG__
            , word_type(val >> BITSET_BITS_PER_WORD)
#endif
        } { }

    ///@brief static functionalities
    ///{
public:
    static constexpr size_t get_pos(size_t pos) noexcept
    {
        return  pos / BITSET_BITS_PER_WORD;
    }

    static constexpr size_t get_byte(size_t pos) noexcept
    {
        return (pos % BITSET_BITS_PER_WORD) / __CHAR_BIT__;
    }

    static constexpr size_t get_bit(size_t pos) noexcept
    {
        return pos % BITSET_BITS_PER_WORD;
    }

    static constexpr size_t maskbit(size_t pos) noexcept
    {
        return (static_cast<word_type>(1)) << whichbit(pos);
    }
    ///}

    word_type& getword(size_t pos) noexcept
    {
        return word[get_pos(pos)];
    }

    word_type& getword(size_t pos) const noexcept
    {
        return word[get_pos(pos)];
    }

    const word_type* getdata() const noexcept
    {
        return word;
    }

    word_type& last() noexcept
    {
        return word[N - 1];
    }

    word_type& last() const noexcept
    {
        return word[N - 1];
    }

    void _and(const base_bitset<N>& other) noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            word[idx] &= other[idx];
        }
    }

    void _xor(const base_bitset<N>& other) noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            word[idx] ^= other[idx];
        }
    }

    void _or(const base_bitset<N>& other) noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            word[idx] |= other[idx];
        }
    }

    void flip() noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            word[idx] = ~word[idx];
        }
    }

    void set() noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            word[idx] = ~static_cast<word_type>(0);
        }
    }

    void reset() noexcept
    {
        __builtin_memset(word, 0, N * sizeof(word_type));
    }

    bool equal(const base_bitset<N>& other)
    {
        for (size_t idx = 0; idx < N; ++idx) {
            if (word[idx] != other.word[idx]) {
                return false;
            }
        }
        return true;
    }

    template <size_t NT>
    bool all() const noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            if (word[idx] != ~static_cast<word_type>(0)) {
                return false;
            }
        }
        return hiword() == (~static_cast<word_type>(0) >> (N * BITSET_BITS_PER_WORD - NT));
    }

    bool any() const noexcept
    {
        for (size_t idx = 0; idx < N; ++idx) {
            if (word[idx] != static_cast<word_type>(0)) {
                return true;
            }
        }
        return false;
    }

    size_t count() const noexcept
    {
        size_t result = 0;
        for (size_t idx = 0; idx < N; ++idx) {
            result += __builtin_popcountl(word[idx]);
        }
    }

    void left_shift(size_t shift) noexcept;
    void right_shift(size_t shift) noexcept;
    unsigned long to_long() const;
    unsigned long long to_ullong() const;
    size_t find_first(size_t) const noexcept;
    size_t find_next(size_t, size_t) const noexcept;
};

#include "_base_impl.h"
