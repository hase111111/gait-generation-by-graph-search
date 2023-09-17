#pragma once

#include "node.h"

#include "hexapod_state_calculator.h"


//! @class StabilityMarginRenderer
//! @date 2023/08/30
//! @author 長谷川
//! @brief ロボットの安定性マージンを描画するクラス．
//! @n 接地点を投影した多角形の内部に，重心が入っているかどうかで安定性を判定する．Stability Margin でググると詳しい説明が出てくる．
class StabilityMarginRenderer final
{
public:

	StabilityMarginRenderer();
	~StabilityMarginRenderer() = default;

	void Draw(const SNode& node) const;


private:
	const unsigned int kMarginColor;		//!< 安定性マージンの色．

	const unsigned int kMarginErrorColor;	//!< 安定でないときの色

	const int kAlpha;						//!< 透明度．


	HexapodStateCalclator_Old m_hexapod_state_calclator;	//!< ロボットの状態を計算するクラス．
};



//! @file stability_margin_renderer.h
//! @date 2023/08/30
//! @author 長谷川
//! @brief StabilityMarginRendererクラスの実装を記述したファイル．
//! @n 行数 : @lineinfo
