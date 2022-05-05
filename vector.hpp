/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    vector.hpp

Abstract:

    

Author / Creation date:

    JulesIMF / 05.04.22

Revision History:

--*/


//
// Includes / usings
//

#pragma once
#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <limits>
#include "allocators.hpp"
#include "on_stack.hpp"
#include "on_heap.hpp"

//
// Defines
//


namespace jules
{
    // example: vector<int, 5, storage::on_stack>
    // template<typename T, size_t MaxSize/*, template<typename, size_t> class Storage*/>
    template<typename T, class Allocator = jules::allocator::Default<T, true>, 
                         template<typename, size_t, class> class Storage = jules::storage::on_heap>
    class vector
    {
        static_assert(Allocator::is_raw, "Allocator for vector must be raw!");
    public:
        using value_type           = T;
        using allocator_type       = Allocator;
        using size_type            = std::size_t;
        using difference_type      = std::ptrdiff_t;
        static size_type const initial_capacity
                                   = 0;
        using storage_type         = Storage<value_type, initial_capacity, allocator_type>;
        using reference            = reference;
        using const_reference      = const_reference;
        using pointer              = T*;
        using const_pointer        = T const*;

        class __vector_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using pointer              = pointer;
            using reference            = reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_iterator(vector& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }
        
        public:
            __vector_iterator(__vector_iterator const&) = default;
            __vector_iterator& operator=(__vector_iterator const&) = default;

            __vector_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[index_];
            }

            pointer operator->()
            {
                return &(*vector_)[index_];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[index_ + index];
            }

            bool operator==(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_const_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using pointer              = const_pointer;
            using reference            = const_reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector const* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_const_iterator(vector const& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }
        
        public:
            __vector_const_iterator(__vector_iterator const& that) :
                vector_(that.vector_),
                index_(that.index_)
            {
            }

            __vector_const_iterator(__vector_const_iterator const&) = default;
            __vector_const_iterator& operator=(__vector_const_iterator const&) = default;

            __vector_const_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_const_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_const_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_const_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_const_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_const_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_const_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_const_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_const_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[index_];
            }

            pointer operator->()
            {
                return &(*vector_)[index_];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[index_ + index];
            }

            bool operator==(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_const_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_const_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_const_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_reverse_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using pointer              = pointer;
            using reference            = reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_reverse_iterator(vector& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }

            difference_type translate_(difference_type index)
            {
                return static_cast<difference_type>(vector_->size()) - index_ - 1;
            }
        
        public:
            __vector_reverse_iterator(__vector_reverse_iterator const&) = default;
            __vector_reverse_iterator& operator=(__vector_reverse_iterator const&) = default;

            __vector_reverse_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_reverse_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_reverse_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_reverse_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_reverse_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_reverse_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_reverse_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_reverse_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_reverse_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[translate_(index_)];
            }

            pointer operator->()
            {
                return &(*vector_)[translate_(index_)];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[translate_(index_ + index)];
            }

            bool operator==(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_reverse_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_reverse_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_reverse_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_reverse_const_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using pointer              = const_pointer;
            using reference            = const_reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector& vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_reverse_const_iterator(vector& vec, difference_type index) :
                vector_(vec),
                index_(index)
            {
            }

            difference_type translate_(difference_type index)
            {
                return static_cast<difference_type>(vector_->size()) - index_ - 1;
            }
        
        public:
            __vector_reverse_const_iterator(__vector_reverse_iterator const& that) :
                vector_(that.vector_),
                index_(that.index_)
            {
            }
            __vector_reverse_const_iterator(__vector_reverse_const_iterator const&) = default;
            __vector_reverse_const_iterator& operator=(__vector_reverse_const_iterator const&) = default;

            __vector_reverse_const_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_reverse_const_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_reverse_const_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_reverse_const_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_reverse_const_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_reverse_const_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_reverse_const_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_reverse_const_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_reverse_const_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[translate_(index_)];
            }

            pointer operator->()
            {
                return &(*vector_)[translate_(index_)];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[translate_(index_ + index)];
            }

