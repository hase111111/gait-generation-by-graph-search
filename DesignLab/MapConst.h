#pragma once

class MapConst
{
public:
	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;

	//constexpr int 
	constexpr static int FOOT_HOLD_XY_DIST = 20;		//z軸からみたときの足場（脚接地可能点）の間隔[mm]、格子点状
	constexpr static int START_ROUGH_TARRAIN_Y = 400;	//直進移動のときの、不整地と水平面の境界[mm]
};

