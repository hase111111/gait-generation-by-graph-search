#include "pch.h"

#include "../DesignLab/leg_state.h"
#include "../DesignLab/com_type.h"
#include "../DesignLab/com_type.cpp"



namespace LegStateEditTest
{
	TEST(MakeLegStateFunc, MakeLegState_ChangeComPattren)
	{
		//全脚接地のデータを作成する．
		ComType::EComPattern com_pattern = ComType::EComPattern::FRONT;
		bool _ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		int _expected = 0b110011001100110011001100 | ComType::convertComPatternToBit(com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(com_pattern, _ground, _leg_pos), _expected);

		//重心パターンを変更して，重心の値が変わるか，脚位置の値が変化しないか確認
		com_pattern = ComType::EComPattern::BACK;
		_expected = 0b110011001100110011001100 | ComType::convertComPatternToBit(com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(com_pattern, _ground, _leg_pos), _expected);

		com_pattern = ComType::EComPattern::BACK_LEFT;
		_expected = 0b110011001100110011001100 | ComType::convertComPatternToBit(com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(com_pattern, _ground, _leg_pos), _expected);
	}


	TEST(MakeLegStateFunc, MakeLegState_ChangeGround)
	{
		//様々な脚接地パターンを作成する．
		ComType::EComPattern _com_pattern = ComType::EComPattern::FRONT;
		bool _ground[HexapodConst::LEG_NUM] = { false, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		int _expected = 0b110011001100110011000100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		_ground[0] = true; _ground[1] = false; _ground[2] = true;
		_ground[3] = true; _ground[4] = true; _ground[5] = true;
		_expected = 0b110011001100110001001100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		_ground[0] = true; _ground[1] = true; _ground[2] = false;
		_ground[3] = true; _ground[4] = true; _ground[5] = true;
		_expected = 0b110011001100010011001100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		//全脚遊脚のデータを作成する．
		_ground[0] = false; _ground[1] = false; _ground[2] = false;
		_ground[3] = false; _ground[4] = false; _ground[5] = false;
		_expected = 0b010001000100010001000100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);
	}


	TEST(MakeLegStateFunc, MAkeLegState_ChangeLegPos)
	{
		//様々な脚位置パターンを作成する．
		ComType::EComPattern _com_pattern = ComType::EComPattern::FRONT;
		bool _ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		int _expected = 0b110011001100110011001100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		//脚位置を変更して，脚位置の値が変わるか，重心の値が変化しないか確認
		_leg_pos[0] = 3; _leg_pos[1] = 3; _leg_pos[2] = 3;
		_leg_pos[3] = 3; _leg_pos[4] = 3; _leg_pos[5] = 3;
		_expected = 0b101110111011101110111011 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		_leg_pos[0] = 2; _leg_pos[1] = 2; _leg_pos[2] = 2;
		_leg_pos[3] = 2; _leg_pos[4] = 2; _leg_pos[5] = 2;
		_expected = 0b101010101010101010101010 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);

		//範囲外の値を入れても，範囲内の値に収まるか確認
		_leg_pos[0] = 10; _leg_pos[1] = 10; _leg_pos[2] = 10;
		_leg_pos[3] = -5; _leg_pos[4] = -5; _leg_pos[5] = -5;
		_expected = 0b110011001100110011001100 | ComType::convertComPatternToBit(_com_pattern) << dl_leg::SHIFT_TO_COM_NUM;
		EXPECT_EQ(dl_leg::makeLegState(_com_pattern, _ground, _leg_pos), _expected);
	}
}