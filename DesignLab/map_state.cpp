#include "map_state.h"

#include "designlab_math_util.h"


namespace dlm = ::designlab::math_util;


DevideMapState::DevideMapState() :
	devided_map_point_(dlm::Squared(kDevideNum)),
	devided_map_top_z_(dlm::Squared(kDevideNum))
{
	// コンストラクタで，vectorの大きさを確保しておく．
	Clear();
}

DevideMapState::DevideMapState(const MapState& map_state) :
	devided_map_point_(dlm::Squared(kDevideNum)),
	devided_map_top_z_(dlm::Squared(kDevideNum))
{
	assert(false);
	Init(map_state, {});
}

void DevideMapState::Init(const MapState& map_state, const dl::Vector3 global_robot_com)
{
	Clear();


	//マップのデータ全てを参照し，切り分ける
	const size_t kMapPointSize = map_state.GetMapPointSize();

	for (size_t i = 0; i < kMapPointSize; ++i)
	{
		//_ xy方向のブロック番号をそれぞれ求める
		const designlab::Vector3 point = map_state.GetMapPoint(i);

		//範囲内にいないならば処理をやめ，continue
		if (point.x < global_robot_com.x + kDevideMapMinX || global_robot_com.x + kDevideMapMaxX < point.x) { continue; }
		if (point.y < global_robot_com.y + kDevideMapMinY || global_robot_com.y + kDevideMapMaxY < point.y) { continue; }

		const int x = static_cast<int>((point.x - static_cast<float>(MapConst::MAP_MIN_FORWARD)) / kLengthX);
		const int y = static_cast<int>((point.y - static_cast<float>(MapConst::MAP_MIN_HORIZONTAL)) / kLengthY);

		//マップの範囲内にいる時のみ追加する
		if (0 <= x && x < MapConst::LP_DIVIDE_NUM)
		{
			if (0 <= y && y < MapConst::LP_DIVIDE_NUM)
			{
				//値を挿入する
				devided_map_point_[GetDevideMapIndex(x, y)].push_back(point);
			}
		}
	}

	// devided_map_top_z_を更新する．

	for (size_t i = 0; i < MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM; ++i)
	{
		devided_map_top_z_[i] = kMapMinZ;	 //初期値は最小値．

		//全ての要素を参照する

		for (const auto& point : devided_map_point_[i])
		{
			//現在値と比べて大きいものを追加．
			devided_map_top_z_[i] = std::max(point.z, devided_map_top_z_[i]);
		}
	}
}

void DevideMapState::Clear()
{
	for (auto& i : devided_map_point_)
	{
		i.clear();
	}

	for (auto& i : devided_map_top_z_)
	{
		i = kMapMinZ;
	}
}

int DevideMapState::GetPointNum(const int x, const int y) const
{
	if (GetDevideMapIndex(x, y) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return 0; }
	return static_cast<int>(devided_map_point_[GetDevideMapIndex(x, y)].size());
}

designlab::Vector3 DevideMapState::GetPointPos(int x_index, int y_index, int devide_map_index) const
{
	//存在していなければ全て0のベクトルを返す．
	if (GetDevideMapIndex(x_index, y_index) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return designlab::Vector3{0, 0, 0}; }

	if (devide_map_index < 0 || static_cast<int>(devided_map_point_[GetDevideMapIndex(x_index, y_index)].size()) <= devide_map_index) { return designlab::Vector3{0, 0, 0}; }

	//存在しているならば値を返す．
	return devided_map_point_[GetDevideMapIndex(x_index, y_index)][devide_map_index];
}

void DevideMapState::GetPointVector(int x_index, int y_index, std::vector<designlab::Vector3>* point_vec) const
{
	if (point_vec == nullptr) { return; }

	//存在していなければ終了
	if (GetDevideMapIndex(x_index, y_index) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return; }

	(*point_vec) = devided_map_point_[GetDevideMapIndex(x_index, y_index)];
}

float DevideMapState::GetTopZ(int x_index, int y_index) const
{
	if (GetDevideMapIndex(x_index, y_index) >= MapConst::LP_DIVIDE_NUM * MapConst::LP_DIVIDE_NUM) { return kMapMinZ; }
	return devided_map_top_z_[GetDevideMapIndex(x_index, y_index)];
}
