#include "ReflectiveStruct.h"
#include "Json.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    RS_STRUCT(Data,
        RS_STRUCT_MEMBER(Video, video,
            RS_STRUCT_MEMBER(Window, window,
                ((std::tuple<int, int, int, int>), position, ({ 200, 200, 640, 400 }))
                (bool, fullscreen, false)
            )
        )
    );
    Data m_data;

    std::istringstream iss("{}");
    rapidjson::IStreamWrapper isw(iss);

    rapidjson::Document d;
    d.ParseStream(isw);

    Json::Read(m_data, d);

    rapidjson::OStreamWrapper osw(std::cout);

    rapidjson::Writer writer(osw);
    Json::Write(writer, m_data);
}
