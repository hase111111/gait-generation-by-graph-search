#pragma once
#include <string>

//ロボットが次にどの動作をするのかを表す列挙体．サイズは 1 byte．sizeofで確認済み
enum class EHexapodMove : char
{
	NONE = 0,					// 何も動作をしない
	LEG_UP_DOWN = 1,			// 脚の上下移動.
	LEG_HIERARCHY_CHANGE = 2,	// 脚の平行移動．脚の階層を変更する．
	COM_MOVE = 3,				// 重心の平行移動．Center Of Massで重心のこと．
	COM_UP_DOWN = 4,			// 重心の上下移動
};

//to_stringをオーバーライドする
namespace std
{
	std::string to_string(const EHexapodMove _move);
}