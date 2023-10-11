//#pragma once
//
//#include "designlab_vector3.h"
//#include "designlab_polygon2.h"
//#include "com_type.h"
//#include "robot_state_node.h"
//#include "hexapod_state_calculator.h"
//
//
////! @class ComSelecter
////! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理を行っている．
//class ComSelecter final
//{
//
//public:
//
//	//! @brief 現在のノードを設定する
//	//! @param [in] _current_node 現在のノード
//	void SetCurrentNode(const RobotStateNode& _current_node) { m_current_node = _current_node; } //!< 現在のノードを設定する
//
//	//! @brief 重心を求める
//	//! @param [in] polygon 重心を求める対象のポリゴン．この中に入る点を出力する．
//	//! @param [in] _com_pattren 重心の求め方
//	//! @param [out] _output_com 重心
//	//! @return 重心を求めることができたかどうか
//	//! @details 重心位置の決め方は波東さんのプログラムに準拠している． <br>
//	//! まずは，候補地点の多角形を囲む四角形を生成，その中に等間隔で候補点を打つ． <br>
//	//! 次に多角形の中に入っていない点を除外する． <br>
//	//! そして，絶対安全余裕を計算し，マージンを外れた点を除外する． <br>
//	//! また，移動後の座標において，脚が他の脚と干渉する場合は除外する． <br>
//	//! 以上の処理を行った後，残った点の重心を求める． <br>
//	bool GetComFromPolygon(const designlab::Polygon2& polygon, const DiscreteComPos _com_pattren, designlab::Vector3& _output_com) const;
//
//private:
//
//	//! @brief 候補地点を生成する
//	void MakeComCandidatePoint(const designlab::Polygon2& polygon, std::vector<designlab::Vector2>& _output_coms) const;
//
//	//! @brief 絶対安全余裕を計算し，マージンを外れていないか調べる
//	bool IsInMargin(const designlab::Polygon2& polygon, const designlab::Vector2& _com) const;
//
//
//	const int kDiscretizationNum = 10; // 重心を求める際の分割数
//
//	const float kStabilityMargin = 10.0f; // 絶対安全余裕
//
//	const bool DO_DEBUG_PRINT = false; // デバッグ用の出力を行うかどうか．テストコードを書きたいが抽象化できていない...
//
//
//	RobotStateNode m_current_node; //!< 現在のノード
//
//	HexapodStateCalclator_Old m_calclator; //!< 状態計算クラス
//};
//
//
////! @file com_selecter.h
////! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理を行っている．