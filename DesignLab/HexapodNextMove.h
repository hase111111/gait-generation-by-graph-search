#pragma once
#include <string>

//! @enum EHexapodMove
//! @brief ロボットが次にどの動作をするのかを表す列挙体．サイズは 1 byte．sizeofで確認済み
enum class EHexapodMove : char
{
	NONE = 0,							//!< 何も動作をしない
	LEG_UP_DOWN = 1,					//!< 脚の上下移動.
	LEG_HIERARCHY_CHANGE = 2,			//!< 脚の平行移動．脚の階層を変更する．
	COM_MOVE = 3,						//!< 重心の平行移動．Center Of Massで重心のこと．
	COM_UP_DOWN = 4,					//!< 重心の上下移動
	LEG_UP_DOWN_NEXT_COM_MOVE = 5,		//!< 脚の上下移動．(次は重心の平行移動)
	LEG_UP_DOWN_NEXT_COM_UP_DOWN = 6,	//!< 脚の上下移動．(次は重心の上下移動)
};

namespace std
{
	//@brief to_stringをオーバーロードする．
	//@details オーバーロードについては以下を参照．<br> 
	// 参考: https://www.s-cradle.com/developer/sophiaframework/tutorial/Cpp/overload.html <br>
	// std::to_stringはC++11から追加された関数．#include <string>で使用可能になる．<br>
	std::string to_string(const EHexapodMove _move);
}