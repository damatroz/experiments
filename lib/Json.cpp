#include "Json.h"

namespace
{
    template <typename T, typename Is, typename Get>
    void GenericRead(T& value, const rapidjson::Value& jsonValue, Is isFunc, Get getFunc)
    {
        if ((jsonValue.*isFunc)())
        {
            value = (jsonValue.*getFunc)();
        }
    }
}

namespace Json
{
    void Read(bool& value, const rapidjson::Value& jsonValue)
    {
        GenericRead(value, jsonValue, &rapidjson::Value::IsBool, &rapidjson::Value::GetBool);
    }

    void Read(int& value, const rapidjson::Value& jsonValue)
    {
        GenericRead(value, jsonValue, &rapidjson::Value::IsInt, &rapidjson::Value::GetInt);
    }
    
    void Read(double& value, const rapidjson::Value& jsonValue)
    {
        GenericRead(value, jsonValue, &rapidjson::Value::IsDouble, &rapidjson::Value::GetDouble);
    }
    
    void Read(std::string& value, const rapidjson::Value& jsonValue)
    {
        GenericRead(value, jsonValue, &rapidjson::Value::IsString, &rapidjson::Value::GetString);
    }
}
