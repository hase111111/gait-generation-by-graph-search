
//! @file      string_util.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_STRING_UTIL_H_
#define DESIGNLAB_STRING_UTIL_H_

#include <string>
#include <vector>

#include <magic_enum.hpp>


namespace designlab::string_util
{

//! @brief 文字列を分割する関数．指定した文字で文字列を分割する．
//! @n 分割した結果，空白が含まれる場合や文字列がない場合は，そのまま返す．
//! @n 最後が区切り文字で終わる場合は，それを無視する．
//! @param[in] str 分割する文字列．
//! @param[in] delim 区切り文字，あるいは文字列．
//! @return 分割した文字列．
std::vector<std::string> Split(const std::string& str, const std::string& delim);

//! @brief enumを文字列に変換する関数．
//! @n Google C++ coding style だと enumの要素は
//! 先頭にkをつけてキャメルケースで書くことが推奨されている．
//! @n 例えば，
//! @code
//! enum class Color
//! {
//!     kRed,
//!     kGreen,
//!     kBlue
//! }
//! @endcode
//! @n と書く．
//! @n このため，この関数はその先頭のkを除去する機能を提供し，
//! Color::kRed を渡すと "Red" という文字列を返す．
//! @subsubsection 参考
//! @li https://marycore.jp/prog/cpp/std-string-split/ (アクセス日 2023/12/26)
//! @li https://marycore.jp/prog/cpp/std-string-find-search/#find%EF%BC%8Frfind
//! (アクセス日 2023/12/26)
//!
//! @param[in] enum_value enumの要素．
//! @return enumの要素を文字列にしたもの．
//! @tparam T enum型．
template <typename T>
std::string EnumToStringRemoveTopK(const T& enum_value)
{
    // 型チェック Tが enum型であることをチェックする．
    static_assert(std::is_enum<T>::value, "引数は enum，あるいは enum classである必要があります．");

    std::string str = static_cast<std::string>(magic_enum::enum_name(enum_value));

    if (str.size() > 0 && str[0] == 'k')
    {
        // 先頭のkを削除する．
        str.erase(0, 1);
    }

    return str;
}

//! @brief enum型を渡すと，その要素を列挙した文字列を返す関数．
//! @param[in] separator 列挙した文字列の区切り文字．
//! @return enumの要素を列挙した文字列．
//! @tparam T enum型．
template <typename T>
std::string EnumValuesToString(const std::string separator)
{
    // 型チェック Tが enum型であることをチェックする．
    static_assert(std::is_enum<T>::value, "引数は enum，あるいは enum classである必要があります．");

    std::string str;
    bool is_first = true;

    for (const auto& e : magic_enum::enum_values<T>())
    {
        if (!is_first)
        {
            str += separator;
        }
        else
        {
            is_first = false;
        }

        str += static_cast<std::string>(magic_enum::enum_name(e));
    }

    return str;
}


//! @brief enum型を渡すと，その要素と値を変換したものを列挙した文字列を返す関数．
//! @param[in] separator 列挙した文字列の区切り文字．
//! @return enumの要素と値を変換したものを列挙した文字列．
//! @tparam T enum型．
template <typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
std::string EnumEntriesToString(const std::string separator)
{
    std::string str;
    using enum_type = typename std::underlying_type<T>::type;
    bool is_first = true;

    for (const auto& e : magic_enum::enum_values<T>())
    {
        if (!is_first)
        {
            str += separator;
        }
        else
        {
            is_first = false;
        }

        str += static_cast<std::string>(magic_enum::enum_name(e));
        str += " = ";
        str += std::to_string(static_cast<enum_type>(e));
    }

    return str;
}


template <typename T>
std::string GetTypeName(const T& type)
{
    std::string str = typeid(type).name();

    std::vector<std::string> eliminate
    {
      "class ", "struct ", "designlab::", ",void"
    };

    for (const auto& e : eliminate)
    {
        while (true)
        {
            auto pos = str.find(e);

            if (pos == std::string::npos)
            {
                break;
            }

            str.erase(pos, e.size());
        }
    }
    return str;
}

}  // namespace designlab::string_util


#endif  // DESIGNLAB_STRING_UTIL_H_
