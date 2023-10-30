//! @file designlab_string_util.h
//! @brief 文字列を扱う関数をまとめた名前空間．


#ifndef	DESIGNLAB_STRING_UTIL_H_
#define	DESIGNLAB_STRING_UTIL_H_


#include <string>
#include <vector>


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

	}	// namespace string_util

}	// namespace designlab


#endif	