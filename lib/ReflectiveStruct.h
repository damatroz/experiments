#pragma once

#include "Preprocessor.h"

#include <array>
#include <tuple>

//=============================================================================

#define RS_STRUCT_MEMBER(type_, member_, members_) \
    (struct, type_, member_, members_)

#define RS_STRUCT(type_, members_) PP_EVAL(RS_DETAIL_STRUCT(type_, members_))

//=============================================================================

#define RS_DETAIL_STRUCT(type_, members_) \
    struct type_ \
    { \
        RS_MEMBERS(members_) \
    }
#define RS_DETAIL_INDIRECT() RS_DETAIL_STRUCT

//=============================================================================

#define RS_MEMBERS(members_) \
    struct ReflectiveStruct{}; \
    RS_DETAIL_DEFINE_STRUCTS(members_) \
    RS_DETAIL_DEFINE_MEMBERS(members_) \
    RS_DETAIL_GET_MEMBERS(members_) \
    RS_DETAIL_GET_MEMBERS(members_, const) \
    RS_DETAIL_GET_NAMES(members_) \
    RS_DETAIL_GET_VALUES(members_) \
    RS_DETAIL_GET_VALUES(members_, const)

//=============================================================================

#define RS_DETAIL_IS_STRUCT_struct(x) x
#define RS_DETAIL_IS_STRUCT(x) \
    PP_IS_PAREN( \
        PP_IF(PP_IS_PAREN(x)) \
                    (0, \
                     PP_CAT(RS_DETAIL_IS_STRUCT_, x) (())))

#define RS_DETAIL_DEFINE_STRUCTS(members_) \
    PP_CAT(RS_DETAIL_DEFINE_STRUCTS_LOOP_A members_, _END)

#define RS_DETAIL_DEFINE_STRUCTS_LOOP_A(...) \
    RS_DETAIL_DEFINE_STRUCTS_BODY((__VA_ARGS__)) \
    RS_DETAIL_DEFINE_STRUCTS_LOOP_B
#define RS_DETAIL_DEFINE_STRUCTS_LOOP_B(...) \
    RS_DETAIL_DEFINE_STRUCTS_BODY((__VA_ARGS__)) \
    RS_DETAIL_DEFINE_STRUCTS_LOOP_A
#define RS_DETAIL_DEFINE_STRUCTS_LOOP_A_END
#define RS_DETAIL_DEFINE_STRUCTS_LOOP_B_END

#define RS_DETAIL_DEFINE_STRUCTS_BODY(...) \
    RS_DETAIL_DEFINE_STRUCTS_BODY_ __VA_ARGS__
#define RS_DETAIL_DEFINE_STRUCTS_BODY_(tag_, ...) \
    PP_WHEN(RS_DETAIL_IS_STRUCT(tag_)) \
                  (RS_DETAIL_DECLARE_STRUCT(__VA_ARGS__))

#define RS_DETAIL_DECLARE_STRUCT(type_, member_, members_) \
    PP_DEFER4(RS_DETAIL_INDIRECT)() (type_, members_);

//=============================================================================

#define RS_DETAIL_IDENTITY(...) __VA_ARGS__

#define RS_DETAIL_DEFINE_MEMBERS(members_) \
    PP_SEQ_CONCAT(PP_CAT( \
        RS_DETAIL_DEFINE_MEMBERS_LOOP_A members_, _END))

#define RS_DETAIL_DEFINE_MEMBERS_LOOP_A(...) \
    RS_DETAIL_DEFINE_MEMBERS_BODY((__VA_ARGS__)) \
    RS_DETAIL_DEFINE_MEMBERS_LOOP_B
#define RS_DETAIL_DEFINE_MEMBERS_LOOP_B(...) \
    RS_DETAIL_DEFINE_MEMBERS_BODY((__VA_ARGS__)) \
    RS_DETAIL_DEFINE_MEMBERS_LOOP_A
#define RS_DETAIL_DEFINE_MEMBERS_LOOP_A_END
#define RS_DETAIL_DEFINE_MEMBERS_LOOP_B_END

#define RS_DETAIL_DEFINE_MEMBERS_BODY(...) \
    RS_DETAIL_DEFINE_MEMBERS_BODY_ __VA_ARGS__
#define RS_DETAIL_DEFINE_MEMBERS_BODY_(tag_, ...) \
    PP_IF(RS_DETAIL_IS_STRUCT(tag_)) \
                (RS_DETAIL_DEFINE_MEMBERS_STRUCT, \
                 RS_DETAIL_DEFINE_MEMBERS_NORMAL) \
                    ((tag_, __VA_ARGS__))

#define RS_DETAIL_DEFINE_MEMBERS_STRUCT(...) \
    RS_DETAIL_DEFINE_MEMBERS_STRUCT_ __VA_ARGS__
#define RS_DETAIL_DEFINE_MEMBERS_STRUCT_(tag_, type_, mem_, ...) \
    (type_ mem_ = {};)

#define RS_DETAIL_DEFINE_MEMBERS_NORMAL(...) \
    RS_DETAIL_DEFINE_MEMBERS_NORMAL_ __VA_ARGS__
#define RS_DETAIL_DEFINE_MEMBERS_NORMAL_(...) \
    PP_SINGLE_EXPAND( \
        PP_CAT( \
            RS_DETAIL_DEFINE_MEMBERS_NORMAL_, \
            PP_VA_SIZE(__VA_ARGS__)) (__VA_ARGS__))