            bool operator==(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_reverse_const_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_reverse_const_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_reverse_const_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        using iterator             = __vector_iterator;
        using const_iterator       = __vector_const_iterator;
        using reverse_iterator     = __vector_reverse_iterator;
        using const_reverse_iterator
                                   = __vector_reverse_const_iterator;

    protected:
        storage_type storage_;
        size_type size_ = 0;

        inline void check_index_(difference_type index, char const* fnc) const
        {
            if (index >= static_cast<difference_type>(size_))
                std::__throw_out_of_range_fmt("%s: "
                    "index == %zu out of range within size == %zu",
                    fnc, index, size_);
        }

        template<typename It>
        inline void check_iterator_(It const& it, char const* fnc)
        {
            if (this != it.vector_)
                std::__throw_out_of_range_fmt("%s: "
                    "wrong iterator, this (%p) != it.vector_ (%p)",
                    fnc, this, it.vector_);
            
            if (it.index_ != size_)
                check_index_(it.index_, fnc);
        }

        inline bool is_created_(difference_type i)
        {
            return i < size_;
        }

        inline void realloc_if_needed_(difference_type size_diff = 1)
        {    
            auto current_capacity = capacity();
            auto new_size = size_ + size_diff;
            if (size_diff < 0)
            {
                assert(-size_diff < size_);
            }

            assert(size_ <= current_capacity);

            if (new_size * 4 <= current_capacity && current_capacity > 0)
            {
                storage_.realloc((current_capacity + 1) / 2, size_);
                return;
            }

            auto new_capacity = current_capacity;
            while (new_size >= new_capacity)
                new_capacity = std::max(new_capacity * 2, static_cast<size_type>(1));
            
            storage_.realloc(new_capacity, size_);
        }

        inline void create_default_(difference_type start, size_t count)
        {
            for (difference_type i = 0; i != static_cast<difference_type>(count); i++)
                storage_.create(i);
        }

        inline void move_tail_(difference_type start, difference_type shift)
        {
            if (shift == 0)
                return;

            if (shift < 0)
                for (difference_type i = start; i != size_; i++)
                    at_unchecked(i + shift) = std::move(at_unchecked(i));
            
            else
                for (difference_type i = size_ - 1; i >= start; i--)
                {
                    if (!is_created_(i + shift))
                        storage_.create(i + shift, std::move(at_unchecked(i)));

                    else
                        at_unchecked(i + shift) = std::move(at_unchecked(i));
                }
        }

    public:
        // inline void reserve(size_type new_capacity)
        explicit vector(size_type size = 0)
        {
            reserve(size);
            difference_type i = 0;
            try
            {
                for (; i != size; i++)
                    storage_.create(i);
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);
                
                size_ = i;
                throw; // up
            }
        }

        template<typename... Args>
        explicit vector(size_type size, Args&&... args)
        {
            reserve(size);
            difference_type i = 0;
            try
            {
                for (; i != size; i++)
                    storage_.create(i, std::forward<Args>(args)...);
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                size_ = i;
                throw; // up
            }
        }

