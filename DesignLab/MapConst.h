#pragma once
#include <string>

class MapConst final
{
public:
	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;

	const static std::string OUTPUT_FILE_NAME;
	
	constexpr static int FOOT_HOLD_XY_DIST = 20;		//z軸からみたときの足場（脚接地可能点）の間隔[mm]、格子点状
	constexpr static int START_ROUGH_TARRAIN_Y = 400;	//直進移動のときの、不整地と水平面の境界[mm]

	constexpr static int HEIGHT_STEP = -140;	//段差高さ[mm]
	constexpr static int  DEPTH_STEP = 500;		//奥行き[mm]

	constexpr static double THETA_SLOPE = 20;	//最大傾斜角[°]	斜面の最大傾斜角
	constexpr static double XI_SLOPE = 0;		//最大傾斜方位角ξ[°]　y軸(グローバル)と最大傾斜方向へのベクトル（勾配ベクトル）のなす角 ±90°は89.9くらいにしとく

	constexpr static int WIDE_TRI = 400;		//2等辺三角形の底辺の長さ/2[mm]
	constexpr static int DEPTH_TRI = 400;		//2等辺三角形の奥行き
	constexpr static int THETA_TRI = 10;		//底角[°]

	constexpr static int LP_DIVIDE_NUM = 40;	//脚接地可能点を平方分割する際の１辺の分割数
	constexpr static int MAP_X_MIN = -1000;
	constexpr static int MAP_X_MAX = 1000;
	constexpr static int MAP_Y_MIN = -400;
	constexpr static int MAP_Y_MAX = 2600;
	constexpr static int MAPDATA3D_MAX = (MAP_X_MAX - MAP_X_MIN) / FOOT_HOLD_XY_DIST * (MAP_Y_MAX - MAP_Y_MIN) / FOOT_HOLD_XY_DIST;

	constexpr static int HOLE_RATE = 60;				//不整地上の足場を除外する割合。ホール率[%]
	constexpr static int HEIGHT_MAGNIFICATION = 10;		//高さ方向のランダムな倍率と刻み数[-]　ここ後で名前変える、有りそうだから
	constexpr static int START_RANDOM_R = 200;			//初期位置のランダムで配置される範囲、半径[mm]。
	constexpr static int INVALID_FOOT_HOLD = -10000;	//ホール部にあった脚設置可能点を飛ばす座標

	constexpr static int SQUARE_SIZE = 100;		//＝FOOT_HOLD_XY_DIST*n(n=1,2,3...)n=1なら1点ずつ、2なら4点、nならn^2点の正方形が対象になる正方形の一片の長さ[mm]
};

