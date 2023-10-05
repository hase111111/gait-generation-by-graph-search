//! @file com_candidate_polygon_maker.h
//! @brief 重心位置の候補地点を示す多角形を作成するクラス


#ifndef DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_
#define DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_


#include <array>
#include <memory>
#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "designlab_polygon2.h"
#include "discrete_com_pos.h"
#include "robot_state_node.h"


//! @struct ComPosAndPolygon
//! @brief 離散化された重心位置とその重心位置を含む多角形の組み合わせ．
struct ComPosAndPolygon
{
	ComPosAndPolygon() : com_pos(DiscreteComPos::kFront), polygon(), is_able(false) {}

	DiscreteComPos com_pos;			//!< 離散化された重心位置
	designlab::Polygon2 polygon;	//!< 重心位置を含む多角形
	bool is_able;					//!< 重心位置を含む多角形が正しいかどうか
};


//! @class ComCandidatePolygonMaker
//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
//! @n 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと
//! @n 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，
//! @n ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
public:

	ComCandidatePolygonMaker(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc);


	static constexpr int MAKE_POLYGON_NUM = 7;	//!< 作成する多角形の数


	//! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
	//! @param [in] node 現在のロボットの状態を表すノード
	//! @param [out] output_poly 重心位置の候補地点を示す多角形
	void MakeCandidatePolygon(const RobotStateNode& node, std::array<ComPosAndPolygon, MAKE_POLYGON_NUM>* output_poly) const;


private:

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は4角形か5角形を用いて表現する．
	void MakeCandidateBox(const std::array<designlab::Vector2, HexapodConst::LEG_NUM>& leg_pos, const int start_leg_num, designlab::Polygon2* output_poly) const;


	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は3角形を用いて表現する．
	void MakeCandidateTriangle(const std::array<designlab::Vector2, HexapodConst::LEG_NUM>& leg_pos, ComPosAndPolygon* output) const;


	//! @brief 正しい多角形が生成されているかを確認する
	//! @param [in] _poly 確認する多角形
	//! @return 正しい多角形が生成されているか
	bool IsAblePolygon(const designlab::Polygon2& poly) const;


	static constexpr bool kDoCheckPolygon = true;	// 多角形のチェックを行う場合はtrueにする．重いのでfalseにしたいが，深さ5までなら問題なし．


	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス

};


#endif	//DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_