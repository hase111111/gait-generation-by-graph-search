//! @file hexapod_next_move.h
//! @brief ロボットが次にどの動作をするのかを表す列挙体．


#ifndef DESIGNLAB_HEXAPOD_NEXT_MOVE_H_
#define DESIGNLAB_HEXAPOD_NEXT_MOVE_H_


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


#endif	// DESIGNLAB_HEXAPOD_NEXT_MOVE_H_