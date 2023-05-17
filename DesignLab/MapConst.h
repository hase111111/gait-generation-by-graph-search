#pragma once

class MapConst
{
public:
	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;

	
	constexpr static int FOOT_HOLD_XY_DIST = 20;		//z軸からみたときの足場（脚接地可能点）の間隔[mm]、格子点状
	constexpr static int START_ROUGH_TARRAIN_Y = 400;	//直進移動のときの、不整地と水平面の境界[mm]

	constexpr static int  LP_DIVIDE_NUM = 40;			//脚接地可能点を平方分割する際の１辺の分割数
	constexpr static int MAP_X_MIN = -1000;
	constexpr static int MAP_X_MAX = 1000;
	constexpr static int MAP_Y_MIN = -400;
	constexpr static int MAP_Y_MAX = 2600;
};

