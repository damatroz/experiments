#include <tuple>
#include <variant>

template <class Variant, class Type>
struct contains;

template <class Type, class... Types, class OtherType>
struct contains<std::variant<Type, Types...>, OtherType> : contains<std::variant<Types...>, OtherType>
{};

template <class... Types, class Type>
struct contains<std::variant<Type, Types...>, Type> : std::true_type
{};

template <class Type>
struct contains<std::variant<>, Type> : std::false_type
{};

template <class Out, class In>
struct filter;

template <class... Out, class Type, class... Types>
struct filter<std::variant<Out...>, std::tuple<Type, Types...>>
{
    using type = typename std::conditional<
        contains<std::variant<Out...>, Type>::value,
        typename filter<std::variant<Out...>, std::tuple<Types...>>::type,
        typename filter<std::variant<Out..., Type>, std::tuple<Types...>>::type
    >::type;
};

template <class Out>
struct filter<Out, std::tuple<>>
{
    using type = Out;
};


template <class T>
using without_duplicates = typename filter<std::variant<std::nullptr_t>, T>::type;

int main(int argc, char* argv[])
{
    without_duplicates<std::tuple<int, int, double>> a;

    return 0;
}
