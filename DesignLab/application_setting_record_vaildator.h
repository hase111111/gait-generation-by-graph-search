//! @file application_setting_record_vaildator.h
//! @brief ApplicationSettingRecord のバリデータ．

#ifndef APPLICATION_SETTING_RECORD_VAILDATOR_H_
#define APPLICATION_SETTING_RECORD_VAILDATOR_H_

#include <string>
#include <tuple>

#include "application_setting_record.h"
#include "interface_toml_data_validator.h"


namespace designlab
{

//! @class ApplicationSettingRecordVaildator
//! @brief ApplicationSettingRecord のバリデータ．
//! @details vaildator (バリデータ) は検証する人のこと．
//! @n バリデータとは，バリデーションを行う機能，またはソフトウェアのことである．
//! @n バリデータでは，入力されたデータが仕様にそって適切に記述されているかを判断し，不適切な箇所があった場合にはエラーとして通知する．
class ApplicationSettingRecordVaildator final : public ITomlDataValidator<ApplicationSettingRecord>
{
public:

	ApplicationSettingRecordVaildator();

	//! @brief 設定ファイルの内容を検証する．
	//! @param[in] setting_record 設定ファイルの内容．
	//! @return std::tuple<bool, std::string> 検証結果．1つ目の要素がtrueならば検証成功．2つ目の要素が検証失敗の理由．
	std::tuple<bool, std::string> Validate(const ApplicationSettingRecord& setting_record) const override;

private:

	//ge = greater equal． le = less equal．

	const std::string kErrorMesForWindowWidthGeMin;
	const std::string kErrorMesForWindowWidthLeMax;
	const std::string kErrorMesForWindowHeightGeMin;
	const std::string kErrorMesForWindowHeightLeMax;
	const std::string kErrorMesForFpsGeMin;
	const std::string kErrorMesForFpsLeMax;
};

} // namespace designlab


#endif // APPLICATION_SETTING_RECORD_VAILDATOR_H_