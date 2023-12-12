#include "application_setting_record_vaildator.h"


ApplicationSettingRecordVaildator::ApplicationSettingRecordVaildator() :
	kErrorMesForWindowWidthGeMin("ウィンドウの幅は" + std::to_string(ApplicationSettingRecord::kWindowWidthMin) + "以上の値を設定してください．"),
	kErrorMesForWindowWidthLeMax("ウィンドウの幅は" + std::to_string(ApplicationSettingRecord::kWindowWidthMax) + "以下の値を設定してください．"),
	kErrorMesForWindowHeightGeMin("ウィンドウの高さは" + std::to_string(ApplicationSettingRecord::kWindowHeightMin) + "以上の値を設定してください．"),
	kErrorMesForWindowHeightLeMax("ウィンドウの高さは" + std::to_string(ApplicationSettingRecord::kWindowHeightMax) + "以下の値を設定してください．"),
	kErrorMesForFpsGeMin("FPSは" + std::to_string(ApplicationSettingRecord::kFpsMin) + "以上の値を設定してください．"),
	kErrorMesForFpsLeMax("FPSは" + std::to_string(ApplicationSettingRecord::kFpsMax) + "以下の値を設定してください．")
{
}

std::tuple<bool, std::string> ApplicationSettingRecordVaildator::Validate(const ApplicationSettingRecord& setting_record) const

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