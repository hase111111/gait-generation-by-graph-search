//! application_setting_record_vaildator.h
//! @brief 設定ファイルの内容を検証するクラス．


#ifndef APPLICATION_SETTING_RECORD_VAILDATOR_H_
#define APPLICATION_SETTING_RECORD_VAILDATOR_H_

#include <string>
#include <tuple>

#include "application_setting_record.h"


//! @class ApplicationSettingRecordVaildator
//! @brief 設定ファイルの内容を検証するクラス．
//! @details vaildator (バリデータ) は検証する人のこと．
//! @n バリデータとは，バリデーションを行う機能，またはソフトウェアのことである．
//! @n バリデータでは，入力されたデータが仕様にそって適切に記述されているかを判断し，不適切な箇所があった場合にはエラーとして通知する．
class ApplicationSettingRecordVaildator final
{
public:

	ApplicationSettingRecordVaildator();

	//! @brief 設定ファイルの内容を検証する．
	//! @param[in] setting_record 設定ファイルの内容．
	//! @return std::tuple<bool, std::string> 検証結果．1つ目の要素がtrueならば検証成功．2つ目の要素が検証失敗の理由．
	std::tuple<bool, std::string> Vaildate(const ApplicationSettingRecord& setting_record);

private:

	//gt = greater than． le = less equal．

	const std::string kErrorMesForWindowWidthGtZero;
	const std::string kErrorMesForWindowWidthLeMax;
	const std::string kErrorMesForWindowHeightGtZero;
	const std::string kErrorMesForWindowHeightLeMax;
	const std::string kErrorMesForFpsGtZero;
	const std::string kErrorMesForFpsLeMax;
};


#endif // APPLICATION_SETTING_RECORD_VAILDATOR_H_