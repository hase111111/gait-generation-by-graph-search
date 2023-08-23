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


	//! @brief ロボットの移動軌跡を追加する．
	//! @param [in] locus 追加するロボットの移動軌跡．
	void addMovementLocusPoint(const dl_vec::SVector& locus);

	//! @brief ロボットの移動軌跡を記録する．
	//! @param [in] locus ロボットの移動軌跡の配列．
	void setMovementLocus(const std::vector<SNode>& locus);


	//! @brief ロボットの移動軌跡を描画する．
	void draw() const;

private:

	const unsigned int LOCUS_COLOR;					//!< 赤色

	const float LOCUS_LINE_MAX_WIDTH = 200.0f;		//!< 軌跡の線の最大長さ


	std::vector<dl_vec::SVector> m_movement_locus;	//!< ロボットの動きの遷移を記録するvector

};


//! @file movement_locus_renderer.h
//! @date 2023/08/23
//! @author 長谷川
//! @brief ロボットの移動軌跡を描画するクラス．