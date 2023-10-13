//@file hexapod_renderer_builder.h
//@brief HexapodRendererクラスのインスタンスを作成するクラス


#ifndef DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_
#define DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_


#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "display_quality.h"
#include "interface_hexapod_renderer.h"


//! @class HexapodRendererBuilder
//! @brief HexapodRendererクラスのインスタンスを作成するクラス
class HexapodRendererBuilder
{
public:

	//! @brief HexapodRendererクラスのインスタンスを作成する．staticメンバ関数
	//! @param [in] calculator AbstractHexapodStateCalculatorクラスのインスタンス．
	//! @n このクラスの型を判別して，適切なHexapodRendererクラスのインスタンスを作成する
	//! @param [in] display_quality 描画品質
	//! @return HexapodRendererクラスのインスタンス
	static std::unique_ptr<IHexapodRenderer> Build(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator, DisplayQuality display_quality);
};


#endif 