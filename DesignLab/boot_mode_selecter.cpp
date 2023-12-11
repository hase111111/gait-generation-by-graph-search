#include "boot_mode_selecter.h"

#include <string>

#include <magic_enum.hpp>

#include "cmdio_util.h"
#include "designlab_string_util.h"


namespace dle = ::designlab::enums;
namespace dlio = ::designlab::cmdio;
namespace dlsu = ::designlab::string_util;


BootModeSelecter::BootModeSelecter() :
	kBootModeNum(static_cast<int>(magic_enum::enum_count<dle::BootMode>())),
	default_mode_(dle::BootMode::kSimulation)
{
}


dle::BootMode BootModeSelecter::SelectBootMode()
{
	const OutputDetail output_detail = OutputDetail::kSystem;	// kSystem にすると、設定にかかわらず必ず表示される

	dlio::Output("起動モードを選択してください", output_detail);

	//起動モードの名前を表示する
	for (int i = 0; i < kBootModeNum; i++)
	{
		const dle::BootMode boot_mode = static_cast<dle::BootMode>(i);

		const std::string boot_mode_name = dlsu::EnumToStringRemoveTopK(boot_mode);

		dlio::Output(std::to_string(i) + " : " + boot_mode_name, output_detail);
	}


	std::string default_mode_name = dlsu::EnumToStringRemoveTopK(default_mode_);

	dlio::Output("other : デフォルトのモード ( " + default_mode_name + " )", output_detail);


	dlio::OutputNewLine(1, output_detail);

	//0からBootModeの数までの整数を入力させる
	int default_mode_num = static_cast<int>(default_mode_);

	const int input = dlio::InputInt(0, kBootModeNum - 1, default_mode_num);

	//受け取った値をmagic_enumのenum_castでBootModeに変換する
	if (magic_enum::enum_cast<dle::BootMode>(input).has_value())
	{
		return magic_enum::enum_cast<dle::BootMode>(input).value();
	}
	else
	{
		// 入力が不正な場合はデフォルトのモードを返す

		dlio::Output("入力が不正です。デフォルトのモードを選択します。", OutputDetail::kSystem);

		return default_mode_;
	}
}
