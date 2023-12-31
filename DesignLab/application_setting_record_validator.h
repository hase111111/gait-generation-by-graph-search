
//! @file      application_setting_record_validator.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_APPLICATION_SETTING_RECORD_VALIDATOR_H_
#define DESIGNLAB_APPLICATION_SETTING_RECORD_VALIDATOR_H_

#include <string>
#include <tuple>

#include "application_setting_record.h"
#include "interface_toml_data_validator.h"


namespace designlab
{

//! @class ApplicationSettingRecordValidator
//! @brief ApplicationSettingRecord のバリデータ．
//! @details validator (バリデータ) は検証する人のこと．
//! @n バリデータとは，バリデーションを行う機能，またはソフトウェアのことである．
//! @n バリデータでは，入力されたデータが仕様にそって適切に記述されているかを判断し，
//! 不適切な箇所があった場合にはエラーとして通知する．
class ApplicationSettingRecordValidator final : public ITomlDataValidator<ApplicationSettingRecord>
{
public:
    ApplicationSettingRecordValidator();

    //! @brief 設定ファイルの内容を検証する．
    //! @param[in] setting_record 設定ファイルの内容．
    //! @return 検証結果．1つ目の要素が true ならば検証成功．2つ目の要素が検証失敗の理由．
    std::tuple<bool, std::string> Validate(
        const ApplicationSettingRecord& setting_record) const override;

private:
    // ge = greater equal． le = less equal．

    const std::string kErrorMesForWindowWidthGeMin;
    const std::string kErrorMesForWindowWidthLeMax;
    const std::string kErrorMesForWindowHeightGeMin;
    const std::string kErrorMesForWindowHeightLeMax;
    const std::string kErrorMesForFpsGeMin;
    const std::string kErrorMesForFpsLeMax;
};

}  // namespace designlab


#endif  // DESIGNLAB_APPLICATION_SETTING_RECORD_VALIDATOR_H_
