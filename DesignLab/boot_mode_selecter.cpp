#include "boot_mode_selecter.h"

#include <string>

#include <magic_enum.hpp>

#include "cmdio_util.h"


namespace dlio = designlab::cmdio;


BootModeSelecter::BootModeSelecter() :
	kBootModeNum(static_cast<int>(magic_enum::enum_count<BootMode>())),
	default_mode_(BootMode::kSimulation)
{
}


BootMode BootModeSelecter::SelectBootMode()
{
	OutputDetail output_detail = OutputDetail::kSystem;

	dlio::Output("起動モードを選択してください", output_detail);

	//起動モードの名前を表示する
	for (int i = 0; i < kBootModeNum; i++)
	{
		std::string boot_mode_name = static_cast<std::string>(magic_enum::enum_name(static_cast<BootMode>(i)).data());

		boot_mode_name.erase(0, 1);	//先頭のkを削除

		dlio::Output(std::to_string(i) + " : " + boot_mode_name, output_detail);
	}


	std::string default_mode_name = static_cast<std::string>(magic_enum::enum_name(default_mode_).data());
	
	default_mode_name.erase(0, 1);	//先頭のkを削除

	dlio::Output("other : デフォルトのモード ( " + default_mode_name + " )", output_detail);


	dlio::OutputNewLine(1, output_detail);

	//0からBootModeの数までの整数を入力させる
	int default_mode_num = static_cast<int>(default_mode_);

	const int input = dlio::InputInt(0, kBootModeNum - 1, default_mode_num);

	//受け取った値をmagic_enumのenum_castでBootModeに変換する
	if (magic_enum::enum_cast<BootMode>(input).has_value()) 
	{
		return magic_enum::enum_cast<BootMode>(input).value();
	}
	else
	{
		return default_mode_;
	}
}
