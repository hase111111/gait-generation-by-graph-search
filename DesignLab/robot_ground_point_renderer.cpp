
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "robot_ground_point_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "leg_state.h"


namespace designlab
{

RobotGroundPointRenderer::RobotGroundPointRenderer(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) :
    kRightLegGroundPointColor(GetColor(230, 15, 145)),
    kLeftLegGroundPointColor(GetColor(15, 230, 145)),
    kRightLegGroundPointDarkColor(GetColor(237, 159, 160)),
    kLeftLegGroundPointDarkColor(GetColor(159, 237, 160)),
    converter_ptr_(converter_ptr)
{
}


void RobotGroundPointRenderer::SetNodeAndSimulationEndNodeIndex(
    const std::vector<RobotStateNode>& node, const std::vector<size_t>& simulation_end_node_index)
{
    while (loaded_node_num_ < node.size())
    {
        int simulation_num = 0;  // このノードのシミュレーション番号．

        for (size_t i = 0; i < simulation_end_node_index.size(); i++)
        {
            if (simulation_end_node_index[i] >= loaded_node_num_)
            {
                break;
            }
            ++simulation_num;
        }


        // 現在のシミュレーション番号のデータがないならば追加する．
        while (simulation_num >= ground_point_.size())
        {
            ground_point_.push_back({});
        }


        // 接地点を計算し，記録する．
        std::array<VectorAndIsGround, HexapodConst::kLegNum> ground_point;

        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            ground_point[i] = {
                converter_ptr_->ConvertLegToGlobalCoordinate(
                    node[loaded_node_num_].leg_pos[i], i, node[loaded_node_num_].center_of_mass_global_coord, node[loaded_node_num_].posture, true),
                leg_func::IsGrounded(node[loaded_node_num_].leg_state, i)
            };
        }

        ground_point_[simulation_num].push_back(ground_point);

        ++loaded_node_num_;
    }
}


void RobotGroundPointRenderer::Draw(const size_t draw_simulation_num, const bool draw_all_simulation) const
{
    unsigned int color[6] = { kRightLegGroundPointColor, kRightLegGroundPointColor, kRightLegGroundPointColor,
                              kLeftLegGroundPointColor, kLeftLegGroundPointColor, kLeftLegGroundPointColor };

    unsigned int color_black[6] = { kRightLegGroundPointDarkColor, kRightLegGroundPointDarkColor, kRightLegGroundPointDarkColor,
                                    kLeftLegGroundPointDarkColor, kLeftLegGroundPointDarkColor, kLeftLegGroundPointDarkColor };

    for (size_t i = 0; i < ground_point_.size(); i++)
    {
        for (auto& leg_data : ground_point_[i])
        {
            for (size_t leg_index = 0; leg_index < HexapodConst::kLegNum; leg_index++)
            {
                if (!leg_data[leg_index].is_ground)
                {
                    continue;
                }

                if (draw_all_simulation || i == draw_simulation_num)
                {
                    dxlib_util::DrawCube3DWithTopPos(dxlib_util::ConvertToDxlibVec(leg_data[leg_index].vec), 25, color[leg_index]);
                }
                else
                {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 32);

                    dxlib_util::DrawCube3DWithTopPos(dxlib_util::ConvertToDxlibVec(leg_data[leg_index].vec), 25, color_black[leg_index]);

                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
        }
    }
}

}  // namespace designlab
