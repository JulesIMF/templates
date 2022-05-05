/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    on_heap.hpp

Abstract:



Author / Creation date:

    JulesIMF / 05.04.22

Revision History:

--*/


//
// Includes / usings
//

#pragma once
#include <cassert>
#include <cstddef>
#include <array>
#include <stdexcept>
#include <type_traits>

#include "allocators.hpp"

//
// Defines
//

namespace jules::storage
{
    template<typename T, std::size_t InitialCapacity, class Allocator = jules::allocator::Default<T, true>>
    class on_heap
    {
    public:
        static bool const is_raw   = Allocator::is_raw;
        using value_type           = T;
        using size_type            = std::size_t;
        using difference_type      = std::ptrdiff_t;

    protected:
        Allocator allocator_;
        value_type* data_;
        size_type capacity_;

    public:
        // 
        // Constructors / destructors
        // 

        on_heap() :
            data_(allocator_.allocate(InitialCapacity)),
            capacity_(InitialCapacity)
        {
        }

        ~on_heap()
        {
            allocator_.deallocate(data_, capacity_);
        }

        //
        // Element access
        //

        void inline create(difference_type index)
        {
            new (data_ + index) value_type();
        }

        template<typename... Args>
        void inline create(difference_type index, Args&&... args)
        {
            new (data_ + index) value_type(std::forward<Args>(args)...);
        }

        void inline destroy(difference_type index)
        {
            (data_ + index)->~value_type();
        }

        [[nodiscard]] inline value_type const& at_unchecked(difference_type index) const noexcept
        {
            return data_[index];
        }

        [[nodiscard]] inline value_type& at_unchecked(difference_type index) noexcept
        {
            return const_cast<value_type&>(static_cast<on_heap const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline value_type const* data() const noexcept
        {
            return data_;
        }

        [[nodiscard]] inline value_type* data() noexcept
        {
            return const_cast<value_type*>(static_cast<on_heap const*>(this)->data());
        }

        //
        // Capacity
        //

        [[nodiscard]] inline size_type capacity() const noexcept
        {
            return capacity_;
        }

        void inline realloc(size_type new_capacity, size_type elements_to_move = 0, difference_type move_from = 0)
        {
            static_assert(std::is_move_constructible<value_type>::value);
            if (new_capacity == capacity_)
                return;

            if (new_capacity < move_from + elements_to_move)
                std::__throw_out_of_range_fmt("storage::on_heap::realloc: "
                    "new capacity == %zu is less than move_from + elements_to_move == %zu",
                    new_capacity, move_from + elements_to_move);

            // value_type* new_data = reinterpret_cast<value_type*>(new uint8_t[new_capacity * sizeof(value_type)]);
            value_type* new_data = allocator_.allocate(new_capacity);

            for (int i = move_from; i != move_from + elements_to_move; i++)
            {
                // only way to do this???
                // can`t just copy, cause fields may depend on this

                if (is_raw)
                    new (new_data + i) value_type(std::move(data_[i]));
                    
                destroy(i);
            }

            allocator_.deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_capacity;
        }

        void inline swap(on_heap& other)
        {
            std::swap(data_, other.data_);
            std::swap(capacity_, other.capacity_);
        }
    };
}