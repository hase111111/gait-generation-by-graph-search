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
	MapState(const MapState& _other);	//コピーコンストラクタ


private:

	//ロボットがあるくマップ．脚設置可能点の集合で表現ざれる．
	std::vector<myvector::SVector> m_map_data;

	//マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．
	std::map< std::pair<int, int>, std::vector<myvector::SVector> > m_devide_map;
};
