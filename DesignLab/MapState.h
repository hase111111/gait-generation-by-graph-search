#pragma once
#include "vectorFunc.h"
#include <vector>
#include <map>
#include "MapCreator.h"

class MapState final
{
private:

	// m_map_data をもとに m_devide_mapに値をセットする
	void makeDevideMap();

	//ロボットがあるくマップ．脚設置可能点の集合で表現ざれる．
	std::vector<myvector::SVector> m_map_data;

	//マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．
	//要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．
	std::vector<std::vector<myvector::SVector> > m_devide_map;

	//m_devide_mapの中の最も高いz座標をまとめたもの，要素が存在しないなら，float型の最小値が入る．
	std::vector<float> m_devide_map_top_z;

	// m_devide_mapでは要素は1次元の配列として並んでいるので，2つの入力から，どの値を参照すればよいのかを計算する．
	inline int getDevideMapNum(const int _x, const int _y) const { return _x * MapConst::LP_DIVIDE_NUM + _y; }

public:
	MapState() = default;

	//初期化を行う．
	void init(const EMapCreateMode _mode, const int _option, const bool _do_output);

	//正方形状に切り分けられたマップから，脚設置可能点の数を取得する．
	int getPointNumFromDevideMap(const int _x, const int _y) const;

	//正方形状に切り分けられたマップから，脚設置可能点の座標を取得する
	myvector::SVector getPosFromDevideMap(const int _x, const int _y, const int _num) const;

	//正方形状に切り分けられたマップから，最も高いZ座標を返す．
	inline float getTopZFromDevideMap(const int _x, const int _y) const { return m_devide_map_top_z.at(getDevideMapNum(_x, _y)); };

	//脚設置可能点の座標を出力する．描画にのみ利用．
	myvector::SVector getPos(const int _num) const;

	//脚設置可能点の座標の数を出力する．getPos関数と併用して使用する．
	inline size_t getPosNum() const { return m_map_data.size();; }

	// x座標の値から，m_devide_mapのどこをさしているか計算して返す
	inline static int getDevideMapNumX(const float _posx)
	{
		int tmp = (int)((_posx - MapConst::MAP_MIN_HORIZONTAL) / (((float)MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / MapConst::LP_DIVIDE_NUM));
		if (tmp < 0) { return 0; }
		if (tmp >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM - 1; }
		return tmp;
	}

	// y座標の値から，m_devide_mapのどこをさしているか計算して返す
	inline static int getDevideMapNumY(const float _posy)
	{
		int tmp = (int)( (_posy - MapConst::MAP_MIN_FORWARD) / ( ((float)MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / MapConst::LP_DIVIDE_NUM) );
		if (tmp < 0)return 0;
		if (tmp >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM)return MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM - 1;
		return tmp;
	}

};
