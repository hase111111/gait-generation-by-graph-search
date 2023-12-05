#pragma once

#include <iostream>
#include <string>


#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT
#include <toml.hpp>
#include <magic_enum.hpp>

//  ↓ tomlの　macroを使用しない場合，以下のコードは無効化される．
#ifndef TOML11_WITHOUT_DEFINE_NON_INTRUSIVE


struct Toml11Description final
{
	std::string description;
};


//// 一般的な関数テンプレート
//template <typename T>
//typename std::enable_if<!std::is_enum<T>::value>::type
//printValue(const T& value) {
//    std::cout << "General case: " << value << std::endl;
//}
//
//// enum型に対する特殊化
//template <typename T>
//typename std::enable_if<std::is_enum<T>::value>::type
//printValue(const T& value) {
//    std::cout << "Specialized for enum: " << static_cast<int>(value) << std::endl;
//}

#define DESIGNLAB_ADD_COMMENT(VARNAME)                              \
if (desc.VARNAME.description != "")                                 \
{                                                                   \
	v[#VARNAME].comments().push_back(desc.VARNAME.description);     \
}


#define DESIGNLAB_FIND_MEMBER_VARIABLE_FROM_VALUE(VAR_NAME)                         \
/*enumだった場合のみstd::string型に変換する*/                                             \
if constexpr (std::is_enum<decltype(obj.VAR_NAME)>::value) {                        \
    std::string str = toml::find<std::string>(v, TOML11_STRINGIZE(VAR_NAME));       \
    obj.VAR_NAME = magic_enum::enum_cast<decltype(obj.VAR_NAME)>(str).value();      \
}                                                                                   \
else {                                                                              \
obj.VAR_NAME = toml::find<decltype(obj.VAR_NAME)>(v, TOML11_STRINGIZE(VAR_NAME));   \
}


#define DESIGNLAB_ASSIGN_MEMBER_VARIABLE_TO_VALUE(VAR_NAME)\
if constexpr (std::is_enum<decltype(obj.VAR_NAME)>::value) {\
    /*enumだった場合のみstd::string型に変換する*/\
	std::string str = static_cast<std::string>(magic_enum::enum_name(obj.VAR_NAME));\
	v[TOML11_STRINGIZE(VAR_NAME)] = str;\
}\
else {\
    v[TOML11_STRINGIZE(VAR_NAME)] = obj.VAR_NAME;\
}


#define DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVE(NAME, DESCRIPTION, ...)            \
namespace toml {                                                                     \
                                                                                     \
struct NAME##_validly_checker final                                                  \
{                                                                                    \
    static_assert(std::is_class<NAME>::value, "The first argument must be a class or a structure." );\
	static_assert(std::is_default_constructible<NAME>::value, "The first argument must have a default constructor.");\
};                                                                                   \
                                                                                     \
template<>                                                                           \
struct from<NAME>                                                                    \
{                                                                                    \
    template<typename C, template<typename ...> class T,                             \
             template<typename ...> class A>                                         \
    static NAME from_toml(const basic_value<C, T, A>& v)                             \
    {                                                                                \
        NAME obj;                                                                    \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_FIND_MEMBER_VARIABLE_FROM_VALUE, __VA_ARGS__) \
        return obj;                                                                  \
    }                                                                                \
};                                                                                   \
                                                                                     \
template<>                                                                           \
struct into<NAME>                                                                    \
{                                                                                    \
    static value into_toml(const NAME& obj)                                          \
    {                                                                                \
        ::toml::basic_value<toml::preserve_comments> v = ::toml::table{};            \
        DESCRIPTION desc;															 \
        v.comments().push_back(desc.file_description.description);                   \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_ASSIGN_MEMBER_VARIABLE_TO_VALUE, __VA_ARGS__) \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_ADD_COMMENT, __VA_ARGS__)                  \
        return v;                                                                    \
    }                                                                                \
};                                                                                   \
                                                                                     \
} /* toml */


#endif	// TOML11_WITHOUT_DEFINE_NON_INTRUSIVE
