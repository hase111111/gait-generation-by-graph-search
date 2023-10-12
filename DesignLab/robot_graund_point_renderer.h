//! @file robot_graund_point_renderer.h
//! @brief ロボットの脚接地点の座標を描画するクラス

#ifndef DESIGNLAB_ROBOT_GRAUND_POINT_RENDERER_H
#define DESIGNLAB_ROBOT_GRAUND_POINT_RENDERER_H


#include <array>
#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "designlab_vector3.h"
#include "hexapod_const.h"
#include "robot_state_node.h"


//! @class RobotGraundPointRenderer
//! @brief ロボットが接地点した地点の履歴を描画するクラス
class RobotGraundPointRenderer final
{
public:

	RobotGraundPointRenderer(const std::shared_ptr<const AbstractHexapodStateCalculator> calclator_ptr);


	//! ロボットが接地点した地点の履歴をセットする．
	//! @n また，シミュレーションが終了したノードのインデックスをセットする．
	//! @param [in] result_node ロボットが接地した座標のvector
	//! @param [in] simu_end_node_index シミュレーションが終了したノードのインデックスのvector
	void SetNodeAndSimulationEndNodeIndex(const std::vector<RobotStateNode>& result_node, const std::vector<size_t>& simu_end_node_index);


	//! ロボットが接地点した地点の履歴の描画を行う．
	//! @param [in] draw_simu_num 描画を行うシミュレーションの番号( 0, 1, 2, ...)
	//! @param [in] draw_all_simulation 上のパラメータを無視して，すべてのシミュレーションについて描画する
	void Draw(size_t draw_simu_num, bool draw_all_simulation = false) const;

private:

	struct VectorAndIsGround
	{
		designlab::Vector3 vec;	//!< 座標

		bool is_ground;			//!< 接地しているかどうか
	};


	const unsigned int kRightLegGraundPointColor;		//!< 脚接地点の色 (右足)

	const unsigned int kLeftLegGraundPointColor;		//!< 脚接地点の色 (左足)

	const unsigned int kRightLegGraundPointDarkColor;	//!< 現在のシミュレーション以外の色 (右足)

	const unsigned int kLeftLegGraundPointDarkColor;	//!< 現在のシミュレーション以外の色 (左足)


	const std::shared_ptr<const AbstractHexapodStateCalculator> calclator_ptr_;	//!< ロボットの座標計算クラス．

	size_t loaded_node_num_;	//!< 読み込んだノードの数，データが更新されるたび毎回全部読込なおさないように，読み込んだノードの数を記憶しておく

	//!< ロボットの脚接地点の座標，graund_point[シミュレーション番号][ノード番号][脚番号]の順でアクセスする
	std::vector<std::vector<std::array<VectorAndIsGround, HexapodConst::kLegNum>>> graund_point_;
};


#endif // !DESIGNLAB_ROBOT_GRAUND_POINT_RENDERER_H