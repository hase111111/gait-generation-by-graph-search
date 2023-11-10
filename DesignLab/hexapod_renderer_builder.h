//@file hexapod_renderer_builder.h
//@brief HexapodRendererクラスのインスタンスを作成するクラス


#ifndef DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_
#define DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_


#include <memory>

#include "display_quality.h"
#include "interface_hexapod_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"

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
	static std::unique_ptr<IHexapodRenderer> Build(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		DisplayQuality display_quality
	);
};


#endif 