        // not explicit!
        vector(std::initializer_list<value_type> list)
        {
            auto size = list.size();
            reserve(size);

            difference_type i = 0;
            try
            {
                for (auto it = list.begin(); it != list.end(); ++it, i++)
                    storage_.create(i, std::move(*it));
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);
                
                size_ = i;
                throw; // up
            }
        }

        vector(vector const& origin)
        {
            auto size = origin.size_;
            reserve(size);

            difference_type i = 0;
            try
            {
                for (; i != size; i++)
                    storage_.create(i, origin.at_unchecked(i));
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);
                
                size_ = i;
                throw; // up
            }
        }

        vector(vector&& origin)
        {
            auto size = origin.size_;
            reserve(size);

            difference_type i = 0;
            try
            {
                for (; i != size; i++)
                    storage_.create(i, std::move(origin.at_unchecked(i)));
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                size_ = i;
                throw; // up
            }
        }

        // void clear() noexcept;
        
        ~vector() noexcept
        {
            clear();
        }

        vector& operator=(vector const& origin)
        {
            clear();
            auto size = origin.size_;

            difference_type i = 0;
            try
            {
                for (; i != size; i++)
                    storage_.create(i, origin.at_unchecked(i));
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);

                size_ = i;
                throw; // up
            }

            return *this;
        }
        
        vector& operator=(vector&& origin)
        {
            clear();

            storage_.swap(origin.storage_);
            size_ = origin.size_;

            return *this;
        }

        vector& operator=(std::initializer_list<value_type> list)
        {
            clear();
            auto size = list.size();
            reserve(size);

            difference_type i = 0;
            try
            {
                for (auto it = list.begin(); it != list.end(); it++, i++)
                    storage_.create(i, std::move(*it));
                
                size_ = i;
            }
            catch (...)
            {
                for (; i > 0;)
                    storage_.destroy(--i);
                
                size_ = i;
                throw; // up
            }

            return *this;
        }

        // TODO: assign

        [[nodiscard]] inline allocator_type get_allocator() const
        {
            return allocator_type();
        }

        //
        // Element access
        //

        [[nodiscard]] inline const_reference at_unchecked(difference_type index) const noexcept
        {
            return storage_.at_unchecked(index);
        }

        [[nodiscard]] inline reference at_unchecked(difference_type index) noexcept
        {
            return const_cast<reference>(static_cast<vector const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline const_reference operator[](difference_type index) const
        {
            check_index_(index, "vector::operator[](difference_type)");
            return storage_.at_unchecked(index);
        }

        [[nodiscard]] inline reference operator[](difference_type index)
        {
            return const_cast<reference>(static_cast<vector const*>(this)->operator[](index));
        }

        [[nodiscard]] inline const_reference front() const
        {
            return operator[](0);
        }

        [[nodiscard]] inline reference front()
        {
            return const_cast<reference>(static_cast<vector const*>(this)->front());
        }

        [[nodiscard]] inline const_reference back() const
        {
            return operator[](size_ - 1);
        }

        [[nodiscard]] inline reference back()
        {
            return const_cast<reference>(static_cast<vector const*>(this)->back());
        }

        [[nodiscard]] inline const_pointer data() const noexcept
        {
            return storage_.data();
        }

        [[nodiscard]] inline pointer data() noexcept
        {
            return const_cast<pointer>(static_cast<vector const*>(this)->data());
        }

        //
        // Iterators
        //

        iterator begin()
        {
            return iterator(*this, 0);
        }

        const_iterator cbegin() const
        {
            return const_iterator(*this, 0);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(*this, 0);
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(*this, 0);
        }

        const_reverse_iterator rbegin() const
        {
            return crbegin();
        }

        //--

        iterator end()
        {
            return iterator(*this, size_);
        }

        const_iterator cend() const
        {
            return const_iterator(*this, size_);
        }

        const_iterator end() const
        {
            return cend();
        }

        reverse_iterator rend()
        {
            return reverse_iterator(*this, size_);
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(*this, size_);
        }

        const_reverse_iterator rend() const
        {
            return crend();
        }

        //
        // Capacity
        //

        [[nodiscard]] inline bool empty() const noexcept
        {
            return size_ == 0;
        }

        [[nodiscard]] inline size_type size() const noexcept
        {
            return size_;
        }

        [[nodiscard]] inline size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max();
        }

        inline void reserve(size_type new_capacity)
        {
            if (new_capacity > storage_.capacity())
                storage_.realloc(new_capacity, size_);
        }

        [[nodiscard]] inline size_type capacity() const noexcept
        {
            return storage_.capacity();
        }

        inline void shrink_to_fit()
        {
            storage_.realloc(size_, size_);
        }

        //
        // Modifiers
        //

        inline void clear() noexcept
        {
            difference_type i = size_;
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

        inline void resize(size_type new_size)
        {
            if (new_size == size_)
                return;
            
            reserve(new_size);

            if (new_size > size_)
            {
                size_type i = size_;
                try
                {
                    for (; i != new_size; i++)
                        storage_.create(i);
                    
                    size_ = i;
                }
                catch (...)
                {
                    for (; i > size_;)
                        storage_.destroy(--i);

                    size_ = i;
                    throw; // up
                }
            }

            else
            {
                for (size_type i = size_; i != new_size;)
                    storage_.destroy(--i);

                size_ = new_size;
            }
        }

        // no nodiscard!
        template<typename... Args>
        inline reference emplace_back(Args&&... args)
        {
            realloc_if_needed_();
            storage_.create(static_cast<difference_type>(size_++), std::forward<Args>(args)...);
            return back();
        }

        inline void push_back(T const& value)
        {
            emplace_back(value);
        }

        inline void push_back(T&& value)
        {
            emplace_back(std::move(value));
        }

        inline void pop_back()
        {
            check_index_(0, "vector::pop_back()");
            storage_.destroy(static_cast<difference_type>(size_ - 1));
            realloc_if_needed_(-1);
            size_--;
        }

        inline iterator insert(const_iterator position, T const& value)
        {
            check_iterator_(position, "vector::insert(const_iterator, T const&)");
            realloc_if_needed_();
            auto index = position - begin();
            move_tail_(index, 1);
            if (!is_created_(index))
                storage_.create(index, value);
            else
                at_unchecked(index) = value;
            size_++;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, T&& value)
        {
            check_iterator_(position, "vector::insert(const_iterator, T&&)");
            realloc_if_needed_();
            auto index = position - begin();
            move_tail_(index, 1);

            if (!is_created_(index))
                storage_.create(index, std::move(value));
            else
                at_unchecked(index) = std::move(value);

            size_++;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, size_type count, T const& value)
        {
            if (count == 0)
                return iterator(*this, position.index_);

            check_iterator_(position, "vector::insert(const_iterator, size_type, T const&)");
            realloc_if_needed_(count);

            auto index = position - begin();
            move_tail_(index, static_cast<difference_type>(count));
            for (auto i = index; i != index + count; i++)
            {
                if (!is_created_(i))
                    storage_.create(i, value);

                else
                    at_unchecked(i) = value;
            }

            size_ += count;
            return begin() + index;
        }

        template<typename It>
        inline iterator insert(const_iterator position, It first, It last)
        {
            if (!(first < last))
                return iterator(*this, position.index_);

            check_iterator_(position, "vector::insert(const_iterator, It, It)");
            auto count = last - first;
            realloc_if_needed_(count);
            auto index = position - begin();            
            move_tail_(index, static_cast<difference_type>(count));
            for (auto i = index; 
                 i != index + count && first < last; 
                 i++, ++first)
            {
                if (!is_created_(i))
                    storage_.create(i, *first);

                else
                    at_unchecked(i) = *first;
            }

            size_ += count;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, std::initializer_list<T> list)
        {
            check_iterator_(position, "vector::insert(const_iterator, std::initializer_list<T>)");
            auto count = list.size();
            realloc_if_needed_(static_cast<difference_type>(count));

            auto index = position - begin();
            move_tail_(index, static_cast<difference_type>(count));
            auto it = list.begin();
            for (auto i = index; i != index + count; i++, ++it)
            {
                if (!is_created_(i))
                    storage_.create(i, std::move(*it));

                else
                    at_unchecked(i) = *it;
            }

            size_ += count;
            return begin() + index;
        }

        inline iterator erase(const_iterator first, const_iterator last)
        {
            check_iterator_(first, "vector::erase(const_iterator, const_iterator): first");
            check_iterator_(last, "vector::erase(const_iterator, const_iterator): second");

            if (!(first < last))
                return iterator(*this, last.index_);

            auto count = last - first;
            auto index = first.index_;

            move_tail_(last.index_, -count);
            for (auto i = size_ - count; i != size_; i++)
                storage_.destroy(i);
            
            size_ -= count;
            return begin() + index;
        }

        inline iterator erase(const_iterator position)
        {
            return erase(position, position + 1);
        }
    };

    template<class Allocator, template<typename, size_t, class> class Storage>
    class vector<bool, Allocator, Storage>
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

        static_assert(Allocator::is_raw, "Allocator for vector must be raw!");
    public:
        using value_type           = bool;
        using real_type            = uint8_t;
        using allocator_type       = Allocator;
        using size_type            = std::size_t;
        using difference_type      = std::ptrdiff_t;
        static size_type const initial_capacity
                                   = 0;
        using storage_type         = Storage<real_type, initial_capacity, allocator_type>;
        using reference            = __bool_ref;
        using const_reference      = __bool_const_ref;
        using pointer              = real_type*;
        using const_pointer        = real_type const*;

        class __vector_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using reference            = reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_iterator(vector& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }
        
        public:
            __vector_iterator(__vector_iterator const&) = default;
            __vector_iterator& operator=(__vector_iterator const&) = default;

            __vector_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[index_];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[index_ + index];
            }

            bool operator==(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_const_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using reference            = const_reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector const* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_const_iterator(vector const& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }
        
        public:
            __vector_const_iterator(__vector_iterator const& that) :
                vector_(that.vector_),
                index_(that.index_)
            {
            }

            __vector_const_iterator(__vector_const_iterator const&) = default;
            __vector_const_iterator& operator=(__vector_const_iterator const&) = default;

            __vector_const_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_const_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_const_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_const_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_const_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_const_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_const_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_const_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_const_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[index_];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[index_ + index];
            }

            bool operator==(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_const_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_const_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_const_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_reverse_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using reference            = reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector* vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_reverse_iterator(vector& vec, difference_type index) :
                vector_(&vec),
                index_(index)
            {
            }

            difference_type translate_(difference_type index)
            {
                return static_cast<difference_type>(vector_->size()) - index_ - 1;
            }
        
        public:
            __vector_reverse_iterator(__vector_reverse_iterator const&) = default;
            __vector_reverse_iterator& operator=(__vector_reverse_iterator const&) = default;

            __vector_reverse_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_reverse_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_reverse_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_reverse_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_reverse_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_reverse_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_reverse_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_reverse_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_reverse_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[translate_(index_)];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[translate_(index_ + index)];
            }

            bool operator==(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_reverse_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_reverse_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_reverse_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_reverse_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        class __vector_reverse_const_iterator
        {
        public:
            using value_type           = value_type;
            using difference_type      = difference_type;
            using reference            = const_reference;
            using iterator_category    = std::random_access_iterator_tag;

        protected:
            vector& vector_;
            difference_type index_;
        
        protected:
            friend class vector;
            __vector_reverse_const_iterator(vector& vec, difference_type index) :
                vector_(vec),
                index_(index)
            {
            }

            difference_type translate_(difference_type index)
            {
                return static_cast<difference_type>(vector_->size()) - index_ - 1;
            }
        
        public:
            __vector_reverse_const_iterator(__vector_reverse_iterator const& that) :
                vector_(that.vector_),
                index_(that.index_)
            {
            }
            __vector_reverse_const_iterator(__vector_reverse_const_iterator const&) = default;
            __vector_reverse_const_iterator& operator=(__vector_reverse_const_iterator const&) = default;

            __vector_reverse_const_iterator& operator++()
            {
                index_++;
                return *this;
            }

            __vector_reverse_const_iterator operator++(int)
            {
                auto prev = *this;
                index_++;
                return prev;
            }

            __vector_reverse_const_iterator& operator+=(difference_type diff)
            {
                index_ += diff;
                return *this;
            }

            __vector_reverse_const_iterator operator+(difference_type diff) const
            {
                auto tmp = *this;
                return tmp += diff;
            }

            __vector_reverse_const_iterator& operator--()
            {
                index_--;
                return *this;
            }

            __vector_reverse_const_iterator operator--(int)
            {
                auto prev = *this;
                index_--;
                return prev;
            }

            __vector_reverse_const_iterator& operator-=(difference_type diff)
            {
                index_ -= diff;
                return *this;
            }

            __vector_reverse_const_iterator operator-(difference_type diff) const
            {
                auto tmp = *this;
                return tmp -= diff;
            }

            difference_type operator-(__vector_reverse_const_iterator that) const
            {
                return index_ - that.index_;
            }

            reference operator*()
            {
                return (*vector_)[translate_(index_)];
            }

            reference operator[](difference_type index)
            {
                return (*vector_)[translate_(index_ + index)];
            }

            bool operator==(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ == that.index_);
            }

            bool operator!=(__vector_reverse_const_iterator const& that) const
            {
                return !(*this == that);
            }

            bool operator<(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ < that.index_);
            }

            bool operator>(__vector_reverse_const_iterator const& that) const
            {
                return (vector_ == that.vector_) &&
                       (index_ > that.index_);
            }

            bool operator<=(__vector_reverse_const_iterator const& that) const
            {
                return (*this < that) || (*this == that);
            }

            bool operator>=(__vector_reverse_const_iterator const& that) const
            {
                return (*this > that) || (*this == that);
            }
        };

        using iterator             = __vector_iterator;
        using const_iterator       = __vector_const_iterator;
        using reverse_iterator     = __vector_reverse_iterator;
        using const_reverse_iterator
                                   = __vector_reverse_const_iterator;

    protected:
        storage_type storage_;
        size_type size_ = 0;

        inline size_type octet_number_(size_type bits)
        {
            return (bits + 8 * sizeof(real_type) - 1) / (8 * sizeof(real_type));
        }

        inline size_type octet_by_bit_(size_type bit)
        {
            return bit / (8 * sizeof(real_type));
        }

        inline void check_index_(difference_type index, char const* fnc) const
        {
            if (index >= static_cast<difference_type>(size_))
                std::__throw_out_of_range_fmt("%s: "
                    "index == %zu out of range within size == %zu",
                    fnc, index, size_);
        }

        template<typename It>
        inline void check_iterator_(It const& it, char const* fnc)
        {
            if (this != it.vector_)
                std::__throw_out_of_range_fmt("%s: "
                    "wrong iterator, this (%p) != it.vector_ (%p)",
                    fnc, this, it.vector_);
            
            if (it.index_ != size_)
                check_index_(it.index_, fnc);
        }

        inline bool is_created_(difference_type i)
        {
            return octet_by_bit_(i) < octets_number_(size_)
        }

        inline void realloc_if_needed_(difference_type size_diff = 1)
        {    
            auto current_capacity = storage_.capacity();
            auto new_size = size_ + size_diff;
            if (size_diff < 0)
            {
                assert(-size_diff < size_);
            }

            assert(size_ <= current_capacity);

            if (octets_numbers_(new_size * 4) <= current_capacity && current_capacity > 0)
            {
                storage_.realloc((current_capacity + 1) / 2, size_);
                return;
            }

            auto new_capacity = current_capacity;
            while (octets_numbers_(new_size) >= new_capacity)
                new_capacity = std::max(new_capacity * 2, static_cast<size_type>(1));
            
            storage_.realloc(new_capacity, octets_numbers_(size_));
        }

        inline void move_tail_(difference_type start, difference_type shift)
        {
            if (shift == 0)
                return;

            if (shift < 0)
                for (difference_type i = start; i != size_; i++)
                    at_unchecked(i + shift) = std::move(at_unchecked(i));
            
            else
                for (difference_type i = size_ - 1; i >= start; i--)
                {
                    if (!is_created_(i + shift))
                        storage_.create(i + shift);

                    at_unchecked(i + shift) = std::move(at_unchecked(i));
                }
        }

    public:
        // inline void reserve(size_type new_capacity)
        explicit vector(size_type size = 0)
        {
            reserve(size);
            difference_type i = 0;
            for (; i < size; i += 8 * sizeof(real_type))
                storage_.create(i / (8 * sizeof(real_type)));
                
            size_ = i;
        }

        template<typename... Args>
        explicit vector(size_type size, Args&&... args)
        {
            bool value = bool(std::forward<Args>(args)...);
            real_type fill = value ? std::numeric_limits<real_type>::max() : 0;
            reserve(size);
            difference_type i = 0;
            for (; i < size; i += 8 * sizeof(real_type))
                storage_.create(octet_by_bit_(i), fill);
                
            size_ = i;
        }

        // not explicit!
        vector(std::initializer_list<value_type> list) :
            size_(list.size())
        {
            reserve(size);

            difference_type i = 0;
            for (auto it = list.begin(); it != list.end(); ++it, i++)
                at_unchecked(i) = !!(*it);
        }

        vector(vector const& origin) noexcept :
            size_(origin.size_)
        {
            reserve(size_);

            for (difference_type i = 0; i != octet_number_(size_); i++)
                storage_.create(i, origin.storage_.at_unchecked(i));
        }

        vector(vector&& origin) :
            size_(origin.size_)
        {
            reserve(size_);

            for (difference_type i = 0; i != octet_number_(size_); i++)
                storage_.create(i, origin.storage_.at_unchecked(i));
        }

        // void clear() noexcept;
        
        ~vector() noexcept
        {
            clear();
        }

        vector& operator=(vector const& origin)
        {
            reserve(origin.size_);
            size_ = origin.size_;

            for (difference_type i = 0; i != octet_number_(size_); i++)
                storage_.create(i, origin.storage_.at_unchecked(i));         

            return *this;
        }
        
        vector& operator=(vector&& origin)
        {
            reserve(origin.size_);
            size_ = origin.size_;

            for (difference_type i = 0; i != octet_number_(size_); i++)
                storage_.create(i, origin.storage_.at_unchecked(i));         

            return *this;
        }

        vector& operator=(std::initializer_list<value_type> list)
        {
            reserve(origin.size_);
            size_ = list.size();

            difference_type i = 0;
            for (auto it = list.begin(); it != list.end(); it++, i++)
                at_unchecked(i) = *it;

            return *this;
        }

        // TODO: assign

        [[nodiscard]] inline allocator_type get_allocator() const
        {
            return allocator_type();
        }

        //
        // Element access
        //

        [[nodiscard]] inline const_reference at_unchecked(difference_type index) const noexcept
        {
            return __bool_const_ref::create_(storage_.data(), index);
        }

        [[nodiscard]] inline reference at_unchecked(difference_type index) noexcept
        {
            return const_cast<reference>(static_cast<vector const*>(this)->at_unchecked(index));
        }

        [[nodiscard]] inline const_reference operator[](difference_type index) const
        {
            check_index_(index, "vector::operator[](difference_type)");
            return storage_.at_unchecked(index);
        }

        [[nodiscard]] inline reference operator[](difference_type index)
        {
            return const_cast<reference>(static_cast<vector const*>(this)->operator[](index));
        }

        [[nodiscard]] inline const_reference front() const
        {
            return operator[](0);
        }

        [[nodiscard]] inline reference front()
        {
            return const_cast<reference>(static_cast<vector const*>(this)->front());
        }

        [[nodiscard]] inline const_reference back() const
        {
            return operator[](size_ - 1);
        }

        [[nodiscard]] inline reference back()
        {
            return const_cast<reference>(static_cast<vector const*>(this)->back());
        }

        [[nodiscard]] inline const_pointer data() const noexcept
        {
            return storage_.data();
        }

        [[nodiscard]] inline pointer data() noexcept
        {
            return const_cast<pointer>(static_cast<vector const*>(this)->data());
        }

        //
        // Iterators
        //

        iterator begin()
        {
            return iterator(*this, 0);
        }

        const_iterator cbegin() const
        {
            return const_iterator(*this, 0);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(*this, 0);
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(*this, 0);
        }

        const_reverse_iterator rbegin() const
        {
            return crbegin();
        }

        //--

        iterator end()
        {
            return iterator(*this, size_);
        }

        const_iterator cend() const
        {
            return const_iterator(*this, size_);
        }

        const_iterator end() const
        {
            return cend();
        }

        reverse_iterator rend()
        {
            return reverse_iterator(*this, size_);
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(*this, size_);
        }

        const_reverse_iterator rend() const
        {
            return crend();
        }

        //
        // Capacity
        //

        [[nodiscard]] inline bool empty() const noexcept
        {
            return size_ == 0;
        }

        [[nodiscard]] inline size_type size() const noexcept
        {
            return size_;
        }

        [[nodiscard]] inline size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max();
        }

        inline void reserve(size_type new_capacity)
        {
            if (new_capacity > storage_.capacity())
                storage_.realloc(new_capacity, size_);
        }

        [[nodiscard]] inline size_type capacity() const noexcept
        {
            return storage_.capacity();
        }

        inline void shrink_to_fit()
        {
            storage_.realloc(octets_number_(size_), octets_number_(size_));
        }

        //
        // Modifiers
        //

        inline void clear() noexcept
        {
            difference_type i = size_;
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

        inline void resize(size_type new_size)
        {
            if (new_size == size_)
                return;
            
            reserve(new_size);

            if (new_size > size_)
            {
                size_type i = size_;
                try
                {
                    for (; i != new_size; i++)
                        storage_.create(i);
                    
                    size_ = i;
                }
                catch (...)
                {
                    for (; i > size_;)
                        storage_.destroy(--i);

                    size_ = i;
                    throw; // up
                }
            }

            else
            {
                for (size_type i = size_; i != new_size;)
                    storage_.destroy(--i);

                size_ = new_size;
            }
        }

        // no nodiscard!
        template<typename... Args>
        inline reference emplace_back(Args&&... args)
        {
            realloc_if_needed_();
            storage_.create(static_cast<difference_type>(size_++), std::forward<Args>(args)...);
            return back();
        }

        inline void push_back(T const& value)
        {
            emplace_back(value);
        }

        inline void push_back(T&& value)
        {
            emplace_back(std::move(value));
        }

        inline void pop_back()
        {
            check_index_(0, "vector::pop_back()");
            storage_.destroy(static_cast<difference_type>(size_ - 1));
            realloc_if_needed_(-1);
            size_--;
        }

        inline iterator insert(const_iterator position, T const& value)
        {
            check_iterator_(position, "vector::insert(const_iterator, T const&)");
            realloc_if_needed_();
            auto index = position - begin();
            move_tail_(index, 1);
            if (!is_created_(index))
                storage_.create(index, value);
            else
                at_unchecked(index) = value;
            size_++;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, T&& value)
        {
            check_iterator_(position, "vector::insert(const_iterator, T&&)");
            realloc_if_needed_();
            auto index = position - begin();
            move_tail_(index, 1);

            if (!is_created_(index))
                storage_.create(index, std::move(value));
            else
                at_unchecked(index) = std::move(value);

            size_++;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, size_type count, T const& value)
        {
            if (count == 0)
                return iterator(*this, position.index_);

            check_iterator_(position, "vector::insert(const_iterator, size_type, T const&)");
            realloc_if_needed_(count);

            auto index = position - begin();
            move_tail_(index, static_cast<difference_type>(count));
            for (auto i = index; i != index + count; i++)
            {
                if (!is_created_(i))
                    storage_.create(i, value);

                else
                    at_unchecked(i) = value;
            }

            size_ += count;
            return begin() + index;
        }

        template<typename It>
        inline iterator insert(const_iterator position, It first, It last)
        {
            if (!(first < last))
                return iterator(*this, position.index_);

            check_iterator_(position, "vector::insert(const_iterator, It, It)");
            auto count = last - first;
            realloc_if_needed_(count);
            auto index = position - begin();            
            move_tail_(index, static_cast<difference_type>(count));
            for (auto i = index; 
                 i != index + count && first < last; 
                 i++, ++first)
            {
                if (!is_created_(i))
                    storage_.create(i, *first);

                else
                    at_unchecked(i) = *first;
            }

            size_ += count;
            return begin() + index;
        }

        inline iterator insert(const_iterator position, std::initializer_list<T> list)
        {
            check_iterator_(position, "vector::insert(const_iterator, std::initializer_list<T>)");
            auto count = list.size();
            realloc_if_needed_(static_cast<difference_type>(count));

            auto index = position - begin();
            move_tail_(index, static_cast<difference_type>(count));
            auto it = list.begin();
            for (auto i = index; i != index + count; i++, ++it)
            {
                if (!is_created_(i))
                    storage_.create(i, std::move(*it));

                else
                    at_unchecked(i) = *it;
            }

            size_ += count;
            return begin() + index;
        }

        inline iterator erase(const_iterator first, const_iterator last)
        {
            check_iterator_(first, "vector::erase(const_iterator, const_iterator): first");
            check_iterator_(last, "vector::erase(const_iterator, const_iterator): second");

            if (!(first < last))
                return iterator(*this, last.index_);

            auto count = last - first;
            auto index = first.index_;

            move_tail_(last.index_, -count);
            for (auto i = size_ - count; i != size_; i++)
                storage_.destroy(i);
            
            size_ -= count;
            return begin() + index;
        }

        inline iterator erase(const_iterator position)
        {
            return erase(position, position + 1);
        }
    };



    // template<size_t MaxSize, template<typename, size_t, class> class Storage>
    // class vector<bool, MaxSize, Storage>
    // {
    // protected:
    //     struct __bool_ref
    //     {
    //     friend class vector<bool, MaxSize, Storage>;
    //     __bool_ref(__bool_ref const&) = default; // not explicit

    //     __bool_ref& operator=(bool value) noexcept
    //     {
    //         octet_ &= ~(1 << bit_);
    //         octet_ |= (1 << bit_) * value;
    //         return *this;
    //     }

    //     __bool_ref& operator=(__bool_ref const& other) noexcept
    //     {
    //         *this = bool(other);
    //         return *this;
    //     }

    //     operator bool() const noexcept
    //     {
    //         return (octet_ >> bit_) & 0x1;
    //     }

        
    //     private:
    //     __bool_ref(uint8_t& octet, size_t bit) noexcept :
    //         octet_(octet), bit_(bit)
    //     {
    //     }

    //     static __bool_ref create_(uint8_t* data, size_t index)
    //     {
    //         return __bool_ref(data[index >> 3], index & 7ull);
    //     }

    //     private:
    //         uint8_t& octet_;
    //         size_t const bit_;
    //     };

    //     struct __bool_const_ref
    //     {
    //     friend class vector<bool, MaxSize, Storage>;
    //     __bool_const_ref(__bool_const_ref const&) = default; // not explicit

    //     operator bool() const noexcept
    //     {
    //         return (octet_ >> bit_) & 0x1;
    //     }

        
    //     private:
    //     __bool_const_ref(uint8_t const& octet, size_t bit) noexcept :
    //         octet_(octet), bit_(bit)
    //     {
    //     }

    //     static __bool_const_ref create_(uint8_t const* data, size_t index)
    //     {
    //         return __bool_const_ref(data[index >> 3], index & 7ull);
    //     }

    //     private:
    //         uint8_t const& octet_;
    //         size_t const bit_;
    //     };
    
    // protected:
    //     static size_t constexpr Capacity = (MaxSize + 7) / 8;
    //     Storage<uint8_t, Capacity, jules::allocator::Default<uint8_t, true>> storage_;
    //     size_t size_ = 0;

    //     inline void check_size_(size_t size, char const* fnc) const
    //     {
    //         if (size > MaxSize)
    //             std::__throw_out_of_range_fmt("%s: "
    //                 "size_t == %zu exceeds max_size == %zu",
    //                 fnc, size_, MaxSize);
    //     }

    //     inline void check_index_(size_t index, char const* fnc) const
    //     {
    //         if (index >= size_)
    //             std::__throw_out_of_range_fmt("%s: "
    //                 "index == %zu out of range within size == %zu",
    //                 fnc, index, size_);
    //     }

    // public:
    //     explicit vector(size_t size = 0) noexcept :
    //         size_(size)
    //     {
    //         check_size_(size_, "vector::vector(size_t)");
    //         for (size_t i = 0; i < size_; i++)
    //             storage_.create(i / 8, 0);
    //     }

    //     template<typename U>
    //     explicit vector(size_t size, U&& value) :
    //         size_(size)
    //     {
    //         check_size_(size_, "vector::vector(size_t, U)");
    //         bool fill = !!(value);
    //         for (size_t i = 0; i != size_; i++)
    //             at_unchecked(i) = fill;
    //     }

    //     // not explicit!
    //     vector(std::initializer_list<bool> list) :
    //         size_(list.size())
    //     {
    //         check_size_(size_, "vector::vector(std::initializer_list<T>)");

    //         size_t i = 0;
    //         for (auto it = list.begin(); it != list.end(); ++it, i++)
    //             at_unchecked(i) = !!(*it);
    //     }

    //     vector(vector const& origin) noexcept :
    //         size_(origin.size_)
    //     {
    //         size_t i = 0;
    //         for (; i != (size_ + 7) / 8; i++)
    //             storage_.create(i, origin.storage_.at_unchecked(i));
    //     }

    //     vector(vector&& origin) noexcept :
    //         size_(origin.size_)
    //     {
    //         size_t i = 0;
    //         for (; i != (size_ + 7) / 8; i++)
    //             storage_.create(i, origin.storage_.at_unchecked(i));
    //     }

    //     // void clear() noexcept;

    //     ~vector() noexcept
    //     {
            
    //     }

    //     vector& operator=(vector const& origin)
    //     {
    //         size_t i = 0;
    //         for (; i != (size_ + 7) / 8; i++)
    //             storage_.create(i, origin.storage_.at_unchecked(i));

    //         return *this;
    //     }

    //     vector& operator=(vector&& origin)
    //     {
    //         size_t i = 0;
    //         for (; i != (size_ + 7) / 8; i++)
    //             storage_.create(i, origin.storage_.at_unchecked(i));

    //         return *this;
    //     }

    //     vector& operator=(std::initializer_list<bool> const& list)
    //     {
    //         size_ = list.size();
    //         check_size_(size_, "vector::operator=(std::initializer_list<T>)");

    //         size_t i = 0;
    //         for (auto it = list.begin(); it != list.end(); it++, i++)
    //             at_unchecked(i) = *it;

    //         return *this;
    //     }

    //     //
    //     // Element access
    //     //

    //     [[nodiscard]] inline __bool_const_ref at_unchecked(size_t index) const noexcept
    //     {
    //         return __bool_const_ref::create_(storage_.data(), index);
    //     }

    //     [[nodiscard]] inline __bool_ref at_unchecked(size_t index) noexcept
    //     {
    //         return __bool_ref::create_(storage_.data(), index);
    //     }

    //     [[nodiscard]] inline __bool_const_ref operator[](size_t index) const
    //     {
    //         check_index_(index, "vector::operator[](size_t)");
    //         return at_unchecked(index);
    //     }

    //     [[nodiscard]] inline __bool_ref operator[](size_t index)
    //     {
    //         check_index_(index, "vector::operator[](size_t)");
    //         return at_unchecked(index);
    //     }

    //     [[nodiscard]] inline __bool_const_ref front() const
    //     {
    //         return operator[](0);
    //     }

    //     [[nodiscard]] inline __bool_ref front()
    //     {
    //         return operator[](0);
    //     }

    //     [[nodiscard]] inline __bool_const_ref back() const
    //     {
    //         return operator[](size_ - 1);
    //     }

    //     [[nodiscard]] inline __bool_ref back()
    //     {
    //         return operator[](size_ - 1);
    //     }

    //     [[nodiscard]] inline uint8_t const* data(size_t index) const noexcept
    //     {
    //         return storage_.data();
    //     }

    //     [[nodiscard]] inline uint8_t* data(size_t index) noexcept
    //     {
    //         return const_cast<vector*>(static_cast<vector const*>(this)->data());
    //     }

    //     //
    //     // Iterators
    //     //


    //     //
    //     // Capacity
    //     //

    //     [[nodiscard]] inline bool empty() const noexcept
    //     {
    //         return size_ == 0;
    //     }

    //     [[nodiscard]] inline size_t size() const noexcept
    //     {
    //         return size_;
    //     }

    //     [[nodiscard]] inline size_t max_size() const noexcept
    //     {
    //         return MaxSize;
    //     }

    //     //
    //     // Modifiers
    //     //
    //     void resize(size_t new_size) noexcept
    //     {
    //         if (new_size == size_)
    //             return;

    //         check_size_(new_size, "vector::resize()");

    //         if (new_size > size_)
    //         {
    //             for (size_t i = size_; i != new_size; i++)
    //                 at_unchecked(i) = false;

    //             size_ = new_size;
    //         }

    //         else
    //             size_ = new_size;
    //     }

    //     void clear() noexcept
    //     {
    //         size_ = 0;
    //     }
    // };
}
