//! @file designlab_string_util.h
//! @brief 文字列を扱う関数をまとめた名前空間．


#ifndef	DESIGNLAB_STRING_UTIL_H_
#define	DESIGNLAB_STRING_UTIL_H_


#include <string>
#include <vector>

#include <magic_enum.hpp>


namespace designlab
{
	namespace string_util
	{
		//! @brief 文字列を分割する関数．指定した文字で文字列を分割する．
		//! @n 分割した結果，空文字列が含まれる場合は，空文字列を含めて返す．
		//! @n 最後が区切り文字で終わる場合は，それを無視して返す．
		//! @param [in] str 分割する文字列．
		//! @param [in] delim 分割する文字，あるいは文字列．
		//! @return std::vector<std::string> 分割した文字列．
		std::vector<std::string> Split(const std::string& str, const std::string& delim);

		//! @brief enumを文字列に変換する関数．
		//! @n Google C++ coding style だとenumの要素は 先頭にkをつけてキャメルケースで書くことが推奨されている．
		//! @n 例えば，
		//! @code 
		//! @n enum class Color 
		//! @n {
		//! @n	kRed,
		//! @n	kGreen,
		//! @n	kBlue 
		//! @n }
		//! @endcode
		//! @n と書く．
		//! @n このため，この関数はその先頭のkを除去する機能を提供し，Color::kRed を渡すと "Red" という文字列を返す．
		//! @param [in] enum_value enumの要素．
		//! @return std::string enumの要素を文字列にしたもの．
		//! @tparam T enum型．
		template <typename T>
		std::string EnumToStringRemoveTopK(const T& enum_value)
		{
			//型チェック Tがenum型であることをチェックする．
			static_assert(std::is_enum<T>::value, "引数はenum，あるいはenum classである必要があります．");

			std::string str = static_cast<std::string>(magic_enum::enum_name(enum_value));

			if (str.size() > 0 && str[0] == 'k')
			{
				// 先頭のkを削除する．
				str.erase(0, 1);
			}

			return str;
		}

		//! @brief enum型を渡すと，その要素を列挙した文字列を返す関数．
		//! @param [in] separator 列挙した文字列の区切り文字．
		//! @return std::string enumの要素を列挙した文字列．
		//! @tparam T enum型．
		template <typename T>
		std::string EnumValuesToString(const std::string separator)
		{
			//型チェック Tがenum型であることをチェックする．
			static_assert(std::is_enum<T>::value, "引数はenum，あるいはenum classである必要があります．");

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

	}	// namespace string_util

}	// namespace designlab


#endif	// DESIGNLAB_STRING_UTIL_H_
