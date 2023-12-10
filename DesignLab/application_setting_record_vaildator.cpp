#include "application_setting_record_vaildator.h"


ApplicationSettingRecordVaildator::ApplicationSettingRecordVaildator() :
	kErrorMesForWindowWidthGtZero("ウィンドウの幅は0より大きい値を設定してください．"),
	kErrorMesForWindowWidthLeMax("ウィンドウの幅は最大値以下の値を設定してください．"),
	kErrorMesForWindowHeightGtZero("ウィンドウの高さは0より大きい値を設定してください．"),
	kErrorMesForWindowHeightLeMax("ウィンドウの高さは最大値以下の値を設定してください．"),
	kErrorMesForFpsGtZero("FPSは0より大きい値を設定してください．"),
	kErrorMesForFpsLeMax("FPSは最大値以下の値を設定してください．")
{
}

std::tuple<bool, std::string> ApplicationSettingRecordVaildator::Vaildate(const ApplicationSettingRecord& setting_record)
{
	if (setting_record.window_size_x <= 0)
	{
		return std::make_tuple(false, kErrorMesForWindowWidthGtZero);
	}

	if (setting_record.window_size_x > 1920)
	{
		return std::make_tuple(false, kErrorMesForWindowWidthLeMax);
	}

	if (setting_record.window_size_y <= 0)
	{
		return std::make_tuple(false, kErrorMesForWindowHeightGtZero);
	}

	if (setting_record.window_size_y > 1080)
	{
		return std::make_tuple(false, kErrorMesForWindowHeightLeMax);
	}

	if (setting_record.window_fps <= 0)
	{
		return std::make_tuple(false, kErrorMesForFpsGtZero);
	}

	if (setting_record.window_fps > 60)
	{
		return std::make_tuple(false, kErrorMesForFpsLeMax);
	}

	return std::make_tuple(true, "");
}