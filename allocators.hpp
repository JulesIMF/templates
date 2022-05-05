/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    allocators.hpp

Abstract:

    Default allocators, INcomaptible with std::allocators

Author / Creation date:

    JulesIMF / 04.04.22

Revision History:

--*/


//
// Includes / usings
//

#pragma once
#include <cstddef>

//
// Defines
//

namespace jules::allocator
{

    template<typename T, bool raw_memory = false>
    struct Empty
    {
        static bool const is_empty = true;
        static bool const is_raw   = raw_memory;
        using value_type           = T;
        using size_type            = std::size_t;
        using difference_type      = std::ptrdiff_t;

        // following functions are removed!
        // [[nodiscard]] value_type* allocate(size_type n);
        // void deallocate(value_type* ptr, size_type n);
    };

    template<typename T, bool raw_memory = false>
    class Default
    {
    protected:
        using raw_type             = uint8_t;

    public:
        static bool const is_empty = false;
        static bool const is_raw   = raw_memory;
        using value_type           = T;
        using size_type            = std::size_t;
        using difference_type      = std::ptrdiff_t;

        
        [[nodiscard]] inline value_type* allocate(size_type n)
        {
            if (raw_memory)
                return reinterpret_cast<value_type*>(new raw_type[n * sizeof(T)]);

            else
                return new T[n];
        }

        inline void deallocate(value_type* ptr, size_type n)
        {
            if (raw_memory)
                delete[] reinterpret_cast<raw_type*>(ptr);

            else
                delete[] ptr;
        }
    };
}