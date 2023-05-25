#include "MapState.h"

MapState::MapState(const EMapCreateMode _mode, const int _option, const bool _do_output)
{
	//コンストラクタの指定通りにマップを生成する．
	map_creator::createMap(_mode, _option, m_map_data, _do_output);

	//地形を分ける．
	makeDevideMap();
}

int MapState::getPointNumFromDevideMap(const int _x, const int _y) const
{
	if (getDevideMapNum(_x, _y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return 0; }
	return (int)m_devide_map.at(getDevideMapNum(_x, _y)).size();
}

myvector::SVector MapState::getPosFromDevideMap(const int _x, const int _y, const int _num) const
{
	//存在していなければ全て0のベクトルを返す．
	if (getDevideMapNum(_x, _y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return myvector::SVector(0, 0, 0); }

	if (_num < 0 || m_devide_map.at(getDevideMapNum(_x, _y)).size() <= _num ) { return myvector::SVector(0, 0, 0); }

	//存在しているならば値を返す．
	return m_devide_map.at(getDevideMapNum(_x, _y)).at(_num);
}

myvector::SVector MapState::getPos(const int _num) const
{
	if (_num < 0 || m_map_data.size() <= _num) { return myvector::SVector(0, 0, 0); }

	return m_map_data.at(_num);
}

void MapState::makeDevideMap()
{
	//マップを切り分ける四角形の辺の長さを算出する．
	const float _lengthX = (MapConst::MAP_X_MAX - MapConst::MAP_X_MIN) / (float)MapConst::LP_DIVIDE_NUM;
	const float _lengthY = (MapConst::MAP_Y_MAX - MapConst::MAP_Y_MIN) / (float)MapConst::LP_DIVIDE_NUM;

	m_devide_map.resize(MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM);

	//マップのデータ全てに処理をする．
	for (const auto &i : m_map_data)
	{
		//xy方向のブロック番号をそれぞれ求める
		int x = (int)((i.x - (float)MapConst::MAP_X_MIN) / _lengthX);
		int y = (int)((i.y - (float)MapConst::MAP_Y_MIN) / _lengthY);

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
}
