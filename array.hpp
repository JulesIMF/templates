/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    array.hpp

Abstract:

    

Author / Creation date:

    JulesIMF / 05.04.22

Revision History:

--*/


//
// Includes / usings
//

#include <cstddef>
#include "on_stack.hpp"
#include "on_heap.hpp"

//
// Defines
//


namespace jules
{
    // example: array<int, 5, storage::on_stack>
    // template<typename T, size_t MaxSize/*, template<typename, size_t> class Storage*/>
    template<typename T, size_t MaxSize, template<typename, size_t, class> class Storage>
    class array
    {
    protected:
        // #define Storage  storage::on_stack
        Storage<T, MaxSize, jules::allocator::Default<T, true>> storage_;
        size_t size_ = 0;

        inline void check_size_(size_t size, char const* fnc) const
        {
            if (size > MaxSize)
                std::__throw_out_of_range_fmt("%s: "
                    "size_t == %zu exceeds max_size == %zu",
                    fnc, size_, MaxSize);
        }

        inline void check_index_(size_t index, char const* fnc) const
        {
            if (index >= size_)
                std::__throw_out_of_range_fmt("%s: "
                    "index == %zu out of range within size == %zu",
                    fnc, index, size_);
        }

    public:
        explicit array(size_t size = 0) :
            size_(size)
        {
            check_size_(size_, "array::array(size_t)");
            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i);
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }
        }

        template<typename... Args>
        explicit array(size_t size, Args&&... args) :
            size_(size)
        {
            check_size_(size_, "array::array(size_t, U)");
            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i, std::forward<Args>(args)...);
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }
        }

        // not explicit!
        array(std::initializer_list<T> list) :
            size_(list.size())
        {
            check_size_(size_, "array::array(std::initializer_list<T>)");

            size_t i = 0;
            try
            {
                for (auto it = list.begin(); it != list.end(); it++, i++)
                    storage_.create(i, std::move(*it));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }
        }

        array(array const& origin) :
            size_(origin.size_)
        {
            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i, origin.at_unchecked(i));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }
        }

        array(array&& origin) :
            size_(origin.size_)
        {
            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i, std::move(origin.at_unchecked(i)));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }
        }

        // void clear() noexcept;
        
        ~array() noexcept
        {
            clear();
        }

        array& operator=(array const& origin)
        {
            clear();
            size_ = origin.size_;

            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i, origin.at_unchecked(i));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }

            return *this;
        }
        
        array& operator=(array&& origin)
        {
            clear();
            size_ = origin.size_;

            size_t i = 0;
            try
            {
                for (; i != size_; i++)
                    storage_.create(i, std::move(origin.at_unchecked(i)));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }

            return *this;
        }

        array& operator=(std::initializer_list<T> list)
        {
            clear();
            size_ = list.size();
            check_size_(size_, "array::operator=(std::initializer_list<T>)");

            size_t i = 0;
            try
            {
                for (auto it = list.begin(); it != list.end(); it++, i++)
                    storage_.create(i, std::move(*it));
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                throw; // up
            }

            return *this;
        }

        //
        // Element access
        //

        [[nodiscard]] inline T const& at_unchecked(size_t index) const noexcept
        {
            return storage_.at_unchecked(index);
        }

        [[nodiscard]] inline T& at_unchecked(size_t index) noexcept
        {
            return const_cast<T&>(static_cast<array const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline T const& operator[](size_t index) const
        {
            check_index_(index, "array::operator[](size_t)");
            return storage_.at_unchecked(index);
        }

        [[nodiscard]] inline T& operator[](size_t index)
        {
            return const_cast<T&>(static_cast<array const*>(this)->operator[](index));
        }

        [[nodiscard]] inline T const& front() const
        {
            return operator[](0);
        }

        [[nodiscard]] inline T& front()
        {
            return const_cast<T&>(static_cast<array const*>(this)->front());
        }

        [[nodiscard]] inline T const& back() const
        {
            return operator[](size_ - 1);
        }

        [[nodiscard]] inline T& back()
        {
            return const_cast<T&>(static_cast<array const*>(this)->back());
        }

        [[nodiscard]] inline T const* data() const noexcept
        {
            return storage_.data();
        }

        [[nodiscard]] inline T* data() noexcept
        {
            return const_cast<T*>(static_cast<array const*>(this)->data());
        }

        //
        // Iterators
        //


        //
        // Capacity
        //

        [[nodiscard]] inline bool empty() const noexcept
        {
            return size_ == 0;
        }

        [[nodiscard]] inline size_t size() const noexcept
        {
            return size_;
        }

        [[nodiscard]] inline size_t max_size() const noexcept
        {
            return MaxSize;
        }

        //
        // Modifiers
        //
        void resize(size_t new_size)
        {
            if (new_size == size_)
                return;

            check_size_(new_size, "array::resize()");

            if (new_size > size_)
            {
                size_t i = size_;
                try
                {
                    for (; i != new_size; i++)
                        storage_.create(i);
                }
                catch (...)
                {
                    for (; i > size_;)
                        storage_.destroy(--i);

                    throw; // up
                }

                size_ = new_size;
            }

            else
            {
                for (size_t i = size_; i != new_size;)
                    storage_.destroy(--i);

                size_ = new_size;
            }
        }

        void clear() noexcept
        {
            size_t i = size_;
            while (i != 0)
            {
                try
                {
                    for (; i > 0;)
                        storage_.destroy(--i);
                }
                catch (...) {}
            }

            size_ = 0;
        }
    };

    template<size_t MaxSize, template<typename, size_t, class> class Storage>
    class array<bool, MaxSize, Storage>
    {
    protected:
        struct __bool_ref
        {
        friend class array<bool, MaxSize, Storage>;
        __bool_ref(__bool_ref const&) = default; // not explicit

        __bool_ref& operator=(bool value) noexcept
        {
            octet_ &= ~(1 << bit_);
            octet_ |= (1 << bit_) * value;
            return *this;
        }

        __bool_ref& operator=(__bool_ref const& other) noexcept
        {
            *this = bool(other);
            return *this;
        }

        operator bool() const noexcept
        {
            return (octet_ >> bit_) & 0x1;
        }

        
        private:
        __bool_ref(uint8_t& octet, size_t bit) noexcept :
            octet_(octet), bit_(bit)
        {
        }

        static __bool_ref create_(uint8_t* data, size_t index)
        {
            return __bool_ref(data[index >> 3], index & 7ull);
        }

        private:
            uint8_t& octet_;
            size_t const bit_;
        };

        struct __bool_const_ref
        {
        friend class array<bool, MaxSize, Storage>;
        __bool_const_ref(__bool_const_ref const&) = default; // not explicit

        operator bool() const noexcept
        {
            return (octet_ >> bit_) & 0x1;
        }

        
        private:
        __bool_const_ref(uint8_t const& octet, size_t bit) noexcept :
            octet_(octet), bit_(bit)
        {
        }

        static __bool_const_ref create_(uint8_t const* data, size_t index)
        {
            return __bool_const_ref(data[index >> 3], index & 7ull);
        }

        private:
            uint8_t const& octet_;
            size_t const bit_;
        };
    
    protected:
        static size_t constexpr Capacity = (MaxSize + 7) / 8;
        Storage<uint8_t, Capacity, jules::allocator::Default<uint8_t, true>> storage_;
        size_t size_ = 0;

        inline void check_size_(size_t size, char const* fnc) const
        {
            if (size > MaxSize)
                std::__throw_out_of_range_fmt("%s: "
                    "size_t == %zu exceeds max_size == %zu",
                    fnc, size_, MaxSize);
        }

        inline void check_index_(size_t index, char const* fnc) const
        {
            if (index >= size_)
                std::__throw_out_of_range_fmt("%s: "
                    "index == %zu out of range within size == %zu",
                    fnc, index, size_);
        }

    public:
        explicit array(size_t size = 0) noexcept :
            size_(size)
        {
            check_size_(size_, "array::array(size_t)");
            for (size_t i = 0; i < size_; i++)
                storage_.create(i / 8, 0);
        }

        template<typename U>
        explicit array(size_t size, U&& value) :
            size_(size)
        {
            check_size_(size_, "array::array(size_t, U)");
            bool fill = !!(value);
            for (size_t i = 0; i != size_; i++)
                at_unchecked(i) = fill;
        }

        // not explicit!
        array(std::initializer_list<bool> list) :
            size_(list.size())
        {
            check_size_(size_, "array::array(std::initializer_list<T>)");

            size_t i = 0;
            for (auto it = list.begin(); it != list.end(); ++it, i++)
                at_unchecked(i) = !!(*it);
        }

        array(array const& origin) noexcept :
            size_(origin.size_)
        {
            size_t i = 0;
            for (; i != (size_ + 7) / 8; i++)
                storage_.create(i, origin.storage_.at_unchecked(i));
        }

        array(array&& origin) noexcept :
            size_(origin.size_)
        {
            size_t i = 0;
            for (; i != (size_ + 7) / 8; i++)
                storage_.create(i, origin.storage_.at_unchecked(i));
        }

        // void clear() noexcept;

        ~array() noexcept
        {
            
        }

        array& operator=(array const& origin)
        {
            size_t i = 0;
            for (; i != (size_ + 7) / 8; i++)
                storage_.create(i, origin.storage_.at_unchecked(i));

            return *this;
        }

        array& operator=(array&& origin)
        {
            size_t i = 0;
            for (; i != (size_ + 7) / 8; i++)
                storage_.create(i, origin.storage_.at_unchecked(i));

            return *this;
        }

        array& operator=(std::initializer_list<bool> const& list)
        {
            size_ = list.size();
            check_size_(size_, "array::operator=(std::initializer_list<T>)");

            size_t i = 0;
            for (auto it = list.begin(); it != list.end(); it++, i++)
                at_unchecked(i) = *it;

            return *this;
        }

        //
        // Element access
        //

        [[nodiscard]] inline __bool_const_ref at_unchecked(size_t index) const noexcept
        {
            return __bool_const_ref::create_(storage_.data(), index);
        }

        [[nodiscard]] inline __bool_ref at_unchecked(size_t index) noexcept
        {
            return __bool_ref::create_(storage_.data(), index);
        }

        [[nodiscard]] inline __bool_const_ref operator[](size_t index) const
        {
            check_index_(index, "array::operator[](size_t)");
            return at_unchecked(index);
        }

        [[nodiscard]] inline __bool_ref operator[](size_t index)
        {
            check_index_(index, "array::operator[](size_t)");
            return at_unchecked(index);
        }

        [[nodiscard]] inline __bool_const_ref front() const
        {
            return operator[](0);
        }

        [[nodiscard]] inline __bool_ref front()
        {
            return operator[](0);
        }

        [[nodiscard]] inline __bool_const_ref back() const
        {
            return operator[](size_ - 1);
        }

        [[nodiscard]] inline __bool_ref back()
        {
            return operator[](size_ - 1);
        }

        [[nodiscard]] inline uint8_t const* data(size_t index) const noexcept
        {
            return storage_.data();
        }

        [[nodiscard]] inline uint8_t* data(size_t index) noexcept
        {
            return const_cast<array*>(static_cast<array const*>(this)->data());
        }

        //
        // Iterators
        //


        //
        // Capacity
        //

        [[nodiscard]] inline bool empty() const noexcept
        {
            return size_ == 0;
        }

        [[nodiscard]] inline size_t size() const noexcept
        {
            return size_;
        }

        [[nodiscard]] inline size_t max_size() const noexcept
        {
            return MaxSize;
        }

        //
        // Modifiers
        //
        void resize(size_t new_size) noexcept
        {
            if (new_size == size_)
                return;

            check_size_(new_size, "array::resize()");

            if (new_size > size_)
            {
                for (size_t i = size_; i != new_size; i++)
                    at_unchecked(i) = false;

                size_ = new_size;
            }

            else
                size_ = new_size;
        }

        void clear() noexcept
        {
            size_ = 0;
        }
    };
}
