//! @file map_state.h
//! @brief マップのデータをもつクラス．


#ifndef DESIGNLAB_MAP_STATE_H_
#define DESIGNLAB_MAP_STATE_H_


#include <map>
#include <vector>

#include "cassert_define.h"
#include "designlab_vector3.h"
#include "map_const.h"


//! @class MapState
//! @brief マップを表すクラス．
//! @details この研究の手法では，ロボットが歩くマップは脚設置可能点の集合で表現される．面ではなく点の集合．
//! @n 脚設置可能地点のデータは位置ベクトルの配列で実装している．
//! @n 実質的にstd::vector<designlab::Vector3>のラッパークラスといえる．
//! @n メンバ変数のデータへのアクセスは，メンバ関数のGet????系の関数で行う．
//! @n 直接データのやり取りを行わないのは，生データであると値を変更可能になってしまうからである．
//! @n constな関数を使えば，間違っても値の変更ができないので，データのやり取りに優れる．
class MapState final
{
public:
	MapState() : map_point_({}) {};
	MapState(const std::vector<designlab::Vector3>& map_point) : map_point_(map_point) {};
	MapState(const MapState& other) = default;					//!< コピーコンストラクタ
	MapState(MapState&& other) noexcept = default;				//!< ムーブコンストラクタ
	MapState& operator = (const MapState& other) = default;		//!< 代入演算子

	//! @brief 脚設置可能点の座標を返す．
	//! @param [in] index 何番目の脚設置可能点の座標を返すか．
	//! @n 範囲外にアクセスした場合，assertで止まる．
	//! @return Vector3 脚設置可能点の座標．
	inline designlab::Vector3 GetMapPoint(const size_t index) const noexcept
	{
		assert(index < map_point_.size());

		return map_point_[index];
	}

	//! @brief 脚設置可能点の総数を返す．
	//! @return size_t 脚設置可能点の総数
	inline size_t GetMapPointSize() const noexcept
	{
		return map_point_.size();
	}

	//! @brief 脚設置可能点の座標を1つ選び上書きする．
	//! @n 一応作ったけど，使うことはないと思う．
	//! @n 内容を書き換えたいならばClearした後，AddMapPointを使うこと．
	//! @param [in] index 変更する脚設置可能点の番号．
	//! @n 範囲外にアクセスした場合，assertで止まる．
	//! @param [in] point 脚設置可能点の座標．
	inline void SetMapPoint(const size_t index, const designlab::Vector3& point) noexcept
	{
		assert(index < map_point_.size());
		map_point_[index] = point;
	}

	//! @brief 脚設置可能点の座標を設定する
	//! @param [in] point 脚設置可能点の座標．
	inline void SetMapPointVec(const std::vector<designlab::Vector3>& point) noexcept
	{
		map_point_ = point;
	}

	//! @brief 脚設置可能点の座標を追加する．
	//! @param [in] point 脚設置可能点の座標．
	inline void AddMapPoint(const designlab::Vector3& point) noexcept
	{
		map_point_.push_back(point);
	}

	//! @brief 脚設置可能点の座標を消去する．
	inline void ClearMapPoint() noexcept
	{
		map_point_.clear();
	}

private:

	// friendにすることで，privateなメンバ変数にアクセスできるようになる．
	template <class Char>
	friend std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const MapState& v);

	std::vector<designlab::Vector3> map_point_;	//!< ロボットが歩くマップ．脚設置可能点の集合で表現される．
};


template <class Char>
std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const MapState& map)
{
	for (const auto &i : map.map_point_)
	{
		os << i << "\n";
	}

	return os;
}


//! @class DevideMapState
//! @details 処理を軽くするために，マップが存在する領域を長方形に切り分けて，その中に存在する脚設置可能点を集めたものがdevided_map_point_．
//! @n devide_mapの要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．
class DevideMapState
{
public:
	DevideMapState();
	DevideMapState(const MapState& map_state);	//!< メンバ関数のInitを呼び出す．

	//! @brief Devideマップのデータを初期化する．
	//! @n マップのデータを格子状に分割し，その中に存在する脚設置可能点を集める．
	//! @param [in] map_state マップのデータ．
	void Init(const MapState& map_state);

	void Clear();

	constexpr bool IsInMap(const designlab::Vector3& pos) const
	{
		if (pos.x < MapConst::MAP_MIN_FORWARD || pos.x > MapConst::MAP_MAX_FORWARD) { return false; }
		if (pos.y < MapConst::MAP_MIN_HORIZONTAL || pos.y > MapConst::MAP_MAX_HORIZONTAL) { return false; }
		return true;
	}

	constexpr bool IsInMap(const float x, const float y) const
	{
		if (x < MapConst::MAP_MIN_FORWARD || x > MapConst::MAP_MAX_FORWARD) { return false; }
		if (y < MapConst::MAP_MIN_HORIZONTAL || y > MapConst::MAP_MAX_HORIZONTAL) { return false; }
		return true;
	}

	static constexpr int GetDevideMapIndexX(const float posx) 
	{
		const int res = static_cast<int>(
			(posx - MapConst::MAP_MIN_FORWARD) / 
			(((float)MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / 
				MapConst::LP_DIVIDE_NUM)
		);
		return res;
	}

	static constexpr int GetDevideMapIndexY(const float posy) 
	{
		const int res = static_cast<int>((posy - MapConst::MAP_MIN_HORIZONTAL) / (((float)MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / MapConst::LP_DIVIDE_NUM));
		return res;
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
	designlab::Vector3 GetPointPos(int x_index, int y_index, int devide_map_index) const;

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点vectorを取得する
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

private:
	float kMapMinZ = -100000.0f;	//!< マップの最低のZ座標

	constexpr int GetDevideMapIndex(const int x_index, const int y_index) const
	{
		return x_index * MapConst::LP_DIVIDE_NUM + y_index;
	}

	std::vector<std::vector<designlab::Vector3> > devided_map_point_;	//!< マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．

	std::vector<float> devided_map_top_z_;							//!< devided_map_point_の中の最も高いz座標をまとめたもの，要素が存在しないなら，kMapMinZが入る．
};


#endif // !DESIGNLAB_MAP_STATE_H_