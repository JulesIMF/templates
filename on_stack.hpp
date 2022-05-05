/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    on_stack.hpp

Abstract:

    Stack storage 

Author / Creation date:

    JulesIMF / 05.04.22

Revision History:

--*/


//
// Includes / usings
//

#pragma once
#include <cstddef>
#include <array>
#include <stdexcept>
#include "on_heap.hpp"

//
// Defines
//

namespace jules::storage
{
    template<typename T, size_t MaxSize, class Allocator = jules::allocator::Empty<T>>
    class on_stack
    {
    protected:
        alignas(T) unsigned char buffer_[MaxSize * sizeof(T)];
        T* const data_;
    
    public:
        // 
        // Constructors / destructors
        // 

        on_stack() :
            data_(reinterpret_cast<T*>(buffer_))
        {
        }       
        
        //
        // Methods
        //

        void create(size_t index)
        {
            new (data_ + index) T();
        }

        template<typename... U>
        void create(size_t index, U&&... value)
        {
            new (data_ + index) T(std::forward<U>(value)...);
        }

        void destroy(size_t index)
        {
            (data_ + index)->~T();
        }

        [[nodiscard]] inline T const& at_unchecked(size_t index) const noexcept
        {
            return data_[index];
        }

        [[nodiscard]] inline T& at_unchecked(size_t index) noexcept
        {
            return const_cast<T&>(static_cast<on_stack const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline T const* data() const noexcept
        {
            return data_;
        }

        [[nodiscard]] inline T* data() noexcept
        {
            return const_cast<T*>(static_cast<on_stack const*>(this)->data());
        }
    };
}