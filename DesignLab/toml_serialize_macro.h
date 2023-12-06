//! @file toml_serialize_macro.h
//! @brief tomlファイルのシリアライズ/デシリアライズを行うためのマクロ．

#include <iostream>
#include <map>
#include <string>

#include <magic_enum.hpp>

#include "toml11_define.h"


struct Toml11Description final
{
	//! テーブルがない場合に指定する文字列
	static const std::string NO_TABLE;

	Toml11Description(const std::string& t, const std::string& d) : table_name(t), description(d) {}

	std::string table_name;
	std::string description;
};


//! @breif tomlファイルに値を追加するための関数．
//! @n enumに関して特殊化されており，enum型の値を文字列に変換してから追加する．
template <typename T>
typename std::enable_if<!std::is_enum<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>& v, const std::string& str, const T& value)
{
	v[str] = value;
}

template <typename T>
typename std::enable_if<std::is_enum<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>& v, const std::string& str, const T& value)
{
	v[str] = static_cast<std::string>(magic_enum::enum_name(value));
}


//! @def DESIGNLAB_FIND_MEMBER_VARIABLE_FROM_VALUE
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! @n tomlファイルからクラスのメンバ変数を取得する．
#define DESIGNLAB_FIND_MEMBER_VARIABLE_FROM_VALUE(VAR_NAME)                                         \
if constexpr (std::is_enum<decltype(obj.VAR_NAME)>::value)                                          \
{                                                                                                   \
    const std::string table_str = desc.VAR_NAME.table_name;                                         \
                                                                                                    \
    if(table_str != Toml11Description::NO_TABLE)                                                    \
    {                                                                                               \
        std::string str = toml::find<std::string>(v, TOML11_STRINGIZE(VAR_NAME));                   \
        obj.VAR_NAME = magic_enum::enum_cast<decltype(obj.VAR_NAME)>(str).value();                  \
    }                                                                                               \
	else                                                                                            \
	{                                                                                               \
		std::string str = toml::find<std::string>(v[table_str], TOML11_STRINGIZE(VAR_NAME));        \
		obj.VAR_NAME = magic_enum::enum_cast<decltype(obj.VAR_NAME)>(str).value();                  \
	}                                                                                               \
}                                                                                                   \
else                                                                                                \
{                                                                                                   \
    const std::string table_str = desc.VAR_NAME.table_name;                                         \
																								    \
	if(table_str != Toml11Description::NO_TABLE)                                                    \
    {																						        \
        obj.VAR_NAME = toml::find<decltype(obj.VAR_NAME)>(v, TOML11_STRINGIZE(VAR_NAME));           \
	}                                                                                               \
	else                                                                                            \
	{                                                                                               \
		obj.VAR_NAME = toml::find<decltype(obj.VAR_NAME)>(v[table_str], TOML11_STRINGIZE(VAR_NAME));\
	}                                                                                               \
}


//! @def DESIGNLAB_ASSIGN_MEMBER_VARIABLE_TO_VALUE
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! @n クラスのメンバ変数をtomlファイルに追加する．
#define DESIGNLAB_ASSIGN_MEMBER_VARIABLE_TO_VALUE(VAR_NAME)                             \
if(desc.VAR_NAME.table_name != Toml11Description::NO_TABLE)                             \
{                                                                                       \
    if(v.count(desc.VAR_NAME.table_name) == 0)                                          \
    {													                                \
		v[desc.VAR_NAME.table_name] = toml::table{};                                    \
	}                                                                                   \
																						\
   	SetTomlValue(v[desc.VAR_NAME.table_name], TOML11_STRINGIZE(VAR_NAME), obj.VAR_NAME);\
}                                                                                       \
else                                                                                    \
{                                                                                       \
    SetTomlValue(v, TOML11_STRINGIZE(VAR_NAME), obj.VAR_NAME);                          \
}


//! @def DESIGNLAB_ADD_COMMENT
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! @n tomlファイルの要素にクラスの説明を追加する．
#define DESIGNLAB_ADD_COMMENT(VAR_NAME)                                 \
if (desc.VAR_NAME.description != "")                                    \
{                                                                       \
    if(desc.VAR_NAME.table_name != Toml11Description::NO_TABLE)         \
    {                                                                   \
    	v[desc.VAR_NAME.table_name][#VAR_NAME].comments().              \
            push_back(desc.VAR_NAME.description);                       \
    }                                                                   \
    else                                                                \
    {                                                                   \
    	v[#VAR_NAME].comments().push_back(desc.VAR_NAME.description);   \
    }                                                                   \
}


//! @def DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVE
//! @brief tomlファイルのシリアライズ/デシリアライズを行うためのマクロ．
//! @n TOML11_DEFINE_CONVERSION_NON_INTRUSIVEをラッパしたもの．
//! @n もともとのほうではenum型を取り扱うことができなかったが，このマクロでは取り扱うことができる．
//! @n また，クラスの説明を追加することができる．
//! @param NAME クラス名．クラスの型を指定する．
//! @param DESCRIPTION クラスの説明．
//! @param ... クラスのメンバ変数．過不足なく，全て指定する必要がある．可変長引数なので複数指定することができる．
#define DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVE(NAME, DESCRIPTION, ...)               \
namespace toml                                                                          \
{                                                                                       \
template<>                                                                              \
struct from<NAME>                                                                       \
{                                                                                       \
    static_assert(std::is_class<NAME>::value,                                           \
        "第1引数はクラスか構造体である必要があります．" );                              \
	static_assert(std::is_default_constructible<NAME>::value,                           \
        "第1引数はデフォルトコンストラクタを持つ必要があります．");                     \
                                                                                        \
    template<typename C, template<typename ...> class T,                                \
             template<typename ...> class A>                                            \
    static NAME from_toml(const basic_value<C, T, A>& v)                                \
    {                                                                                   \
        NAME obj;                                                                       \
        DESCRIPTION desc;															    \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_FIND_MEMBER_VARIABLE_FROM_VALUE, __VA_ARGS__) \
        return obj;                                                                     \
    }                                                                                   \
};                                                                                      \
                                                                                        \
template<>                                                                              \
struct into<NAME>                                                                       \
{                                                                                       \
    static value into_toml(const NAME& obj)                                             \
    {                                                                                   \
        ::toml::basic_value<toml::preserve_comments, std::map> v = ::toml::table{};     \
        DESCRIPTION desc;															    \
        v.comments().push_back(desc.file_description);                                  \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_ASSIGN_MEMBER_VARIABLE_TO_VALUE, __VA_ARGS__) \
        TOML11_FOR_EACH_VA_ARGS(DESIGNLAB_ADD_COMMENT, __VA_ARGS__)                     \
        return v;                                                                       \
    }                                                                                   \
};                                                                                      \
                                                                                        \
} /* namesppppac toml */