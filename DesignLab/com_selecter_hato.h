//! @file com_selecter_hato.h
//! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理と同様の処理を行う．


#ifndef DESIGNLAB_COM_SELECTER_HATO_H_
#define DESIGNLAB_COM_SELECTER_HATO_H_

#include <vector>
#include <memory>

#include "com_type.h"
#include "designlab_vector3.h"
#include "designlab_polygon2.h"
#include "interface_hexapod_vaild_checker.h"
#include "robot_state_node.h"


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

	ComSelecterHato(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) : checker_ptr_(checker_ptr) {};

	//! @brief 重心を求める
	//! @param [in] polygon 重心を求める対象のポリゴン．この中に入る点を出力する．
	//! @param [in] current_node 現在のノード
	//! @param [out] output_com 重心
	//! @return 重心を求めることができたかどうか
	bool GetComFromPolygon(const designlab::Polygon2& polygon, const RobotStateNode& current_node, designlab::Vector3* output_com) const;

private:

	static constexpr int kDiscretizationNum = 10;	//!< 重心を求める際の分割数

	//! @todo この値をphantomx_mk2に移行する
	const float kStabilityMargin = 10.0f;			//!< 絶対安全余裕

	//! @brief 候補地点を生成する
	bool MakeComCandidatePoint(const designlab::Polygon2& polygon, std::pair<bool, designlab::Vector2> output_coms[kDiscretizationNum * kDiscretizationNum]) const;

	//! @brief 絶対安全余裕を計算し，マージンを外れていないか調べる
	bool IsInMargin(const designlab::Polygon2& polygon, const std::vector<designlab::Vector2>& edge_vec, const designlab::Vector2& candidate_point) const;

	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_COM_SELECTER_HATO_H_