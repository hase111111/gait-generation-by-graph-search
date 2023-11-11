//! @file hexapod_renderer_builder.h
//! @brief HexapodRendererクラスのインスタンスを作成するクラス


#ifndef DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_
#define DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_


#include <memory>

#include "display_quality.h"
#include "interface_hexapod_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"

//! @class HexapodRendererBuilder
//! @brief HexapodRendererクラスのインスタンスを作成するクラス
class HexapodRendererBuilder final
{
public:

	//! @brief HexapodRendererクラスのインスタンスを作成する．static関数なので，HexapodRendererBuilder::Build()と呼び出す．
	//! @param [in] calculator AbstractHexapodStateCalculatorクラスのインスタンス．
	//! @n このクラスの型を判別して，適切なHexapodRendererクラスのインスタンスを作成する．
	//! @param [in] converter AbstractHexapodCoordinateConverterクラスのインスタンス．shared_ptrで渡すこと．
	//! @param [in] calculator AbstractHexapodStateCalculatorクラスのインスタンス．shared_ptrで渡すこと．
	//! @param [in] display_quality 描画品質．
	//! @return HexapodRendererクラスのインスタンス．unique_ptrで返す．
	static std::unique_ptr<IHexapodRenderer> Build(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		DisplayQuality display_quality
	);
};


#endif	// #ifndef DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_