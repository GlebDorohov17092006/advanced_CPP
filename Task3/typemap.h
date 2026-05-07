#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <optional>
#include <tuple>
#include <type_traits>
#include <stdexcept>

namespace typemap
{
    template <typename T, typename... Types>
    struct IndexOf;

    template <typename T, typename First, typename... Rest>
    struct IndexOf<T, First, Rest...>
    {
        static constexpr int value = std::is_same_v<T, First> ? 0 : (IndexOf<T, Rest...>::value == -1 ? -1 : IndexOf<T, Rest...>::value + 1);
    };

    template <typename T>
    struct IndexOf<T>
    {
        static constexpr int value = -1;
    };

    template <typename T, typename... Types>
    inline constexpr int index_of_v = IndexOf<T, Types...>::value;

    template <typename... Keys>
    class TypeMap
    {
    private:
        template <typename T>
        static constexpr int index_of()
        {
            static_assert(index_of_v<T, Keys...> != -1, "Key not found in TypeMap");
            return index_of_v<T, Keys...>;
        }

        std::tuple<std::optional<Keys>...> values;

        template <size_t I>
        auto &get_opt() { return std::get<I>(values); }

        template <size_t I>
        const auto &get_opt() const { return std::get<I>(values); }

        template <size_t... Is>
        size_t size_impl(std::index_sequence<Is...>) const
        {
            return (get_opt<Is>().has_value() + ...);
        }

        template <size_t... Is>
        void clear_impl(std::index_sequence<Is...>)
        {
            (get_opt<Is>().reset(), ...);
        }

    public:
        TypeMap() = default;

        template <typename Key>
        void AddValue(const Key &val)
        {
            constexpr int idx = index_of<Key>();
            get_opt<idx>() = val;
        }

        template <typename Key>
        Key &GetValue()
        {
            constexpr int idx = index_of<Key>();
            auto &opt = get_opt<idx>();
            if (!opt.has_value())
                throw std::bad_optional_access();
            return opt.value();
        }

        template <typename Key>
        const Key &GetValue() const
        {
            constexpr int idx = index_of<Key>();
            const auto &opt = get_opt<idx>();
            if (!opt.has_value())
                throw std::bad_optional_access();
            return opt.value();
        }

        template <typename Key>
        bool Contains() const
        {
            constexpr int idx = index_of<Key>();
            return get_opt<idx>().has_value();
        }

        template <typename Key>
        void RemoveValue()
        {
            constexpr int idx = index_of<Key>();
            get_opt<idx>().reset();
        }

        size_t Size() const
        {
            return size_impl(std::index_sequence_for<Keys...>{});
        }

        static constexpr size_t Capacity()
        {
            return sizeof...(Keys);
        }

        void Clear()
        {
            clear_impl(std::index_sequence_for<Keys...>{});
        }
    };

}

#endif