
//! @file      hexapod_renderer_builder.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_
#define DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_

#include <memory>
#include <tuple>

#include "display_quality.h"
#include "interface_dxlib_node_setter.h"
#include "interface_dxlib_3d_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"


namespace designlab
{

//! @class HexapodRendererBuilder
//! @brief HexapodRendererクラスのインスタンスを作成するクラス．
class HexapodRendererBuilder final
{
public:
    //! @brief HexapodRendererクラスのインスタンスを作成する．
    //! static関数なので，HexapodRendererBuilder::Build()と呼び出す．
    //! @param[in] converter_ptr 座標変換クラスのインスタンス．
    //! @param[in] calculator_ptr 関節計算クラスのインスタンス．
    //! @return HexapodRendererクラスのインスタンス．
    //! あまりよくない気がするが，継承している2つのインターフェースで返すために，std::tupleで返す．
    static std::tuple<std::shared_ptr<IDxlib3dRenderer>, std::shared_ptr<IDxlibNodeSetter> > Build(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      enums::DisplayQuality display_quality);
};

}  // namespace designlab


#endif  // DESIGNLAB_HEXAPOD_RENDERER_BUILDER_H_
