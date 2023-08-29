#pragma once

#include <vector>

#include "designlab_vector.h"
#include "node.h"


//! @class MovementLocusRenderer
//! @date 2023/08/23
//! @author 長谷川
//! @brief ロボットの移動軌跡を描画するクラス．
class MovementLocusRenderer
{
public:
	MovementLocusRenderer();


	//! @brief シミュレーションの終了点を取得する
	//! @param [in] index シミュレーションの終了点の配列
	void setSimuEndIndex(const std::vector<size_t>& index);


	//! @brief ロボットの移動軌跡を記録する．
	//! @param [in] locus ロボットの移動軌跡の配列．
	void setMovementLocus(const std::vector<SNode>& locus);


	//! @brief ロボットの移動軌跡を描画する．
	void draw() const;

private:

	const unsigned int LOCUS_BASE_COLOR;			//!< 現在表示中でない，軌道

	const unsigned int LOCUS_DISPLAY_LINE_COLOR;	//!< 現在表示中の軌道の線の色

	const int LOCUS_ALPHA;							//!< 軌跡の透明度

	const float LOCUS_LINE_MAX_WIDTH = 200.0f;		//!< 軌跡の線の最大長さ


	std::vector<dl_vec::SVector> m_movement_locus;	//!< ロボットの動きの遷移を記録するvector

	std::vector<size_t> m_simu_end_index;			//!< シミュレーションの終了点の配列

	int m_display_simu_num;							//!< シミュレーション回数

};


//! @file movement_locus_renderer.h
//! @date 2023/08/23
//! @author 長谷川
//! @brief ロボットの移動軌跡を描画するクラス．