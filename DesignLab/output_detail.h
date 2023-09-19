//! @file output_detail.h
//! @brief コマンドラインに文字を出力する際の詳細．

#ifndef DESIGNLAB_OUTPUT_DETAIL_H_
#define DESIGNLAB_OUTPUT_DETAIL_H_

#include <string>


//! @enum OutputDetail
//! @brief コマンドラインに文字を出力する際の詳細．
//! @details Settingファイルで指定された優先度以上のメッセージのみ出力する．

enum class OutputDetail
{
	kSystem = 0,	//!< システムメッセージ，常に出力する
	kError,			//!< エラーメッセージ
	kWarning,		//!< 警告メッセージ，エラーではないが注意が必要なメッセージ
	kInfo,			//!< 優先度低めの情報
	kDebug,			//!< デバッグ時のみ出力，一番優先度が低い
};


namespace std
{

	//! @brief EOutputPriorityを文字列に変換する
	//! @param[in] detail OutputDetail
	//! @return EOutputPriorityを文字列に変換したもの
	std::string to_string(OutputDetail detail);


	//! @fn OutputDetail toOutputPriority(const std::string& str)
	//! @brief 文字列をEOutputPriorityに変換する
	//! @param[in] str 文字列
	//! @return OutputDetail 
	OutputDetail toOutputPriority(const std::string& str);

} // namespace std


#endif	// DESIGNLAB_OUTPUT_DETAIL_H_