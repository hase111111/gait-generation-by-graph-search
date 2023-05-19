#pragma once
#include "vectorFunc.h"
#include <vector>
#include <map>
#include "MapCreator.h"

class MapState final
{
public:
	MapState() = delete;	//デフォルトコンストラクタは消去して，必ず下のコンストラクタでマップ生成をしないと実体を生成できないようにする．

	MapState(const EMapCreateMode _mode, const int _option, const bool _do_output);
	MapState(const MapState& _other) = delete;	//コピーコンストラクタ

	//正方形状に切り分けられたマップから，脚設置可能点の数を取得する．
	int getPointNumFromDevideMap(const int _x, const int _y) const;

	//正方形状に切り分けられたマップから，脚設置可能点の座標を取得する
	myvector::SVector getPosFromDevideMap(const int _x, const int _y, const int _num) const;

	myvector::SVector getPos(const int _num) const;

	// x座標の値から，m_devide_mapのどこをさしているか計算して返す
	inline static int getDevideMapNumX(const double _posx)
	{
		return (int)( (_posx - MapConst::MAP_X_MIN) / ( ((double)MapConst::MAP_X_MAX - MapConst::MAP_X_MIN) / MapConst::LP_DIVIDE_NUM) );
	}

	// y座標の値から，m_devide_mapのどこをさしているか計算して返す
	inline static int getDevideMapNumY(const double _posy)
	{
		return (int)( (_posy - MapConst::MAP_Y_MIN) / ( ((double)MapConst::MAP_Y_MAX - MapConst::MAP_Y_MIN) / MapConst::LP_DIVIDE_NUM) );
	}

private:

	// m_map_data をもとに m_devide_mapに値をセットする
	void makeDevideMap();

	//ロボットがあるくマップ．脚設置可能点の集合で表現ざれる．
	std::vector<myvector::SVector> m_map_data;

	//マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．
	//要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．
	std::vector<std::vector<myvector::SVector> > m_devide_map;

	// m_devide_mapでは要素は1次元の配列として並んでいるので，2つの入力から，どの値を参照すればよいのかを計算する．
	inline int getDevideMapNum(const int _x, const int _y) const { return _x * MapConst::LP_DIVIDE_NUM + _y; }

};
