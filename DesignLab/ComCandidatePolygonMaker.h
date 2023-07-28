#pragma once
#include "Node.h"
#include "MyPolygon.h"
#include "ComType.h"
#include "HexapodStateCalculator.h"
#include <vector>

//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する<br>
//! 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと<br>
//! 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，<br>
//! ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
public:

	static constexpr int MAKE_POLYGON_NUM = 7;	//!< 作成する多角形の数

	ComCandidatePolygonMaker() = default;

	//! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
	//! @param[in] node 現在のロボットの状態を表すノード
	//! @param[out] output_poly 重心位置の候補地点を示す多角形
	void makeCandidatePolygon(const SNode& node, std::pair<my_vec::SPolygon2, ComType::EComPattern> output_poly[MAKE_POLYGON_NUM]) const;

private:

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は4角形か5角形を用いて表現する．
	void makeCandidateBox(const my_vec::SVector2 leg_pos[HexapodConst::LEG_NUM], const int start_leg_num, my_vec::SPolygon2* output_poly) const;

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は3角形を用いて表現する．
	void makeCandidateTriangle(const my_vec::SVector2 leg_pos[HexapodConst::LEG_NUM], my_vec::SPolygon2* output_poly, ComType::EComPattern* output_com_pattern) const;

	//! @brief 正しい多角形が生成されているかを確認する
	//! @param[in] _poly 確認する多角形
	//! @return 正しい多角形が生成されているか
	bool checkPolygon(const my_vec::SPolygon2& poly) const;

	static constexpr bool DO_DEBUG_PRINT = false;	// デバッグ用に出力を行う場合はtrueにする．テストコードを書きたいけど抽象化できていない...

	static constexpr bool DO_CHECK_POLYGON = true;	// 多角形のチェックを行う場合はtrueにする．重いのでfalseにしたいが，深さ5までなら問題なし．

	const HexapodStateCalclator m_calclator;

};