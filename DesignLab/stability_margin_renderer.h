//! @file stability_margin_renderer.h
//! @brief ロボットの静的安定余裕(支持脚多角形)を描画するクラス．


#ifndef DESIGNLAB_STABILITY_MARGIN_RENDERER_H_
#define DESIGNLAB_STABILITY_MARGIN_RENDERER_H_


#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "node.h"


//! @class StabilityMarginRenderer
//! @brief ロボットの静的安定余裕(支持脚多角形)を描画するクラス．
//! @n 接地点を投影した多角形の内部に，重心が入っているかどうかで安定性を判定する．Stability Margin でググると詳しい説明が出てくる．
class StabilityMarginRenderer final
{
public:

	StabilityMarginRenderer(const std::shared_ptr<const AbstractHexapodStateCalculator> calclator_ptr);
	~StabilityMarginRenderer() = default;

	//! @brief ロボットの静的安定余裕(支持脚多角形)を描画する．
	//! @param [in] node ロボットの状態．
	void Draw(const SNode& node) const;


private:
	const unsigned int kMarginColor;		//!< 支持脚多角形の色．

	const unsigned int kMarginErrorColor;	//!< 安定でないときの色

	const int kAlpha;						//!< 透明度．


	const std::shared_ptr<const AbstractHexapodStateCalculator> calclator_ptr_;	//!< ロボットの状態を計算するクラス．
};


#endif // !DESIGNLAB_STABILITY_MARGIN_RENDERER_H_