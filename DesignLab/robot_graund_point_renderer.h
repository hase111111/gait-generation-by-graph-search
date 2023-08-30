#pragma once

#include <vector>

#include "designlab_vector.h"
#include "node.h"

//! @class RobotGraundPointRenderer
//! @date 2033/08/29
//! @author 長谷川
//! @brief ロボットの脚接地点の座標を描画するクラス
class RobotGraundPointRenderer final
{
public:

	RobotGraundPointRenderer();


	//! ロボットの脚接地点の座標を設定する．
	//! @param [in] node ロボットの脚接地点の座標
	void setNode(const std::vector<SNode>& node, const std::vector<size_t>& simu_end_node_index);


	//! ロボットの脚接地点の描画を行う．
	//! @param [in] draw_simu_num 描画を行うシミュレーションの番号( 0, 1, 2, ...)
	//! @param [in] draw_all_simulation 上のパラメータを無視して，すべてのシミュレーションについて描画する
	void draw(const size_t draw_simu_num, const bool draw_all_simulation = false) const;

private:

	const unsigned int GRAUND_POINT_COLOR_RIGHT;			//!< 脚接地点の色

	const unsigned int GRAUND_POINT_COLOR_LEFT;			//!< 脚接地点の色

	const unsigned int GRAUND_POINT_COLOR_BLACK_RIGHT;		//!< すべてのシミュレーションについて描画する場合，現在のシミュレーション以外の色

	const unsigned int GRAUND_POINT_COLOR_BLACK_LEFT;		//!< すべてのシミュレーションについて描画する場合，現在のシミュレーション以外の色


	std::vector<size_t> m_simu_end_index;			//!< シミュレーションの終了インデックス

	size_t m_loaded_node_num;						//!< 読み込んだノードの数

	std::vector<std::vector<std::pair<dl_vec::SVector, int>>> m_graund_point;	//!< ロボットの脚接地点の座標
};



//! @file robot_graund_point_renderer.h
//! @date 2033/08/29
//! @author 長谷川
//! @brief ロボットの脚接地点の座標を描画するクラス
//! @n 行数 : @lineinfo