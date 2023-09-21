//! @file hexapod_next_move.h
//! @brief ロボットが次にどの動作をするのかを表す列挙体．

#ifndef DESIGNLAB_HEXAPOD_NEXT_MOVE_H_
#define DESIGNLAB_HEXAPOD_NEXT_MOVE_H_


#include <string>


//! @enum HexapodMove
//! @brief ロボットが次にどの動作をするのかを表す列挙体．
//! @n 先行研究では int型の変数 debug を用いていたが，可読性が低いので列挙体を用いる．
enum class HexapodMove : int
{
	kNone,						//!< 何も動作をしない．
	kLegUpDown,					//!< 脚の上下移動.
	kLegUp,						//!< 脚を上げる，遊脚する．
	kLegDown,					//!< 脚を下げる，接地する．
	kLegHierarchyChange,		//!< 脚の平行移動．脚の階層を変更する．
	kComMove,					//!< 重心の平行移動．Center Of Massで重心のこと．
	kComUpDown,					//!< 重心の上下移動．
	kLegUpDownNextComMove,		//!< 脚の上下移動．(次は重心の平行移動)
	kLegUpDownNextComUpDown,	//!< 脚の上下移動．(次は重心の上下移動)
};


namespace std
{
	//! @brief ロボットの動作を文字列に変換する．
	//! @param [in] move ロボットの動作
	//! @return std::string ロボットの動作を表す文字列
	//! @details オーバーロードについては以下を参照．
	//! @n 参考: https://www.s-cradle.com/developer/sophiaframework/tutorial/Cpp/overload.html
	//! @n std::to_stringはC++11から追加された関数．#include <string>で使用可能になる．
	//! @n メンテが面倒なので，良い方法募集中
	std::string to_string(HexapodMove move);
}


#endif	// DESIGNLAB_HEXAPOD_NEXT_MOVE_H_