#ifndef TYPEMAP_H
#define TYPEMAP_H

#include "typelist.h"
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace typemap
{
    namespace detail
    {
        template <typename T>
        struct ValueHolder
        {
            T value;
            bool has_value = false;
        };

        template <typename TypeList>
        struct TypeMapStorage;

        template <>
        struct TypeMapStorage<typelist::TypeList<>>
        {
            static constexpr size_t size = 0;
        };

        template <typename First, typename... Rest>
        struct TypeMapStorage<typelist::TypeList<First, Rest...>>
        {
            ValueHolder<First> first;
            TypeMapStorage<typelist::TypeList<Rest...>> rest;

            static constexpr size_t size = 1 + TypeMapStorage<typelist::TypeList<Rest...>>::size;
        };

        template <size_t Index, typename Storage>
        struct GetByIndexHelper;

        template <typename First, typename... Rest>
        struct GetByIndexHelper<0, TypeMapStorage<typelist::TypeList<First, Rest...>>>
        {
            static ValueHolder<First> &get(TypeMapStorage<typelist::TypeList<First, Rest...>> &storage)
            {
                return storage.first;
            }

            static const ValueHolder<First> &get(const TypeMapStorage<typelist::TypeList<First, Rest...>> &storage)
            {
                return storage.first;
            }
        };

        template <size_t Index, typename First, typename... Rest>
        struct GetByIndexHelper<Index, TypeMapStorage<typelist::TypeList<First, Rest...>>>
        {
            static auto &get(TypeMapStorage<typelist::TypeList<First, Rest...>> &storage)
            {
                return GetByIndexHelper<Index - 1, TypeMapStorage<typelist::TypeList<Rest...>>>::get(storage.rest);
            }

            static const auto &get(const TypeMapStorage<typelist::TypeList<First, Rest...>> &storage)
            {
                return GetByIndexHelper<Index - 1, TypeMapStorage<typelist::TypeList<Rest...>>>::get(storage.rest);
            }
        };

        template <typename T, typename TypeList>
        struct FindTypeIndex;

        template <typename T, typename First, typename... Rest>
        struct FindTypeIndex<T, typelist::TypeList<First, Rest...>>
        {
            static constexpr int value = std::is_same_v<T, First> ? 0 : (FindTypeIndex<T, typelist::TypeList<Rest...>>::value == -1 ? -1 : FindTypeIndex<T, typelist::TypeList<Rest...>>::value + 1);
        };

        template <typename T>
        struct FindTypeIndex<T, typelist::TypeList<>>
        {
            static constexpr int value = -1;
        };
    }

    template <typename... KeyTypes>
    class TypeMap
    {
    private:
        using KeyTypeList = typelist::TypeList<KeyTypes...>;
        using Storage = detail::TypeMapStorage<KeyTypeList>;
        Storage m_storage;

        template <typename T>
        static constexpr int get_index()
        {
            constexpr int idx = typelist::index_of_v<T, KeyTypeList>;
            static_assert(idx != -1, "Type is not a valid key for this TypeMap");
            return idx;
        }

        template <size_t Index>
        auto &get_holder()
        {
            return detail::GetByIndexHelper<Index, Storage>::get(m_storage);
        }

        template <size_t Index>
        const auto &get_holder() const
        {
            return detail::GetByIndexHelper<Index, Storage>::get(m_storage);
        }

    public:
        TypeMap() = default;

        template <typename T>
        void AddValue(T &&value)
        {
            constexpr int idx = get_index<T>();
            auto &holder = get_holder<idx>();
            holder.value = std::forward<T>(value);
            holder.has_value = true;
        }

        template <typename T>
        void AddValue(const T &value)
        {
            constexpr int idx = get_index<T>();
            auto &holder = get_holder<idx>();
            holder.value = value;
            holder.has_value = true;
        }

        template <typename T>
        T &GetValue()
        {
            constexpr int idx = get_index<T>();
            auto &holder = get_holder<idx>();
            if (!holder.has_value)
            {
                throw std::bad_optional_access();
            }
            return holder.value;
        }

        template <typename T>
        const T &GetValue() const
        {
            constexpr int idx = get_index<T>();
            const auto &holder = get_holder<idx>();
            if (!holder.has_value)
            {
                throw std::bad_optional_access();
            }
            return holder.value;
        }

        template <typename T>
        bool Contains() const
        {
            constexpr int idx = get_index<T>();
            const auto &holder = get_holder<idx>();
            return holder.has_value;
        }

        template <typename T>
        void RemoveValue()
        {
            constexpr int idx = get_index<T>();
            auto &holder = get_holder<idx>();
            holder.has_value = false;
        }

        template <typename T>
        std::optional<std::reference_wrapper<T>> TryGetValue()
        {
            constexpr int idx = get_index<T>();
            auto &holder = get_holder<idx>();
            if (holder.has_value)
            {
                return std::ref(holder.value);
            }
            return std::nullopt;
        }

        template <typename T>
        std::optional<std::reference_wrapper<const T>> TryGetValue() const
        {
            constexpr int idx = get_index<T>();
            const auto &holder = get_holder<idx>();
            if (holder.has_value)
            {
                return std::cref(holder.value);
            }
            return std::nullopt;
        }

        void Clear()
        {
            clear_impl(m_storage, std::index_sequence_for<KeyTypes...>{});
        }

        static constexpr size_t Capacity()
        {
            return sizeof...(KeyTypes);
        }

        size_t Size() const
        {
            return size_impl(m_storage, std::index_sequence_for<KeyTypes...>{});
        }

        bool Empty() const
        {
            return Size() == 0;
        }

    private:
        template <size_t... Indices>
        void clear_impl(Storage &storage, std::index_sequence<Indices...>)
        {
            (clear_holder<Indices>(storage), ...);
        }

        template <size_t Index>
        void clear_holder(Storage &storage)
        {
            auto &holder = detail::GetByIndexHelper<Index, Storage>::get(storage);
            holder.has_value = false;
        }

        template <size_t... Indices>
        size_t size_impl(const Storage &storage, std::index_sequence<Indices...>) const
        {
            size_t count = 0;
            ((count += (detail::GetByIndexHelper<Indices, Storage>::get(storage).has_value ? 1 : 0)), ...);
            return count;
        }
    };
}

#endif