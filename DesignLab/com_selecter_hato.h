#pragma once

#include <vector>
#include <memory>

#include "designlab_vector3.h"
#include "designlab_polygon2.h"
#include "com_type.h"
#include "node.h"
#include "abstract_hexapod_state_calculator.h"



//! @class ComSelecterHato
//! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理と同様の処理を行う．
//! @details 重心位置の決め方は波東さんのプログラムに準拠している．
//! @n まずは，候補地点の多角形を囲む四角形を生成，その中に等間隔で候補点を打つ． 
//! @n 次に多角形の中に入っていない点を除外する． 
//! @n そして，絶対安全余裕を計算し，マージンを外れた点を除外する．
//! @n また，移動後の座標において，脚が他の脚と干渉する場合は除外する．
//! @n 以上の処理を行った後，残った点の重心を求める． 
//! @note CCCではTargetの値を持っているので，その値を利用して重心位置選択するが，この実装ではこのクラスにその選択を任せたくない．
//! 同様の処理を行うために，Targetの値を適当に決めている．
class ComSelecterHato final
{
public:

	ComSelecterHato(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) : mp_calculator(calc) {};


	//! @brief 現在のノードを設定する
	//! @param [in] current_node 現在のノード
	inline void setCurrentNode(const SNode& current_node) { m_current_node = current_node; } //!< 現在のノードを設定する

	//! @brief 重心を求める
	//! @param [in] polygon 重心を求める対象のポリゴン．この中に入る点を出力する．
	//! @param [in] com_pattren 重心の求め方
	//! @param [out] output_com 重心
	//! @return 重心を求めることができたかどうか
	bool getComFromPolygon(const designlab::Polygon2& polygon, designlab::Vector3* output_com) const;

private:

	static constexpr int DISCRETIZATION_NUM = 10; // 重心を求める際の分割数

	const float STABILITY_MARGIN = 10.0f; // 絶対安全余裕

	static constexpr bool DO_DEBUG_PRINT = false; // デバッグ用の出力を行うかどうか．テストコードを書きたいが抽象化できていない...



	SNode getCurrentNode() const { return m_current_node; } //!< 現在のノードを取得する

	//! @brief 候補地点を生成する
	bool makeComCandidatePoint(const designlab::Polygon2& polygon, std::pair<bool, designlab::SVector2> output_coms[DISCRETIZATION_NUM * DISCRETIZATION_NUM]) const;

	//! @brief 絶対安全余裕を計算し，マージンを外れていないか調べる
	bool isInMargin(const designlab::Polygon2& polygon, const std::vector<designlab::SVector2>& edge_vec, const designlab::SVector2& candidate_point) const;



	SNode m_current_node; //!< 現在のノード

	const std::shared_ptr<const AbstractHexapodStateCalculator> mp_calculator;	//!< ロボットの状態を計算するクラス
};



//! @file com_selecter_hato.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理と同様の処理を行う．
//! @n 行数 : @lineinfo

