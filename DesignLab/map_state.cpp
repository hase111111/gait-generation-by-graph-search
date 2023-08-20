#include "map_state.h"


MapState& MapState::operator=(const MapState& other)
{
	//自分自身への代入は無視する．
	if (this == &other) { return *this; }

	//メンバ変数をコピーする．
	m_map_data.clear();

	for (const auto& i : other.m_map_data)
	{
		m_map_data.push_back(i);
	}


	m_devide_map.clear();

	for (const auto& i : other.m_devide_map)
	{
		m_devide_map.push_back(i);
	}


	m_devide_map_top_z.clear();

	for (const auto& i : other.m_devide_map_top_z)
	{
		m_devide_map_top_z.push_back(i);
	}


	return *this;
}

void MapState::init(const EMapCreateMode mode, const int option, const bool do_output)
{
	//引数の指定通りにマップを生成する．
	MapCreator creator;
	creator.create(mode, option, do_output, &m_map_data);

	//地形を分ける．
	makeDevideMap();
}


int MapState::getPointNumFromDevideMap(const int x, const int y) const
{
	if (getDevideMapNum(x, y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return 0; }
	return static_cast<int>(m_devide_map[getDevideMapNum(x, y)].size());
}


dl_vec::SVector MapState::getPosFromDevideMap(const int x, const int y, const int num) const
{
	//存在していなければ全て0のベクトルを返す．
	if (getDevideMapNum(x, y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return dl_vec::SVector{0, 0, 0}; }

	if (num < 0 || static_cast<int>(m_devide_map[getDevideMapNum(x, y)].size()) <= num) { return dl_vec::SVector{0, 0, 0}; }

	//存在しているならば値を返す．
	return m_devide_map[getDevideMapNum(x, y)][num];
}


dl_vec::SVector MapState::getPos(const int num) const
{
	if (num < 0 || static_cast<int>(m_map_data.size()) <= num) { return dl_vec::SVector{0, 0, 0}; }

	return m_map_data[num];
}

void MapState::makeDevideMap()
{
	//マップを切り分ける四角形の辺の長さを算出する．
	const float x_length = (MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / static_cast<float>(MapConst::LP_DIVIDE_NUM);
	const float y_length = (MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / static_cast<float>(MapConst::LP_DIVIDE_NUM);

	m_devide_map.resize(MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM);

	//マップのデータ全てに処理をする．
	for (const auto& i : m_map_data)
	{
		//xy方向のブロック番号をそれぞれ求める
		int x = (int)((i.x - (float)MapConst::MAP_MIN_FORWARD) / x_length);
		int y = (int)((i.y - (float)MapConst::MAP_MIN_HORIZONTAL) / y_length);

		//マップの範囲内にいる時のみ追加する
		if (0 <= x && x < MapConst::LP_DIVIDE_NUM)
		{
			if (0 <= y && y < MapConst::LP_DIVIDE_NUM)
			{
				//値を挿入する
				m_devide_map[getDevideMapNum(x, y)].push_back(i);
			}
		}
	}

	// m_devide_map_top_zを更新する．
	for (int i = 0; i < MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM; i++)
	{
		m_devide_map_top_z.push_back(kMapMinZ);	 //初期値は最小値．

		//全ての要素を参照する．
		for (const auto& i : m_devide_map[i])
		{
			//現在値と比べて大きいものを追加．
			m_devide_map_top_z.back() = std::max(i.z, m_devide_map_top_z.back());
		}
	}
}
