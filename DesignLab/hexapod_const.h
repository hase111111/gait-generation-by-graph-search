//! @file hexapod_const.h
//! @brief Hexapodの定数をまとめたクラス．


#ifndef DESIGNLAB_HEXAPOD_CONST_H_
#define DESIGNLAB_HEXAPOD_CONST_H_


#include "designlab_math_util.h"


//! @class HexapodConst
//! @brief Hexapodの定数をまとめたクラス．
//! @n 先行研究のマクロをまとめたもの，徐々にここから値を消していく．
//! @details コンストラクタを削除したので，実体は生成できない．( HexapodConst::kLegNum みたいに値を呼び出すこと )
class HexapodConst final
{
public:

	//コンストラクタとコピーコンストラクタを削除し，実体を生成できないようにする．
	HexapodConst() = delete;
	HexapodConst(const HexapodConst& other) = delete;
	HexapodConst& operator=(const HexapodConst& other) = delete;
	HexapodConst(HexapodConst&& other) = delete;


	constexpr static int kLegNum = 6;	//!< Hexapodの脚の本数を表す．これを変更しても脚の本数が変更できるわけではない．マジックナンバーをなくすことが目的．

};


#endif	// DESIGNLAB_HEXAPOD_CONST_H_