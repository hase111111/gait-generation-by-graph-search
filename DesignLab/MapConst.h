#pragma once
#include <string>

// ここに定義されている数値は変更はあまりしないもの．
// よく変更するであろう数値は .cppのほうで定義している

class MapConst final
{
public:
	const static std::string INPUT_FILE_NAME;
	const static std::string OUTPUT_FILE_NAME;

	constexpr static int FOOT_HOLD_XY_DIST = 20;	//z軸からみたときの足場（脚接地可能点）の間隔[mm]、格子点状
	constexpr static int MAP_X_MIN = -1000;			//マップの横の最小値点
	constexpr static int MAP_X_MAX = 1000;			//マップの横の最大値点
	constexpr static int MAP_Y_MIN = -400;			//マップの縦の最小値点
	constexpr static int MAP_Y_MAX = 2600;			//マップの縦の最大値点
	constexpr static int MAPDATA3D_MAX = (MAP_X_MAX - MAP_X_MIN) / FOOT_HOLD_XY_DIST * (MAP_Y_MAX - MAP_Y_MIN) / FOOT_HOLD_XY_DIST;

	constexpr static int START_ROUGH_TARRAIN_Y = 400;	//直進移動のときの、不整地と水平面の境界[mm]
	constexpr static int STRIPE_INTERVAL = 5;			//各種模様や穴を作成する際，これで指定したマス分の1辺を持つ正方形状にあなをあける．

	const static unsigned int HOLE_RATE;	//不整地上の足場を除外する割合。ホール率[%]
	const static double STEP_HEIGHT;		//段差高さ[mm]．負の値にすると下りの階段になる．
	const static double STEP_LENGTH;		//階段縦幅[mm]
	const static double SLOPE_ANGLE;		//斜面の傾斜角[deg]．
	const static double TILT_ANGLE;			//地形を傾ける角度[deg]．
	const static double ROUGH_MAX_HEIGHT;	//デコボコな地形の最大高さ[mm]
	const static double ROUGH_MIN_HEIGHT;	//デコボコな地形の最小高さ[mm]

	constexpr static int LP_DIVIDE_NUM = 40;			//脚接地可能点を平方分割する際の１辺の分割数
	constexpr static int START_RANDOM_R = 200;			//初期位置のランダムで配置される範囲、半径[mm]。
	constexpr static int INVALID_FOOT_HOLD = -10000;	//ホール部にあった脚設置可能点を飛ばす座標

private:
	//デフォルトコンストラクタとコピーコンストラクタを削除する．実体が生成できないようする
	MapConst() = delete;
	MapConst(MapConst& _other) = delete;
};

