
//! @file      approximated_motion_range_render.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_APPROXIMATED_MOTION_RANGE_RENDER_H_
#define DESIGNLAB_APPROXIMATED_MOTION_RANGE_RENDER_H_

#include <memory>

#include "interface_dxlib_3d_renderer.h"
#include "interface_dxlib_node_setter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_coordinate_converter.h"


namespace designlab
{

//! @class ApproximatedMotionRangeRender
//! @brief 6脚歩行ロボットの近似された脚の動作範囲を描画するクラス．
//! @details 回転を考慮していないので注意．
class ApproximatedMotionRangeRender :
    public IDxlib3dRenderer,
    public IDxlibNodeSetter
{
public:
    ApproximatedMotionRangeRender(const std::shared_ptr<const IHexapodPostureValidator>& validator,
                                  const std::shared_ptr<const IHexapodCoordinateConverter>& converter);
    ~ApproximatedMotionRangeRender() = default;

    void Draw() const override;

    void SetNode(const RobotStateNode& node) override;

private:
    enum class PosState
    {
        kNotInRange,
        kInRange,
        kInRangeDrawable,
    };

    static constexpr int kDivNum = 300;
    static constexpr float EdgeLength = 300.f;
    static constexpr int kLegIndex = 1;

    void InitPosState();

    const std::shared_ptr<const IHexapodPostureValidator> validator_;
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_;

    PosState pos_state_[kDivNum][kDivNum][kDivNum];

    Vector3 center_;

    RobotStateNode node_;
};

}  // namespace designlab


#endif  // DESIGNLAB_APPROXIMATED_MOTION_RANGE_RENDER_H_
