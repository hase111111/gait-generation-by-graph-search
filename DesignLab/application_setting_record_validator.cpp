
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "application_setting_record_validator.h"

#include <format>


namespace designlab
{

ApplicationSettingRecordValidator::ApplicationSettingRecordValidator() :
    kErrorMesForWindowWidthGeMin(std::format("ウィンドウの幅は{}以上の値を設定してください．",
                                 ApplicationSettingRecord::kWindowWidthMin)),
    kErrorMesForWindowWidthLeMax(std::format("ウィンドウの幅は{}以下の値を設定してください．",
                                 ApplicationSettingRecord::kWindowWidthMax)),
    kErrorMesForWindowHeightGeMin(std::format("ウィンドウの高さは{}以上の値を設定してください．",
                                  ApplicationSettingRecord::kWindowHeightMin)),
    kErrorMesForWindowHeightLeMax(std::format("ウィンドウの高さは{}以下の値を設定してください．",
                                  ApplicationSettingRecord::kWindowHeightMax)),
    kErrorMesForFpsGeMin(std::format("FPSは{}以上の値を設定してください．",
                         ApplicationSettingRecord::kFpsMin)),
    kErrorMesForFpsLeMax(std::format("FPSは{}以下の値を設定してください．",
                         ApplicationSettingRecord::kFpsMax))
{
}

std::tuple<bool, std::string> ApplicationSettingRecordValidator::Validate(
    const ApplicationSettingRecord& setting_record) const

{
    if (setting_record.window_size_x < ApplicationSettingRecord::kWindowWidthMin)
    {
        return std::make_tuple(false, kErrorMesForWindowWidthGeMin);
    }

    if (setting_record.window_size_x > ApplicationSettingRecord::kWindowWidthMax)
    {
        return std::make_tuple(false, kErrorMesForWindowWidthLeMax);
    }

    if (setting_record.window_size_y < ApplicationSettingRecord::kWindowHeightMin)
    {
        return std::make_tuple(false, kErrorMesForWindowHeightGeMin);
    }

    if (setting_record.window_size_y > ApplicationSettingRecord::kWindowHeightMax)
    {
        return std::make_tuple(false, kErrorMesForWindowHeightLeMax);
    }

    if (setting_record.window_fps < ApplicationSettingRecord::kFpsMin)
    {
        return std::make_tuple(false, kErrorMesForFpsGeMin);
    }

    if (setting_record.window_fps > ApplicationSettingRecord::kFpsMax)
    {
        return std::make_tuple(false, kErrorMesForFpsLeMax);
    }

    return std::make_tuple(true, "");
}

}  // namespace designlab
