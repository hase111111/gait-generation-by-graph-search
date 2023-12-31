
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "movement_locus_renderer.h"

#include <Dxlib.h>

#include "dxlib_util.h"


namespace designlab
{

MovementLocusRenderer::MovementLocusRenderer() :
    kHiddenLocusLineColor(GetColor(173, 187, 50)),
    kDisplayLocusLineColor(GetColor(239, 237, 84)),
    kHiddenLocusLineAlpha(128),
    kLocusLineMaxLength(300.0f),
    kLocusLineRadius(5.0f),
    is_high_quality_(false)
{
}


void MovementLocusRenderer::SetSimulationEndIndexes(const std::vector<size_t>& index)
{
    simulation_end_indexes_.clear();

    for (const auto& i : index)
    {
        simulation_end_indexes_.push_back(i);
    }
}


void MovementLocusRenderer::SetMoveLocusPoint(const std::vector<RobotStateNode>& locus)
{
    move_locus_point_.clear();

    for (const auto& i : locus)
    {
        move_locus_point_.push_back(i.center_of_mass_global_coord);
    }
}


void MovementLocusRenderer::Draw(const size_t draw_simulation_num,
                                 const bool draw_all_simulation)  const
{
    const size_t kSize = move_locus_point_.size();

    if (kSize < 2)
    {
        return;
    }

    for (size_t i = 0; i < kSize - 1; i++)
    {
        // 範囲外アクセスを防ぐ．
        if (i < 0 && kSize - 1 <= i)
        {
            break;
        }

        // 現在のシミュレーション回数．
        size_t now_simulation_num = simulation_end_indexes_.size();

        bool do_draw = true;  // 描画するかどうか．


        // 始点のインデックスがシミュレーション終了インデックスに含まれているならば描画を飛ばす．
        for (size_t j = 0; j < simulation_end_indexes_.size(); j++)
        {
            if (i == simulation_end_indexes_[j])
            {
                do_draw = false;
            }

            if (i < simulation_end_indexes_[j])
            {
                now_simulation_num = j;
                break;
            }
        }

        // 始点と終点の座標を描画座標に変換する．
        VECTOR start = dxlib_util::ConvertToDxlibVec(move_locus_point_.at(i));
        VECTOR end = dxlib_util::ConvertToDxlibVec(move_locus_point_.at(i + 1));


        // 描画．
        if (do_draw)
        {
            int kDivNum = 6;

            if (draw_simulation_num == now_simulation_num || draw_all_simulation)
            {
                if (is_high_quality_)
                {
                    DrawCapsule3D(start, end, kLocusLineRadius, kDivNum,
                                  kDisplayLocusLineColor, kDisplayLocusLineColor, TRUE);
                }
                else
                {
                    DrawLine3D(start, end, kDisplayLocusLineColor);
                }
            }
            else
            {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, kHiddenLocusLineAlpha);

                if (is_high_quality_)
                {
                    DrawCapsule3D(start, end, kLocusLineRadius, kDivNum,
                                  kHiddenLocusLineColor, kHiddenLocusLineColor, TRUE);
                }
                else
                {
                    DrawLine3D(start, end, kHiddenLocusLineColor);
                }

                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
        }
    }
}

}  // namespace designlab
