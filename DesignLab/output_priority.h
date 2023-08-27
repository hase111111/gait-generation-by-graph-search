#pragma once

#include <string>



//! @enum EOutputPriority
//! @date 2023/08/27
//! @author 長谷川
//! @brief コマンドラインに文字を出力する際の優先度．
enum class EOutputPriority
{
	SYSTEM = 0,		//!< 常に出力する
	ERROR_MES = 1,		//!< エラー出力
	WARNING = 2,	//!< 警告出力
	INFO = 3,		//!< 優先度低めの情報
	DEBUG = 4,		//!< デバッグ時のみ出力
};


namespace std
{

	//! @brief EOutputPriorityを文字列に変換する
	//! @param[in] priority EOutputPriority
	//! @return EOutputPriorityを文字列に変換したもの
	std::string to_string(EOutputPriority priority);


	//! @fn EOutputPriority toOutputPriority(const std::string& str)
	//! @brief 文字列をEOutputPriorityに変換する
	//! @param[in] str 文字列
	//! @return EOutputPriority 
	EOutputPriority toOutputPriority(const std::string& str);

} // namespace std




//! @file output_priority.h
//! @date 2023/08/27
//! @author 長谷川
//! @brief コマンドラインに文字を出力する際の優先度．
//! @n 行数 : @lineinfo
