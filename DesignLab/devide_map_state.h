//! @file devide_map_state.h
//! @brief マップを格子状に分割して管理するクラス．

#ifndef DESIGNLAB_DEVIDE_MAP_STATE_H_
#define DESIGNLAB_DEVIDE_MAP_STATE_H_


#include "map_state.h"
#include "designlab_vector3.h"


//! @class DevideMapState
//! @brief マップを格子状に分割して管理するクラス．
//! @details 処理を軽くするために，マップが存在する領域を長方形に切り分けて，その中に存在する脚設置可能点を集めたものが devided_map_point_．
//! @n devide_mapの要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．
//! @n 座標はグローバル座標である．
class DevideMapState final
{
public:

	//! @param [in] min_z マップの最低のZ座標．
	explicit DevideMapState(float min_z = MapState::kMapMinZ);

	//! @brief Devideマップのデータを初期化する．
	//! @n マップのデータを格子状に分割し，その中に存在する脚設置可能点を集める．
	//! @param [in] map_state マップのデータ．
	//! @param [in] global_robot_com ロボットの重心のグローバル座標．
	void Init(const MapState& map_state, const ::designlab::Vector3 global_robot_com);

	//! @brief Devideマップのデータを初期化する．
	void Clear();

	//! @brief 指定した座標がDevideマップの範囲内に存在するかどうかを返す．
	//! @param [in] x グローバル座標．
	//! @param [in] y グローバル座標．
	//! @return bool 範囲内に存在するならtrue．
	constexpr bool IsInMap(const float x, const float y) const
	{
		if (x < global_robot_com_.x + kDevideMapMinX || global_robot_com_.x + kDevideMapMaxX < x) { return false; }
		if (y < global_robot_com_.y + kDevideMapMinY || global_robot_com_.y + kDevideMapMaxY < y) { return false; }

		return true;
	}

	//! @brief 指定した座標がDevideマップの範囲内に存在するかどうかを返す．
	//! @param [in] pos グローバル座標．
	//! @return bool 範囲内に存在するならtrue．
	constexpr bool IsInMap(const ::designlab::Vector3& pos) const noexcept
	{
		return IsInMap(pos.x, pos.y);
	}

	//! @brief 指定した座標がDevideマップのindexにおいてどの位置にあるかを返す．
	//! @param [in] posx グローバル座標のx座標．
	//! @return int 何番目の脚設置可能点か．
	constexpr int GetDevideMapIndexX(const float posx) const noexcept
	{
		return static_cast<int>((posx - global_robot_com_.x - kDevideMapMinX) * static_cast<float>(kDevideNum) / (kDevideMapMaxX - kDevideMapMinX));
	}

	//! @brief 指定した座標がDevideマップのindexにおいてどの位置にあるかを返す．
	//! @param [in] posy グローバル座標のy座標．
	//! @return int 何番目の脚設置可能点か．
	constexpr int GetDevideMapIndexY(const float posy) const noexcept
	{
		return static_cast<int>((posy - global_robot_com_.y - kDevideMapMinY) * static_cast<float>(kDevideNum) / (kDevideMapMaxY - kDevideMapMinY));
	}

	//! @brief 指定した座標がDevideマップのindexの範囲内になるように丸める．
	static constexpr int ClampDevideMapIndex(const int index) noexcept
	{
		if (index < 0) { return 0; }
		if (kDevideNum <= index) { return kDevideNum - 1; }

		return index;
	}

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点の数を取得する．
	//! @n 範囲外の値を指定した場合は，0を返す．
	//! @param [in] x_index X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y_index Y座標，切り分けられたタイルの位置で指定する．
	//! @return int 脚設置可能点の数
	int GetPointNum(int x_index, int y_index) const;

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点の実際の座標を取得する．
	//! @n 範囲外の値を指定した場合は，(0,0,0)を返す．
	//! @param [in] x_index x座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y_index y座標，切り分けられたタイルの位置で指定する．
	//! @param [in] devide_map_index 何番目の脚設置可能点か．
	//! @return Vector3 脚設置可能点の座標．
	::designlab::Vector3 GetPointPos(int x_index, int y_index, int devide_map_index) const;

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点のvectorを取得する
	//! @n 範囲外の値を指定した場合は，空のvectorを返す．
	//! @param [in] x_index x座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y_index y座標，切り分けられたタイルの位置で指定する．
	//! @param [out] std::vector<Vector3> point_vec 脚設置可能点の座標．
	void GetPointVector(int x_index, int y_index, std::vector<designlab::Vector3>* point_vec) const;

