#include "MapState.h"
#include <cfloat>
#include <algorithm>

void MapState::init(const EMapCreateMode _mode, const int _option, const bool _do_output)
{
	//引数の指定通りにマップを生成する．
	MapCreator _creator;
	_creator.create(_mode, _option, m_map_data, _do_output);

	//地形を分ける．
	makeDevideMap();
}

int MapState::getPointNumFromDevideMap(const int _x, const int _y) const
{
	if (getDevideMapNum(_x, _y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return 0; }
	return (int)m_devide_map.at(getDevideMapNum(_x, _y)).size();
}

my_vec::SVector MapState::getPosFromDevideMap(const int _x, const int _y, const int _num) const
{
	//存在していなければ全て0のベクトルを返す．
	if (getDevideMapNum(_x, _y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return my_vec::SVector(0, 0, 0); }

	if (_num < 0 || m_devide_map[getDevideMapNum(_x, _y)].size() <= _num) { return my_vec::SVector(0, 0, 0); }

	//存在しているならば値を返す．
	return m_devide_map[getDevideMapNum(_x, _y)][_num];
}

my_vec::SVector MapState::getPos(const int _num) const
{
	if (_num < 0 || m_map_data.size() <= _num) { return my_vec::SVector(0, 0, 0); }

	return m_map_data.at(_num);
}

void MapState::makeDevideMap()
{
	//マップを切り分ける四角形の辺の長さを算出する．
	const float _lengthX = (MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / (float)MapConst::LP_DIVIDE_NUM;
	const float _lengthY = (MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / (float)MapConst::LP_DIVIDE_NUM;

	m_devide_map.resize(MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM);

	//マップのデータ全てに処理をする．
	for (const auto& i : m_map_data)
	{
		//xy方向のブロック番号をそれぞれ求める
		int x = (int)((i.x - (float)MapConst::MAP_MIN_FORWARD) / _lengthX);
		int y = (int)((i.y - (float)MapConst::MAP_MIN_HORIZONTAL) / _lengthY);

		//マップの範囲内にいる時のみ追加する
		if (0 <= x && x < MapConst::LP_DIVIDE_NUM)
		{
			if (0 <= y && y < MapConst::LP_DIVIDE_NUM)
			{
				//値を挿入する
				m_devide_map.at(getDevideMapNum(x, y)).push_back(i);
			}
		}
	}

	// m_devide_map_top_zを更新する．
	for (int i = 0; i < MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM; i++)
	{
		//floatの最小値を追加する．
		m_devide_map_top_z.push_back(-1000000.0f);

		//全ての要素を参照する．
		for (const auto& i : m_devide_map.at(i))
		{
			//現在値と比べて大きいものを追加．
			m_devide_map_top_z.back() = std::max(i.z, m_devide_map_top_z.back());
		}
	}
}
