#include "robot_graund_point_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "leg_state.h"


namespace designlab
{

RobotGraundPointRenderer::RobotGraundPointRenderer(const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) :
    kRightLegGraundPointColor(GetColor(230, 15, 145)),
    kLeftLegGraundPointColor(GetColor(15, 230, 145)),
    kRightLegGraundPointDarkColor(GetColor(237, 159, 160)),
    kLeftLegGraundPointDarkColor(GetColor(159, 237, 160)),
    converter_ptr_(converter_ptr)
{
}


void RobotGraundPointRenderer::SetNodeAndSimulationEndNodeIndex(const std::vector<RobotStateNode>& node, const std::vector<size_t>& simu_end_node_index)
{
    while (loaded_node_num_ < node.size())
    {
        int simu_num = 0;	//このノードのシミュレーション番号

        for (size_t i = 0; i < simu_end_node_index.size(); i++)
        {
            if (simu_end_node_index[i] >= loaded_node_num_)
            {
                break;
            }
            ++simu_num;
        }


        //現在のシミュレーション番号のデータがないならば追加する
        while (simu_num >= graund_point_.size()) { graund_point_.push_back({}); }


        //接地点を計算し，記録する
        std::array<VectorAndIsGround, HexapodConst::kLegNum> graund_point;

        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            graund_point[i] = {
              converter_ptr_->ConvertLegToGlobalCoordinate(
                node[loaded_node_num_].leg_pos[i], i, node[loaded_node_num_].center_of_mass_global_coord, node[loaded_node_num_].posture, true
              ),
              leg_func::IsGrounded(node[loaded_node_num_].leg_state, i)
            };
        }

        graund_point_[simu_num].push_back(graund_point);

        ++loaded_node_num_;
    }
}


void RobotGraundPointRenderer::Draw(const size_t draw_simu_num, const bool draw_all_simulation) const
{
    unsigned int color[6] = { kRightLegGraundPointColor,kRightLegGraundPointColor,kRightLegGraundPointColor,kLeftLegGraundPointColor,kLeftLegGraundPointColor,kLeftLegGraundPointColor };

    unsigned int color_black[6] = { kRightLegGraundPointDarkColor,kRightLegGraundPointDarkColor,kRightLegGraundPointDarkColor,kLeftLegGraundPointDarkColor,kLeftLegGraundPointDarkColor,kLeftLegGraundPointDarkColor };

    for (size_t i = 0; i < graund_point_.size(); i++)
    {
        for (auto& leg_data : graund_point_[i])
        {
            for (size_t leg_index = 0; leg_index < HexapodConst::kLegNum; leg_index++)
            {
                if (!leg_data[leg_index].is_ground) { continue; }

                if (draw_all_simulation || i == draw_simu_num)
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

}	//namespace designlab