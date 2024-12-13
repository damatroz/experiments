#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <string>
#include <tuple>

namespace Json
{
    void Read(bool& value, const rapidjson::Value& jsonValue);

    void Read(int& value, const rapidjson::Value& jsonValue);

    void Read(double& value, const rapidjson::Value& jsonValue);

    void Read(std::string& value, const rapidjson::Value& jsonValue);

    template <typename ...T>
    void Read(std::tuple<T...>& values, const rapidjson::Value& jsonValue)
    {
        if (jsonValue.IsArray())
        {
            std::apply(
                [jsonArray = jsonValue.GetArray()](auto&... elem)
                {
                    unsigned i = 0;
                    auto processElem = [&](auto& elem) {
                        if (i < jsonArray.Size())
                        {
                            Read(elem, jsonArray[i++]);
                        }
                    };
                    (processElem(elem), ...);
                }, values);
        }
    }

    template <typename T>
    std::void_t<typename T::ReflectiveStruct> Read(T& values, const rapidjson::Value& jsonValue)
    {
        if (jsonValue.IsObject())
        {
            std::apply(
                [jsonObject = jsonValue.GetObject()](const auto&... member)
            {
                auto processMember = [&jsonObject](const auto& member) {
                    if (jsonObject.HasMember(member.name))
                    {
                        Read(member.value, jsonObject[member.name]);
                    }
                };
                (processMember(member), ...);
            }, values.GetMembers());
        }
    }

    template <typename Buffer>
    void Write(rapidjson::Writer<Buffer>& writer, bool value)
    {
        writer.Bool(value);
    }

    template <typename Buffer>
    void Write(rapidjson::Writer<Buffer>& writer, int value)
    {
        writer.Int(value);
    }

    template <typename Buffer>
    void Write(rapidjson::Writer<Buffer>& writer, double value)
    {
        writer.Double(value);
    }

    template <typename Buffer>
    void Write(rapidjson::Writer<Buffer>& writer, const std::string& value)
    {
        writer.String(value.c_str());
    }

    template <typename Buffer, typename ...T>
    void Write(rapidjson::Writer<Buffer>& writer, const std::tuple<T...>& values)
    {
        writer.StartArray();
        std::apply(
            [&writer](auto&... elem)
            {
                unsigned i = 0;
                ((Write(writer, elem)), ...);
            }, values);
        writer.EndArray();
    }

    template <typename Buffer, typename T>
    std::void_t<typename T::ReflectiveStruct> Write(rapidjson::Writer<Buffer>& writer, const T& values)
    {
        writer.StartObject();
        std::apply(
            [&writer](const auto&... member)
            {
                ((writer.Key(member.name), Write(writer, member.value)), ...);
            }, values.GetMembers());
        writer.EndObject();
    }
}
