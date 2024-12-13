#include "ReflectiveStruct.h"

int main(int argc, char *argv[])
{
    struct Data
    {
        struct Video
        {
            struct Window
            {
                RS_MEMBERS(
                    ((std::tuple<int, int, int, int>), position, ({ 200, 200, 640, 400 }))
                    (bool, fullscreen, false)
                )
            };

            RS_MEMBERS(
                (Window, window)
            )
        };

        RS_MEMBERS(
            (Video, video)
        )
    };

    RS_STRUCT(Data2,
        RS_STRUCT_MEMBER(Video, video,
            RS_STRUCT_MEMBER(Window, window,
                ((std::tuple<int, int, int, int>), position, ({ 200, 200, 640, 400 }))
                (bool, fullscreen, false)
            )
        )
    );

    RS_STRUCT(A,
        (int, a, 0)
        RS_STRUCT_MEMBER(Data1, data,
            (int, b, 1)
            RS_STRUCT_MEMBER(Data2, data,
                (int, b, 1)
            )
        )
        (int, c)
    );

    A a;
    a.GetMembers();

    return 0;
}
