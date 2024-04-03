
//! @file      toml_serialize_macro.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.
//! @brief tomlファイルのシリアライズ/デシリアライズを行うためのマクロ．
//! @details
//! TOML11_DEFINE_CONVERSION_NON_INTRUSIVEをラッパしたもの．
//! もともとのほうでは enum型を取り扱うことができなかったが，
//! このマクロでは取り扱うことができる．
//! また，クラスの説明を追加することができる．

#ifndef DESIGNLAB_TOML_SERIALIZE_MACRO_H_
#define DESIGNLAB_TOML_SERIALIZE_MACRO_H_


#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <magic_enum.hpp>

#include "strconv2.h"

#include "implicit_metafunction_for_toml11.h"
#include "math_euler.h"
#include "math_quaternion.h"
#include "math_vector3.h"
#include "toml11_define.h"


//! @namespace designlab::toml_func
//! @brief tomlファイルのシリアライズ/デシリアライズを行うための関数群．
//! @details
//! ここで定義されている関数は，
//! tomlファイルのシリアライズ/デシリアライズを行うための関数である．
//! @n 他のファイルから呼び出すことを想定していないので，
//! このように奥まった名前空間に配置している．
//! @n C#の internal がC++にもあればこのような処理を書かなくともすむが，
//! マクロの仕様上，このような処理を書かなければならない．
namespace designlab::toml_func
{

//! @struct Toml11Description
//! @brief tomlファイルに追加する変数の説明を追加するための構造体．
struct Toml11Description final
{
    //! テーブルがない場合に指定する文字列．
    static const char kNoTable[];

    Toml11Description(const std::string& t, const std::string& d) :
        table_name(t),
        description(d)
    {
    }

    std::string table_name;  //!< テーブル名．
    std::string description;  //!< 説明，tomlファイルにはコメントとして追加される．
};


//! @brief 文字列のベクターをShift-jisからUTF-8に変換する．
//! @param str_vec 変換する文字列のベクター．
//! @return std::vector<std::string> 変換後の文字列のベクター．
std::vector<std::string> sjis_to_utf8_vec(const std::vector<std::string>& str_vec);

//! @brief tomlファイルに値を追加するための関数．
//! @n enum 型と vector3 型と euler_xyz 型以外の型に対応している．
//! @param v tomlファイルのデータ．
//! @param str 追加する変数の名前．
//! @param value 追加する値．
template <typename T>
typename std::enable_if<impl::is_toml11_available_type<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& str, const T& value)
{
    (*v)[str] = value;
}

//! @brief tomlファイルに値を追加するための関数．
//! @n enum 型に対応している．値を magic_enum で文字列に変換してから追加する．
//! @param v tomlファイルのデータ．
//! @param str 追加する変数の名前．
//! @param value 追加する値．
template <typename T>
typename std::enable_if<!impl::is_toml11_available_type<T>::value&& std::is_enum<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& str, const T& value)
{
    (*v)[str] = static_cast<std::string>(magic_enum::enum_name(value));
}

//! @brief tomlファイルに値を追加するための関数．
//! @n 出力ストリームを実装している型に対応している．
//! 値をストリームを用いて文字列に変換してから追加する．
//! @param v tomlファイルのデータ．
//! @param str 追加する変数の名前．
//! @param value 追加する値．
template <typename T>
typename std::enable_if<!impl::is_toml11_available_type<T>::value&& impl::has_output_operator<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& str, const T& value)
{
    std::stringstream ss;
    ss << value;
    (*v)[str] = ss.str();
}

template <typename T>
typename std::enable_if<!impl::is_toml11_available_type<T>::value&& impl::is_vector_of_has_output_operator<T>::value>::type
SetTomlValue(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& str, const T& value)
{
    std::vector<std::string> str_vec(value.size());

    for (int i = 0; i < value.size(); ++i)
    {
        std::stringstream ss;
        ss << value[i];
        str_vec[i] = ss.str();
    }

    (*v)[str] = str_vec;
}

//! プライマリ テンプレート．特殊化されない場合はコンパイルエラーになる．
template <typename T, typename Enable = void>
struct GetTomlValueImpl;

//! @brief tomlファイルから値を取得するための関数を
//! 特殊化するために暗黙的に呼ばれる構造体．
//! @tparam T toml11に対応している型．
//! @details 型の条件によって，Get関数を特殊化する．
//! @see GetTomlValue
template <typename T>
struct GetTomlValueImpl < T, typename std::enable_if<impl::is_toml11_available_type<T>::value> ::type >
{
    static T Get(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& var_str)
    {
        return toml::find<T>(*v, var_str);
    }
};

