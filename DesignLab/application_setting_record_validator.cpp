
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "application_setting_record_validator.h"

#include <format>


namespace designlab
{

ApplicationSettingRecordValidator::ApplicationSettingRecordValidator() :
    kErrorMesForWindowWidthGeMin(std::format(
    "The window width should be set to a value greater than or equal to {}.",
    ApplicationSettingRecord::kWindowWidthMin)),

    kErrorMesForWindowWidthLeMax(std::format(
    "The window width should be set to a value less than or equal to {}.",
    ApplicationSettingRecord::kWindowWidthMax)),

    kErrorMesForWindowHeightGeMin(std::format(
    "The window height should be set to a value greater than or equal to {}.",
    ApplicationSettingRecord::kWindowHeightMin)),

    kErrorMesForWindowHeightLeMax(std::format(
    "The window height should be set to a value less than or equal to {}.",
    ApplicationSettingRecord::kWindowHeightMax)),

    kErrorMesForFpsGeMin(std::format(
    "FPS should be set to a value greater than {}.",
    ApplicationSettingRecord::kFpsMin)),

    kErrorMesForFpsLeMax(std::format(
    "FPS should be set to a value less than or equal to {}.",
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
