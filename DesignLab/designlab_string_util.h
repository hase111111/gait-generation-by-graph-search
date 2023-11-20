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
		//! @brief 文字列を分割する関数．
		//! @param [in] str 分割する文字列．
		//! @param [in] delim 分割する文字．
		//! @return std::vector<std::string> 分割した文字列の配列．
		std::vector<std::string> Split(const std::string& str, const char delim);

		//! @brief 文字列を分割する関数．
		//! @param [in] str 分割する文字列．
		//! @param [in] delim 分割する文字．2文字以上の文字列を指定できない．
		//! @return std::vector<std::string> 分割した文字列．
		std::vector<std::string> Split(const std::string& str, const std::string& delim);

		//! @brief enumを文字列に変換する関数．
		//! @n Google C++ coding style だとenumの要素は 先頭にkをつけてキャメルケースで書くことが推奨されている．
		//! @n 例えば enum class Color { kRed, kGreen, kBlue } と書く．
		//! @n このため，この関数はそのkを除去する機能を提供し，Color::kRed を渡すと "Red" という文字列を返す．
		//! @param [in] enum_value enumの要素．
		//! @return std::string enumの要素を文字列にしたもの．
		//! @tparam T enum型．
		template <typename T>
		std::string MyEnumToString( const T& enum_value )
		{
			//型チェック Tがenum型であることをチェックする．
			static_assert( std::is_enum<T>::value, "引数はenum，あるいはenum classである必要があります．" );

			std::string str = static_cast<std::string>(magic_enum::enum_name(enum_value));

			if ( str.size() > 0 && str[0] == 'k' )
			{
				// 先頭のkを削除する．
				str.erase( 0, 1 );
			}

			return str;
		}

	}	// namespace string_util

}	// namespace designlab


#endif	// DESIGNLAB_STRING_UTIL_H_