	//! @brief 長方形状に切り分けられたマップから，最も高いZ座標を返す．
	//! @param [in] x_index X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y_index Y座標，切り分けられたタイルの位置で指定する．
	//! @return float 最も高いZ座標．
	float GetTopZ(int x_index, int y_index) const;

	float GetMapMinZ() const noexcept { return kMapMinZ; }

private:

	static constexpr int kDevideMapPointNum{ 4 };	//!< 1つのマスに存在する脚設置可能点の数はkDevideMapPointNum × kDevideMapPointNum 個．
	static constexpr float kDevideAreaLength{ MapState::kMapPointDistance * kDevideMapPointNum };	//!< 1つのマスの一辺の長さ．

	static constexpr int kDevideNum{ 15 };
	static constexpr float kDevideMapMaxX{ kDevideAreaLength * kDevideNum / 2.0f };	//!< マップの最大のX座標
	static constexpr float kDevideMapMinX{ -kDevideMapMaxX };						//!< マップの最小のX座標
	static constexpr float kDevideMapMaxY{ kDevideAreaLength * kDevideNum / 2.0f };	//!< マップの最大のY座標
	static constexpr float kDevideMapMinY{ -kDevideMapMaxY };						//!< マップの最小のY座標

	const float kMapMinZ;	//!< マップの最低のZ座標

	//! @brief Devide Mapでは，2次元の配列を1次元の配列として扱っている．
	//! @n そのため，2次元の配列のインデックスを1次元の配列のインデックスに変換する．
	//! @param [in] x_index x座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y_index y座標，切り分けられたタイルの位置で指定する．
	//! @return int 1次元の配列のインデックス．
	constexpr int GetDevideMapIndex(const int x_index, const int y_index) const noexcept
	{
		return x_index * kDevideNum + y_index;
	}

	//! @brief 与えられたインデックスが有効な値かどうかを返す．
	//! @param [in] index マップのインデックス．
	//! @return bool 有効な値ならtrue．
	constexpr bool IsVaildIndex(const int index) const noexcept
	{
		if (index < 0 || kDevideNum <= index) { return false; }

		return true;
	}

	::designlab::Vector3 global_robot_com_;	//!< ロボットの重心のグローバル座標．

	//!< マップが存在する領域を格子状に切り分けて，その中に存在する脚設置可能点を集めたもの．
	std::vector<std::vector<::designlab::Vector3> > devided_map_point_;

	//!< devided_map_point_の中の最も高いz座標をまとめたもの，要素が存在しないなら，kMapMinZが入る．
	std::vector<float> devided_map_top_z_;


	static_assert(kDevideMapPointNum > 0, "kDevideMapPointNumは正の整数である必要があります．");
	static_assert(kDevideAreaLength > 0.0f, "kDevideAreaLengthは正の実数である必要があります．");
	static_assert(kDevideNum > 0, "kDevideNumは正の整数である必要があります．");
	static_assert(kDevideMapMaxX > kDevideMapMinX, "kDevideMapMaxXはMinより大きい必要があります．");
	static_assert(kDevideMapMaxY > kDevideMapMinY, "kDevideMapMaxYはMinより大きい必要があります．");
};


#endif	// DESIGNLAB_DEVIDE_MAP_STATE_H_