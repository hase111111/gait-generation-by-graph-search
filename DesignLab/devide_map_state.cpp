#include "devide_map_state.h"

#include "math_util.h"


namespace designlab
{

DevideMapState::DevideMapState(const float min_z) :
	kMapMinZ(min_z),
	global_robot_com_{},
	devided_map_point_(math_util::Squared(kDevideNum)),
	devided_map_top_z_(math_util::Squared(kDevideNum))
{
	// コンストラクタで，vectorの大きさを確保しておく．
	Clear();
}

void DevideMapState::Init(const MapState& map_state, const Vector3 global_robot_com)
{
	assert(devided_map_point_.size() == math_util::Squared(kDevideNum));	//vectorの大きさが確保されているか確認
	assert(devided_map_top_z_.size() == math_util::Squared(kDevideNum));	//vectorの大きさが確保されているか確認

	Clear();

	global_robot_com_ = global_robot_com;	//ロボットの位置を更新する．

	//マップのデータ全てを参照し，切り分ける
	const size_t kMapPointSize = map_state.GetMapPointSize();

	for (size_t i = 0; i < kMapPointSize; ++i)
	{
		//_ xy方向のブロック番号をそれぞれ求める
		const designlab::Vector3 point = map_state.GetMapPoint(i);

		//範囲内にいないならば処理をやめ，continue
		if (!IsInMap(point)) { continue; }

		const int x = GetDevideMapIndexX(point.x);
		const int y = GetDevideMapIndexY(point.y);

		//マップの範囲内にいる時のみ追加する
		if (IsVaildIndex(x) && IsVaildIndex(y))
		{
			devided_map_point_[GetDevideMapIndex(x, y)].push_back(point);

			//最大値を更新する
			devided_map_top_z_[GetDevideMapIndex(x, y)] = std::max(point.z, devided_map_top_z_[GetDevideMapIndex(x, y)]);
		}
	}
}

void DevideMapState::Clear()
{
	// vectorの中身を全てクリアする．
	for (auto& i : devided_map_point_)
	{
		i.clear();
	}

	// 最小値で埋める．
	for (auto& i : devided_map_top_z_)
	{
		i = kMapMinZ;
	}
}

int DevideMapState::GetPointNum(const int x_index, const int y_index) const
{
	//存在していなければ終了
	if (!IsVaildIndex(x_index) || !IsVaildIndex(y_index)) { return 0; }

	return static_cast<int>(devided_map_point_[GetDevideMapIndex(x_index, y_index)].size());
}

designlab::Vector3 DevideMapState::GetPointPos(int x_index, int y_index, int devide_map_index) const
{
	//存在していなければ終了
	if (!IsVaildIndex(x_index) || !IsVaildIndex(y_index)) { return designlab::Vector3{ 0, 0, 0 }; }

	if (devide_map_index < 0 || static_cast<int>(devided_map_point_[GetDevideMapIndex(x_index, y_index)].size()) <= devide_map_index) { return designlab::Vector3{ 0, 0, 0 }; }

	//存在しているならば値を返す．
	return devided_map_point_[GetDevideMapIndex(x_index, y_index)][devide_map_index];
}

void DevideMapState::GetPointVector(int x_index, int y_index, std::vector<designlab::Vector3>* point_vec) const
{
	if (point_vec == nullptr) { return; }

	//存在していなければ終了
	if (!IsVaildIndex(x_index) || !IsVaildIndex(y_index)) { return; }

	(*point_vec) = devided_map_point_[GetDevideMapIndex(x_index, y_index)];
}

float DevideMapState::GetTopZ(int x_index, int y_index) const
{
	//存在していなければ終了
	if (!IsVaildIndex(x_index) || !IsVaildIndex(y_index)) { return kMapMinZ; }

	return devided_map_top_z_[GetDevideMapIndex(x_index, y_index)];
}

}	// namespace designlab