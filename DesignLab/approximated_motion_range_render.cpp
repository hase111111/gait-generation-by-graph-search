
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "approximated_motion_range_render.h"

#include <DxLib.h>

#include "dxlib_util.h"


namespace designlab
{

ApproximatedMotionRangeRender::ApproximatedMotionRangeRender(
    const std::shared_ptr<const IHexapodPostureValidator>& validator,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter) :
    validator_(validator),
    converter_(converter)
{
    InitPosState();
}

void ApproximatedMotionRangeRender::Draw() const
{
    // 描画を行うもののみを描画する．
    for (int i = 0; i < kDivNum; ++i)
    {
        for (int j = 0; j < kDivNum; ++j)
        {
            for (int k = 220; k < kDivNum; ++k)
            {
                if (pos_state_[i][j][k] == PosState::kInRangeDrawable)
                {
                    const Vector3 pos = Vector3(
                        center_.x + (i - kDivNum / 2) * EdgeLength / kDivNum,
                        center_.y + (j - kDivNum / 2) * EdgeLength / kDivNum,
                        center_.z + (k - kDivNum / 2) * EdgeLength / kDivNum);

                    const Vector3 global_pos = converter_->ConvertLegToGlobalCoordinate(pos, kLegIndex, node_.center_of_mass_global_coord,
                                                                                        node_.posture, true);

                    dxlib_util::DrawCube3D(dxlib_util::ConvertToDxlibVec(global_pos), EdgeLength / kDivNum, GetColor(255, 0, 0));
                }
            }
        }
    }
}

void ApproximatedMotionRangeRender::SetNode(const RobotStateNode& node)
{
    node_ = node;
}

void ApproximatedMotionRangeRender::InitPosState()
{
    // まずは全ての点を範囲外に設定．
    for (int i = 0; i < kDivNum; ++i)
    {
        for (int j = 0; j < kDivNum; ++j)
        {
            for (int k = 0; k < kDivNum; ++k)
            {
                pos_state_[i][j][k] = PosState::kNotInRange;
            }
        }
    }

    // center_ を中心として，近似された脚の動作範囲内であるかどうかを判定する．

    center_ = Vector3(0.f, -150.f, -100.f);  //! @todo 仮の値．

    for (int i = 0; i < kDivNum; ++i)
    {
        for (int j = 0; j < kDivNum; ++j)
        {
            for (int k = 0; k < kDivNum; ++k)
            {
                const Vector3 pos = Vector3(
                    center_.x + (i - kDivNum / 2) * EdgeLength / kDivNum,
                    center_.y + (j - kDivNum / 2) * EdgeLength / kDivNum,
                    center_.z + (k - kDivNum / 2) * EdgeLength / kDivNum);

                if (validator_->IsLegInRange(kLegIndex, pos))
                {
                    pos_state_[i][j][k] = PosState::kInRange;
                }
            }
        }
    }

    // 範囲内の点のうち，描画可能な点を設定する．
    for (int i = 0; i < kDivNum; ++i)
    {
        for (int j = 0; j < kDivNum; ++j)
        {
            for (int k = 0; k < kDivNum; ++k)
            {
                // 自身と隣接する 8 + 9 + 9 = 26 点を調べ，範囲内が5点以下なら描画可能とする．
                if (pos_state_[i][j][k] == PosState::kNotInRange) { continue; }

                int in_range_num = 0;

                for (int di = -1; di <= 1; ++di)
                {
                    for (int dj = -1; dj <= 1; ++dj)
                    {
                        for (int dk = -1; dk <= 1; ++dk)
                        {
                            if (i + di < 0 || i + di >= kDivNum ||
                                j + dj < 0 || j + dj >= kDivNum ||
                                k + dk < 0 || k + dk >= kDivNum)
                            {
                                continue;
                            }

                            if (pos_state_[i + di][j + dj][k + dk] == PosState::kInRange ||
                                pos_state_[i + di][j + dj][k + dk] == PosState::kInRangeDrawable)
                            {
                                ++in_range_num;
                            }
                        }
                    }
                }

                if (in_range_num <= 25)
                {
                    pos_state_[i][j][k] = PosState::kInRangeDrawable;
                }
            }
        }
    }

}

}  // namespace designlab