//! @brief プライマリ テンプレートの特殊化 : enum 型
//! @tparam T enum型．
//! @details enum型の値を magic_enumで文字列に変換してから取得する．
template <typename T>
struct GetTomlValueImpl<T, typename std::enable_if<!impl::is_toml11_available_type<T>::value&& std::is_enum<T>::value>::type>
{
    //! @brief tomlファイルから値を取得するための関数．
    //! @details enum型の値を magic_enumで文字列から変換して取得する．
    //! @param[in] v tomlファイルのデータ．
    //! @param[in] var_str 取得する変数の名前．
    //! @return T 取得した値．
    static T Get(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& var_str)
    {
        std::string str = toml::find<std::string>(*v, var_str);
        return magic_enum::enum_cast<T>(str).value();
    }
};

//! @brief プライマリ テンプレートの特殊化 : is_vector3 型と is_euler_xyz 型
//! @tparam T 入力オペレータを実装している型．
//! @details 値をストリームを用いて文字列に変換してから取得する．
template <typename T>
struct GetTomlValueImpl<T, typename std::enable_if<!impl::is_toml11_available_type<T>::value&& impl::has_input_operator<T>::value>::type>
{
    //! @brief tomlファイルから値を取得するための関数．
    //! @details ストリームを用いて文字列から変換して取得する．
    //! @param[in] v tomlファイルのデータ．
    //! @param[in] var_str 取得する変数の名前．
    //! @return T 取得した値．
    static T Get(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& var_str)
    {
        std::string str = toml::find<std::string>(*v, var_str);
        std::stringstream ss;
        ss << str;
        T temp{};
        ss >> temp;
        return temp;
    }
};

template <typename T>
struct GetTomlValueImpl<T, typename std::enable_if<!impl::is_toml11_available_type<T>::value&& impl::is_vector_of_has_input_operator<T>::value>::type>
{
    static T Get(::toml::basic_value<toml::preserve_comments, std::map>* v, const std::string& var_str)
    {
        std::vector<std::string> str_vec = toml::find<std::vector<std::string>>(*v, var_str);

        T temp{};

        temp.resize(str_vec.size());

        for (int i = 0; i < str_vec.size(); ++i)
        {
            std::stringstream ss;
            ss << str_vec[i];
            ss >> temp[i];
        }

        return temp;
    }
};

//! @brief ユーザーが直接呼ぶ関数．
//! GetTomlValueImpl を利用してテンプレートの型を解決し，
//! それに応じたGet関数を呼び出す．
//! @tparam T 取得する値の型．
//! @param[in] v tomlファイルのデータ．
//! @param[in] var_str 取得する変数の名前．
//! @return 取得した値．
template <typename T>
T GetTomlValue(::toml::basic_value<toml::preserve_comments, std::map> v, const std::string& var_str)
{
    return GetTomlValueImpl<T>::Get(&v, var_str);
}

}  // namespace designlab::toml_func


//! @def DESIGNLAB_SUB_MACRO_FIND_MEMBER_VARIABLE_FROM_VALUE
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! 他のファイルから呼び出さないこと．
//! @n tomlファイルからクラスのメンバ変数を取得する．
//! @param VAR_NAME 変数名．
#define DESIGNLAB_SUB_MACRO_FIND_MEMBER_VARIABLE_FROM_VALUE(VAR_NAME)      \
{                                                                          \
    const std::string table_str = desc.VAR_NAME.table_name;                \
                                                                           \
    if (table_str == ::designlab::toml_func::Toml11Description::kNoTable)  \
    {                                                                      \
        obj.VAR_NAME =                                                     \
            ::designlab::toml_func::GetTomlValue<decltype(obj.VAR_NAME)>(  \
                v_,                                                        \
                TOML11_STRINGIZE(VAR_NAME));                               \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        obj.VAR_NAME =                                                     \
            ::designlab::toml_func::GetTomlValue<decltype(obj.VAR_NAME)>(  \
                v_[table_str],                                             \
                TOML11_STRINGIZE(VAR_NAME));                               \
    }                                                                      \
}



//! @def DESIGNLAB_SUB_MACRO_ASSIGN_MEMBER_VARIABLE_TO_VALUE
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! 他のファイルから呼び出さないこと．
//! @n クラスのメンバ変数を tomlファイルに追加する．
//! @param VAR 変数名．
#define DESIGNLAB_SUB_MACRO_ASSIGN_MEMBER_VARIABLE_TO_VALUE(VAR)                  \
if (desc.VAR.table_name != ::designlab::toml_func::Toml11Description::kNoTable) { \
    if (v.count(desc.VAR.table_name) == 0)                                        \
    {                                                                             \
        v[desc.VAR.table_name] = toml::table{};                                   \
    }                                                                             \
                                                                                  \
    ::designlab::toml_func::SetTomlValue(&v[desc.VAR.table_name],                 \
        TOML11_STRINGIZE(VAR), obj.VAR);                                          \
}                                                                                 \
else                                                                              \
{                                                                                 \
    ::designlab::toml_func::SetTomlValue(                                         \
        &v, TOML11_STRINGIZE(VAR), obj.VAR);                                      \
}


