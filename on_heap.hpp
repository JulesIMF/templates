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

#include <cassert>
#include <cstddef>
#include <array>
#include <stdexcept>
#include <type_traits>

//
// Defines
//

namespace jules::storage
{
    template<typename T, size_t InitialCapacity>
    class on_heap
    {
    protected:
        T* const data_;
        size_t capacity_;

    public:
        // 
        // Constructors / destructors
        // 

        on_heap() :
            data_(reinterpret_cast<T*>(new uint8_t[InitialCapacity * sizeof(T)])),
            capacity_(InitialCapacity)
        {
        }

        ~on_heap()
        {
            delete[] reinterpret_cast<uint8_t*>(data_);
        }

        //
        // Element access
        //

        void inline create(size_t index)
        {
            new (data_ + index) T();
        }

        template<typename... Args>
        void inline create(size_t index, Args&&... args)
        {
            new (data_ + index) T(std::forward<Args>(args)...);
        }

        void inline destroy(size_t index)
        {
            (data_ + index)->~T();
        }

        [[nodiscard]] inline T const& at_unchecked(size_t index) const noexcept
        {
            return data_[index];
        }

        [[nodiscard]] inline T& at_unchecked(size_t index) noexcept
        {
            return const_cast<T&>(static_cast<on_heap const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline T const* data() const noexcept
        {
            return data_;
        }

        [[nodiscard]] inline T* data() noexcept
        {
            return const_cast<T*>(static_cast<on_heap const*>(this)->data());
        }

        //
        // Capacity
        //

        [[nodiscard]] inline size_t capacity() const noexcept
        {
            return capacity_;
        }

        void inline realloc(size_t new_capacity, size_t elements_to_move = 0, size_t move_from = 0)
        {
            static_assert(std::is_move_constructible<T>::value);
            if (new_capacity == capacity_)
                return;

            if (new_capacity < move_from + elements_to_move)
                std::__throw_out_of_range_fmt("storage::on_heap::realloc: "
                    "new capacity == %zu is less than move_from + elements_to_move == %zu",
                    new_capacity, move_from + elements_to_move);

            T* new_data = reinterpret_cast<T*>(new uint8_t[new_capacity * sizeof(T)]);

            for (int i = move_from; i != move_from + elements_to_move; i++)
            {
                // only way to do this???
                // can`t just copy, cause fields may depend on this

                new (new_data + i) T(std::move(data_[i]));
                destroy(i);
            }

            delete[] reinterpret_cast<uint8_t*>(data_);
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