//! @file output_detail.h
//! @brief コマンドラインに文字を出力する際の詳細をまとめた列挙体．

#ifndef DESIGNLAB_OUTPUT_DETAIL_H_
#define DESIGNLAB_OUTPUT_DETAIL_H_


//! @enum OutputDetail
//! @brief コマンドラインに文字を出力する際の詳細．
//! @details Settingファイルで指定された優先度以上のメッセージのみ出力する．
enum class OutputDetail
{
	kSystem = 0,	//!< システムメッセージ，常に出力する．
	kError,			//!< エラーメッセージ．
	kWarning,		//!< 警告メッセージ，エラーではないが注意が必要なメッセージ．
	kInfo,			//!< 優先度低めの情報．
	kDebug,			//!< デバッグ時のみ出力，一番優先度が低い．
};


#endif	// DESIGNLAB_OUTPUT_DETAIL_H_