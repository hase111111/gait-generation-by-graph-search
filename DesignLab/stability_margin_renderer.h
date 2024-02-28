
//! @file      stability_margin_renderer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_STABILITY_MARGIN_RENDERER_H_
#define DESIGNLAB_STABILITY_MARGIN_RENDERER_H_

#include <memory>

#include "interface_dxlib_node_setter.h"
#include "interface_dxlib_3d_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class StabilityMarginRenderer
//! @brief ロボットの静的安定余裕(支持脚多角形)を描画するクラス．
//! @details
//! 接地点を投影した多角形の内部に，重心が入っているかどうかで安定性を判定する．
//! Stability Margin で調べると詳しい説明が出てくる．
class StabilityMarginRenderer final :
    public IDxlib3dRenderer,
    public IDxlibNodeSetter
{
public:
    StabilityMarginRenderer(
        const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);
    ~StabilityMarginRenderer() = default;

    void SetNode(const RobotStateNode& node) override
    {
        node_ = node;
    };

    void Draw() const override;


private:
    const unsigned int kMarginColor;  //!< 支持脚多角形の色．

    const unsigned int kMarginErrorColor;  //!< 安定でないときの色

    const int kAlpha;  //!< 透明度．


    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;  //!< 座標変換器．

    RobotStateNode node_;  //!< ロボットの状態．
};

}  // namespace designlab


#endif  // DESIGNLAB_STABILITY_MARGIN_RENDERER_H_