//! @def DESIGNLAB_SUB_MACRO_ADD_COMMENT
//! @brief DESIGNLAB_DEFINE_CONVERSION_NON_INTRUSIVEの補助マクロ．
//! 他のファイルから呼び出さないこと．
//! @n tomlファイルの要素にクラスの説明を追加する．
//! もし，説明の用意がなければ説明を追加しない．
//! @param VN 変数名．
#define DESIGNLAB_SUB_MACRO_ADD_COMMENT(VN)                                        \
if (desc.VN.description != "") {                                                   \
    if (desc.VN.table_name != ::designlab::toml_func::Toml11Description::kNoTable) \
    {                                                                              \
        v[desc.VN.table_name][#VN].comments().push_back(desc.VN.description);      \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        v[#VN].comments().push_back(desc.VN.description);                          \
    }                                                                              \
}


//! @def DESIGNLAB_TOML11_DESCRIPTION_CLASS
//! @brief tomlファイルに説明を追加するためのクラスの宣言を行うためのマクロ．
//! @param CLASS クラス名．
#define DESIGNLAB_TOML11_DESCRIPTION_CLASS(CLASS) \
struct CLASS##Description final


//! @def DESIGNLAB_TOML11_FILE_NO_DESCRIPTION
//! @brief tomlファイルにファイルの説明を追加しないことを示す文字列．
//! @n DESIGNLAB_TOML11_DESCRIPTION_CLASS内に必ず記述する必要がある．
#define DESIGNLAB_TOML11_FILE_NO_DESCRIPTION() \
const ::std::vector<::std::string> file_description_vec{};

//! @def DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION
//! @brief tomlファイルにファイルの説明を追加するためのマクロ．
//! @n DESIGNLAB_TOML11_DESCRIPTION_CLASS内に必ず記述する必要がある．
//! @param DESCRIPTION 説明．文字列で指定する．
#define DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION(DESCRIPTION) \
const ::std::vector<::std::string> file_description_vec{sjis_to_utf8(DESCRIPTION)};

//! @def DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION_MULTI_LINE
//! @brief tomlファイルにファイルの説明を追加するためのマクロ．
//! @n DESIGNLAB_TOML11_DESCRIPTION_CLASS内に必ず記述する必要がある．
//! @param DESCRIPTION_VEC 説明．文字列の vector で指定する．
#define DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION_MULTI_LINE(DESCRIPTION_VEC) \
const ::std::vector<::std::string> file_description_vec = \
    ::designlab::toml_func::sjis_to_utf8_vec(DESCRIPTION_VEC);


//! @def DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION
//! @brief tomlファイルにテーブルの説明を追加するためのマクロ．
//! @n DESIGNLAB_TOML11_DESCRIPTION_CLASS内に必ず記述する必要がある．
//! @param ... テーブル名と説明．
#define DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION(...) \
const std::vector<std::string> table_name_description_vec = \
    ::designlab::toml_func::sjis_to_utf8_vec({ __VA_ARGS__ });

//! @def DESIGNLAB_TOML11_TABLE_NO_DESCRIPTION
//! @brief tomlファイルに追加するテーブルにコメントを追加しないことを示すマクロ．
//! @n DESIGNLAB_TOML11_DESCRIPTION_CLASS内に必ず記述する必要がある．
#define DESIGNLAB_TOML11_TABLE_NO_DESCRIPTION() \
const std::vector<std::string> table_name_description_vec = {};


//! @def DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION
//! @brief tomlファイルに変数とファイルの説明を追加するためのマクロ．
//! @param VARIABLE 変数名．
//! @param TABLE テーブル名．
//! @param DESCRIPTION 説明．
#define DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(VARIABLE, TABLE, DESCRIPTION) \
const ::designlab::toml_func::Toml11Description VARIABLE{                       \
    TABLE, sjis_to_utf8(DESCRIPTION)                                            \
}

//! @def DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION
//! @brief ファイルの説明を追加したくない場合には，このマクロで変数を追加する．
//! @param VARIABLE 変数名．
//! @param TABLE テーブル名．
#define DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(VARIABLE, TABLE) \
const ::designlab::toml_func::Toml11Description VARIABLE{TABLE, ""}

//! @def DESIGNLAB_TOML11_NO_TABLE
//! @brief tomlファイルに追加する変数をテーブルに追加しないことを示すためのマクロ．
#define DESIGNLAB_TOML11_NO_TABLE \
::designlab::toml_func::Toml11Description::kNoTable


//! @def DESIGNLAB_TOML11_SERIALIZE
//! @brief tomlファイルのシリアライズ/デシリアライズを行うためのマクロ．
//! @n TOML11_DEFINE_CONVERSION_NON_INTRUSIVEをラッパしたもの．
//! @n もともとのほうでは enum型を取り扱うことができなかったが，
//! このマクロでは取り扱うことができる．
//! @n また，クラスの説明を追加することができる．
//! @n 注意点として，このクラスを使用する場合は，
//! 必ずDESIGNLAB_TOML11_DESCRIPTION_CLASSを用意する必要がある．
//! @details 以下のように使用する．
//! @code
//! enum class SampleEnum : int
//! {
//!     A,
//!     B,
//!     C,
//! }
//!
//! class Sample final
//! {
//! public:
//!     int data{ 0 };
//!     std::string str{ "This is String"};
//!     SampleEnum enum_data{ SampleEnum::A };
//! };
//!
//! DESIGNLAB_TOML11_DESCRIPTION_CLASS(Sample)
//! {
//!     DESIGNLAB_TOML11_FILE_NO_DESCRIPTION();
//!
//!     DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION(
//!     "number", "This is Number Table."
//!     "enum", "This is Enum Table.");
//!
//!     DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(data, "number", "This is data");
//!     DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
//!         str, DESIGNLAB_TOML11_NO_TABLE, "This is str");
//!     DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
//!         enum_data, "enum", "This is enum_data");
//! };
//!
//! DESIGNLAB_TOML11_SERIALIZE(Sample, data, str, enum_data);
//! @endcode
//! @param NAME クラス名．クラスの型を指定する．
//! @param ... クラスのメンバ変数．過不足なく，全て指定する必要がある．
//! 可変長引数なので複数指定することができる．
#define DESIGNLAB_TOML11_SERIALIZE(NAME, ...)                                   \
namespace toml                                                                  \
{                                                                               \
template<>                                                                      \
struct from<NAME>                                                               \
{                                                                               \
  static_assert(std::is_class<NAME>::value,                                     \
    "第1引数はクラスか構造体である必要があります．");                           \
  static_assert(std::is_default_constructible<NAME>::value,                     \
    "第1引数はデフォルトコンストラクタを持つ必要があります．");                 \
                                                                                \
  template<typename C, template<typename ...> class T,                          \
       template<typename ...> class A>                                          \
  static NAME from_toml(basic_value<C, T, A>& v)                                \
  {                                                                             \
    ::toml::basic_value<toml::preserve_comments, std::map> v_ = v;              \
    NAME obj;                                                                   \
    NAME##Description desc;                                                     \
    TOML11_FOR_EACH_VA_ARGS(                                                    \
        DESIGNLAB_SUB_MACRO_FIND_MEMBER_VARIABLE_FROM_VALUE, __VA_ARGS__)       \
    return obj;                                                                 \
  }                                                                             \
};                                                                              \
                                                                                \
template<>                                                                      \
struct into<NAME>                                                               \
{                                                                               \
  static value into_toml(const NAME& obj)                                       \
  {                                                                             \
    ::toml::basic_value<toml::preserve_comments, std::map> v = ::toml::table{}; \
                                                                                \
    NAME##Description desc;                                                     \
                                                                                \
    for (const auto i : desc.file_description_vec)                              \
    {                                                                           \
      v.comments().push_back(i);                                                \
    }                                                                           \
                                                                                \
    for (int i = 0; i < desc.table_name_description_vec.size(); ++i)            \
    {                                                                           \
      v[desc.table_name_description_vec[i]] = ::toml::table{};                  \
      v[desc.table_name_description_vec[i]].comments().                         \
        push_back(desc.table_name_description_vec[i + 1]);                      \
      ++i;                                                                      \
    }                                                                           \
                                                                                \
    TOML11_FOR_EACH_VA_ARGS(                                                    \
        DESIGNLAB_SUB_MACRO_ASSIGN_MEMBER_VARIABLE_TO_VALUE, __VA_ARGS__)       \
    TOML11_FOR_EACH_VA_ARGS(                                                    \
        DESIGNLAB_SUB_MACRO_ADD_COMMENT, __VA_ARGS__)                           \
    return v;                                                                   \
  }                                                                             \
};                                                                              \
                                                                                \
}  // namespace toml


#endif  // DESIGNLAB_TOML_SERIALIZE_MACRO_H_
