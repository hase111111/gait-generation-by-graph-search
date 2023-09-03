#pragma once

#include <vector>
#include <memory>

#include "node.h"
#include "designlab_polygon.h"
#include "com_type.h"
#include "abstract_hexapod_state_calculator.h"


//! @class ComCandidatePolygonMaker
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
//! @n 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと
//! @n 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，
//! @n ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
public:

	ComCandidatePolygonMaker(std::shared_ptr<AbstractHexapodStateCalculator> calc) : mp_calculator(calc) {};


	static constexpr int MAKE_POLYGON_NUM = 7;	//!< 作成する多角形の数


	//! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
	//! @param [in] node 現在のロボットの状態を表すノード
	//! @param [out] output_poly 重心位置の候補地点を示す多角形
	void makeCandidatePolygon(const SNode& node, std::pair<dl_vec::SPolygon2, ComType::EComPattern> output_poly[MAKE_POLYGON_NUM]) const;


private:

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は4角形か5角形を用いて表現する．
	void makeCandidateBox(const dl_vec::SVector2 leg_pos[HexapodConst::LEG_NUM], const int start_leg_num, dl_vec::SPolygon2* output_poly) const;


	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は3角形を用いて表現する．
	void makeCandidateTriangle(const dl_vec::SVector2 leg_pos[HexapodConst::LEG_NUM], dl_vec::SPolygon2* output_poly, ComType::EComPattern* output_com_pattern) const;


	//! @brief 正しい多角形が生成されているかを確認する
	//! @param [in] _poly 確認する多角形
	//! @return 正しい多角形が生成されているか
	bool checkPolygon(const dl_vec::SPolygon2& poly) const;


	static constexpr bool DO_DEBUG_PRINT = false;	// デバッグ用に出力を行う場合はtrueにする．テストコードを書きたいけど抽象化できていない...

	static constexpr bool DO_CHECK_POLYGON = true;	// 多角形のチェックを行う場合はtrueにする．重いのでfalseにしたいが，深さ5までなら問題なし．


	std::shared_ptr<AbstractHexapodStateCalculator> mp_calculator;	//!< ロボットの状態を計算するクラス

};


//! @file com_candidate_polygon_maker.h
//! @date 2023/08/12
//!	@author 長谷川
//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @n 行数 : @lineinfo