#define RS_DETAIL_DEFINE_MEMBERS_NORMAL_2(type_, member_) \
    (RS_DETAIL_IDENTITY(PP_REMOVE_PARENS(type_)) member_;)
#define RS_DETAIL_DEFINE_MEMBERS_NORMAL_3(type_, member_, init_) \
    (RS_DETAIL_IDENTITY(PP_REMOVE_PARENS(type_)) \
        member_ = PP_REMOVE_PARENS(init_);)

//=============================================================================

#define RS_DETAIL_MEMBER(tag_, ...) \
    PP_IF(RS_DETAIL_IS_STRUCT(tag_)) \
                (RS_DETAIL_MEMBER_STRUCT, \
                 RS_DETAIL_MEMBER_NORMAL)((tag_, __VA_ARGS__,))

#define RS_DETAIL_MEMBER_STRUCT(...) \
    RS_DETAIL_MEMBER_STRUCT_ __VA_ARGS__
#define RS_DETAIL_MEMBER_STRUCT_(tag_, type_, member_, ...) \
    member_

#define RS_DETAIL_MEMBER_NORMAL(...) \
    RS_DETAIL_MEMBER_NORMAL_ __VA_ARGS__
#define RS_DETAIL_MEMBER_NORMAL_(type_, member_, ...) member_

//=============================================================================

#define RS_DETAIL_GET_MEMBERS(members_, ...) \
auto GetMembers() __VA_ARGS__ \
{ \
    return std::make_tuple(RS_DETAIL_GET_MEMBERS_(members_)); \
}
#define RS_DETAIL_GET_MEMBERS_(members_) \
    PP_SEQ_ENUM( \
        PP_CAT(\
            RS_DETAIL_GET_MEMBERS_LOOP_A members_, _END))

#define RS_DETAIL_GET_MEMBERS_LOOP_A(...) \
    RS_DETAIL_GET_MEMBERS_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_MEMBERS_LOOP_B
#define RS_DETAIL_GET_MEMBERS_LOOP_B(...) \
    RS_DETAIL_GET_MEMBERS_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_MEMBERS_LOOP_A
#define RS_DETAIL_GET_MEMBERS_LOOP_A_END
#define RS_DETAIL_GET_MEMBERS_LOOP_B_END

#define RS_DETAIL_GET_MEMBERS_BODY(...) \
    RS_DETAIL_GET_MEMBERS_BODY_( \
        RS_DETAIL_MEMBER __VA_ARGS__)
#define RS_DETAIL_GET_MEMBERS_BODY_(member_) \
    RS_DETAIL_GET_MEMBERS_BODY__(member_)
#define RS_DETAIL_GET_MEMBERS_BODY__(member_) \
    (::ReflectiveStruct::Member(PP_STRINGIZE(member_), member_))

namespace ReflectiveStruct
{
template <typename T>
struct Member
{
    Member(const char* name_, T& value_) : name(name_), value(value_) {}

    const char* name;
    T& value;
};
}

//=============================================================================

#define RS_DETAIL_GET_NAMES(members_) \
auto GetNames() const \
{ \
    return std::array{RS_DETAIL_GET_NAMES_(members_)}; \
}
#define RS_DETAIL_GET_NAMES_(members_) \
    PP_SEQ_ENUM( \
        PP_CAT( \
            RS_DETAIL_GET_NAMES_LOOP_A members_, _END))

#define RS_DETAIL_GET_NAMES_LOOP_A(...) \
    RS_DETAIL_GET_NAMES_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_NAMES_LOOP_B
#define RS_DETAIL_GET_NAMES_LOOP_B(...) \
    RS_DETAIL_GET_NAMES_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_NAMES_LOOP_A
#define RS_DETAIL_GET_NAMES_LOOP_A_END
#define RS_DETAIL_GET_NAMES_LOOP_B_END

#define RS_DETAIL_GET_NAMES_BODY(...) \
    RS_DETAIL_GET_NAMES_BODY_(\
        RS_DETAIL_MEMBER __VA_ARGS__)
#define RS_DETAIL_GET_NAMES_BODY_(member_) \
    (RS_DETAIL_GET_NAMES_BODY__(member_))
#define RS_DETAIL_GET_NAMES_BODY__(member_) \
    PP_STRINGIZE(member_)

//=============================================================================

#define RS_DETAIL_GET_VALUES(members_, ...) \
auto GetValues() __VA_ARGS__ \
{ \
    return std::tie(RS_DETAIL_GET_VALUES_(members_)); \
}
#define RS_DETAIL_GET_VALUES_(members_) \
    PP_SEQ_ENUM( \
        PP_CAT( \
            RS_DETAIL_GET_VALUES_LOOP_A members_, _END))

#define RS_DETAIL_GET_VALUES_LOOP_A(...) \
    RS_DETAIL_GET_VALUES_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_VALUES_LOOP_B
#define RS_DETAIL_GET_VALUES_LOOP_B(...) \
    RS_DETAIL_GET_VALUES_BODY((__VA_ARGS__)) \
    RS_DETAIL_GET_VALUES_LOOP_A
#define RS_DETAIL_GET_VALUES_LOOP_A_END
#define RS_DETAIL_GET_VALUES_LOOP_B_END

#define RS_DETAIL_GET_VALUES_BODY(...) \
    RS_DETAIL_GET_VALUES_BODY_(\
        RS_DETAIL_MEMBER __VA_ARGS__)
#define RS_DETAIL_GET_VALUES_BODY_(member_) (member_)
