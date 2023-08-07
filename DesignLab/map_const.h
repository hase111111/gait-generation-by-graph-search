#pragma once

#include <string>

//! @class MapConst
//! @date 2023/08/06
//! @author 長谷川
//! @brief 地形状態を表す定数．
//! @details 地形のデータの表現方法や，読み取り方を定義している． 
//! @n ヘッダーに定義されている数値は変更はあまりしないもの．
//! @n よく変更するであろう数値は .cppのほうで定義している．
class MapConst final
{
public:
	const static std::string INPUT_FILE_NAME;			//!< 出力されるファイルの名前．英語の名前にすることを推奨．
	const static std::string OUTPUT_FILE_NAME;			//!< マップデータをファイルから読み取るときに，読み取るファイルの名前．

	constexpr static int FOOT_HOLD_XY_DIST = 20;		//!< z軸からみたときの足場（脚接地可能点）の間隔[mm]、格子点状

	constexpr static int MAP_MIN_HORIZONTAL = -1000;	//!< マップの横の最小値点
	constexpr static int MAP_MAX_HORIZONTAL = 1000;		//!< マップの横の最大値点
	constexpr static int MAP_MIN_FORWARD = -400;		//!< マップの縦の最小値点
	constexpr static int MAP_START_ROUGH = 400;			//!< マップの縦の不整地と水平面の境界[mm]
	constexpr static int MAP_MAX_FORWARD = 2600;		//!< マップの縦の最大値点
	constexpr static float MAX_Z_BASE = 0.0f;			//!< ロボットが最初に待機している場所の Z 座標．

	constexpr static int STRIPE_INTERVAL = 5;			//!< 各種模様や穴を作成する際，これで指定したマス分の1辺を持つ正方形状にあなをあける．

	const static unsigned int HOLE_RATE;		//!< 不整地上の足場を除外する割合。ホール率[%]
	const static float STEP_HEIGHT;				//!< 段差高さ[mm]．負の値にすると下りの階段になる．
	const static float STEP_LENGTH;				//!< 階段縦幅[mm]
	const static float SLOPE_ANGLE;				//!< 斜面の傾斜角[deg]．
	const static float TILT_ANGLE;				//!< 地形を傾ける角度[deg]．
	const static float ROUGH_MAX_HEIGHT;		//!< デコボコな地形の最大高さ[mm]
	const static float ROUGH_MIN_HEIGHT;		//!< デコボコな地形の最小高さ[mm]

	constexpr static int LP_DIVIDE_NUM = 40;	//!< 脚接地可能点を平方分割する際の１辺の分割数

private:

	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;
};

//! @file map_const.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief 地形状態を表す定数の実装．
//! @n 行数 : @lineinfo