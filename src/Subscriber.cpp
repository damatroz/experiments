#include "ReflectiveStruct.h"

#include <optional>
#include <string>
#include <tuple>

namespace detail
{
    template <typename Ret, typename T, typename V = void>
    struct Helper
    {
        static Ret* GetMember(const std::string& key, T& value)
        {
            return nullptr;
        }
    };

    template <typename T>
    struct Helper<T, T>
    {
        static T* GetMember(const std::string& key, T& value)
        {
            if (!key.empty())
            {
                return nullptr;
            }
            return &value;
        }
    };

    template <typename Ret, typename T>
    struct HelperReflective
    {
        static Ret* GetMember(T&)
        {
            return nullptr;
        }
    };

    template <typename T>
    struct HelperReflective<T, T>
    {
        static T* GetMember(T& value)
        {
            return &value;
        }
    };

    template <typename Ret, std::size_t I = 0, typename... Tp>
    typename std::enable_if_t<I == sizeof...(Tp), Ret*>
        GetMemberReflective(const std::string& currKey, const std::string& restKey, const std::tuple<Tp...>& t)
    {
        return nullptr;
    }

    template <typename Ret, std::size_t I = 0, typename... Tp>
    typename std::enable_if_t<I < sizeof...(Tp), Ret*>
        GetMemberReflective(const std::string& currKey, const std::string& restKey, const std::tuple<Tp...>& t)
    {
        if (currKey == std::get<I>(t).name)
        {
            return Helper<Ret, std::decay_t<decltype(std::get<I>(t).value)>>::GetMember(restKey, std::get<I>(t).value);
        }
        return GetMemberReflective<Ret, I + 1, Tp...>(currKey, restKey, t);
    }

    template <typename Ret, typename T>
    struct Helper<Ret, T, std::void_t<typename T::ReflectiveStruct>>
    {
        static Ret* GetMember(const std::string& key, T& value)
        {
            if (key.empty())
            {
                return HelperReflective<Ret, T>::GetMember(value);
            }
            auto pos = key.find(".");
            std::string currKey = key.substr(0, pos);
            std::string restKey;
            if (pos != std::string::npos)
            {
                restKey = key.substr(pos + 1);
            }
            return GetMemberReflective<Ret>(currKey, restKey, value.GetMembers());
        }
    };
}

template <typename Ret, typename T>
Ret* GetMember(const std::string& key, T& value)
{
    return detail::Helper<Ret, T>::GetMember(key, value);
}

#include <functional>

template <typename T>
class Subscriber
{
public:
    Subscriber(T& value, std::function<void()> func)
        : m_value(value)
        , m_func(func)
    {

    }

    const T& Read()
    {
        return m_value;
    }

    void Write(std::function<void(T& value)> func)
    {
        func(m_value);
        m_func();
    }

private:
    T& m_value;
    std::function<void()> m_func;
};

template <typename Ret, typename T>
std::optional<Subscriber<Ret>> GetSubscriber(const std::string& key, T& value, std::function<void()> func)
{
    auto ret = GetMember<Ret>(key, value);
    if (ret)
    {
        return Subscriber(*ret, func);
    }
    else
    {
        return std::nullopt;
    }
}

//=======================================================================================================

#include <iostream>

int main()
{
    struct Test
    {
        struct Data
        {
            RS_MEMBERS(
                (double, a, 2.0)
            )
        };

        RS_MEMBERS(
            (int, a, 2)
            ((std::tuple<bool, double, std::string>), b, ({ true, 2.5, "test" }))
            (std::string, c)
            (Data, d)
        )
    };
    Test test;
    test.d.a;
    auto subscriber = GetSubscriber<std::tuple<bool, double, std::string>>("b", test, []() { std::cout << "written" << std::endl; });
    if (subscriber)
    {
        subscriber->Write([](auto& value) { std::get<1>(value) = 3.5; });
    }
    else
    {
        std::cout << "nullopt" << std::endl;
    }

    return 0;
}
