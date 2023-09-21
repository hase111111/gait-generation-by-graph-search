//! @file discrete_leg_pos.h
//! @breif 離散化された脚位置を表す列挙体

#ifndef DESIGNLAB_DISCRETE_LEG_POS_H_
#define DESIGNLAB_DISCRETE_LEG_POS_H_


#include <string>


//! @enum DiscreteLegPos
//! @breif 離散化された脚位置を表すenum
//! @n 先行研究では 1～7のint型の数値で表現されているが，可読性を上げるためにenumにした
//! @n 処理の速度は変わらんはず
//! @n 離散化された脚位置は 3bit (0 ～ 7)の範囲で表現されるため，これを拡張する場合，
//! @n leg stateを表す変数の型を変更する必要がある
enum class DiscreteLegPos : int
{
	kLowerBack = 1,		//!< 現在の位置より後方かつ下方にある
	kBack,				//!< 現在の位置より後方にある
	kUpperBack,			//!< 現在の位置より後方かつ上方にある
	kCenter,			//!< 現在の位置にある
	kLowerFront,		//!< 現在の位置より前方かつ下方にある
	kFront,				//!< 現在の位置より前方にある
	kUpperFront,		//!< 現在の位置より前方かつ上方にある
};


namespace std
{
	//! @brief DiscreteLegPosを文字列に変換する
	//! @n enumを文字列に変換するため，std::to_string()をオーバーロードする．
	//! @n DiscreteLegPosの値が増えたらここも増やす必要がある．
	//! @n 正直メンテナンスが大変なんでもっといい方法ないすかね？
	//! @param[in] leg_pos 変換したいDiscreteLegPos
	//! @return DiscreteLegPosを文字列に変換したもの
	std::string to_string(DiscreteLegPos leg_pos);

}	// namespace std


#endif // DESIGNLAB_DISCRETE_LEG_POS_H_