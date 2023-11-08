//! @file map_const.h
//! @brief 地形状態を表す定数の実装．


#pragma once

#include <string>

//! @todo : 後で消す

//! @class MapConst
//! @brief 地形状態を表す定数．
//! @details 地形のデータの表現方法や，読み取り方を定義している． 
//! @n ヘッダーに定義されている数値は変更はあまりしないもの．
//! @n よく変更するであろう数値は .cppのほうで定義している．
class MapConst final
{
public:

	constexpr static int STRIPE_INTERVAL = 5;			//!< 各種模様や穴を作成する際，これで指定したマス分の1辺を持つ正方形状にあなをあける．

	const static unsigned int HOLE_RATE;		//!< 不整地上の足場を除外する割合。ホール率[%]
	const static float STEP_HEIGHT;				//!< 段差高さ[mm]．負の値にすると下りの階段になる．
	const static float STEP_LENGTH;				//!< 階段縦幅[mm]
	const static float SLOPE_ANGLE;				//!< 斜面の傾斜角[deg]．
	const static float TILT_ANGLE;				//!< 地形を傾ける角度[deg]．
	const static float ROUGH_MAX_HEIGHT;		//!< デコボコな地形の最大高さ[mm]
	const static float ROUGH_MIN_HEIGHT;		//!< デコボコな地形の最小高さ[mm]

private:

	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;
};