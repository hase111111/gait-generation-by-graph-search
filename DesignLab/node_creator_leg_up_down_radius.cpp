
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_leg_up_down_radius.h"

#include <algorithm>

#include <boost/dynamic_bitset.hpp>

#include "com_type.h"
#include "math_const.h"
#include "math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace designlab
{

NodeCreatorLegUpDownRadius::NodeCreatorLegUpDownRadius(
  const DividedMapState& divided_map,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  enums::HexapodMove next_move
) :
    kLegMargin(10),
    map_(divided_map),
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr),
    next_move_(next_move)
{
};



void NodeCreatorLegUpDownRadius::Create(const RobotStateNode& current_node, const int current_num,
                                  std::vector<RobotStateNode>* output_graph) const
{
    // 脚の遊脚・接地によって生じるとりうる重心を com type として仕分けている．
    // (詳しくは com_type.h を参照)．
    // vector<bool>を使用したいが，vector<bool>はテンプレートの特殊化で，
    // 通常の vector とは違う挙動をするので，boost::dynamic_bitset<>を使用する．
    boost::dynamic_bitset<> is_able_leg_ground_pattern(com_func::GetLegGroundPatternNum());

    is_able_leg_ground_pattern.set();  // 全て true にする．


    // まず離散化された重心位置から取り得ない接地パターンを除外する．
    com_func::RemoveLegGroundPatternFromCom(
        leg_func::GetDiscreteComPos(current_node.leg_state), &is_able_leg_ground_pattern);


    // 次に脚が地面に接地可能か調べる．

    // 脚が設置可能ならば true になる．既に接地しているならば true になる．
    bool is_groundable_leg[HexapodConst::kLegNum];
    Vector3 ground_pos[HexapodConst::kLegNum];  // 脚が接地する座標．

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        ground_pos[i] = current_node.leg_pos[i];
    }

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(current_node.leg_state, i))
        {
            // すでに接地している脚は接地可能に決まっているので true にする．
            is_groundable_leg[i] = true;
            ground_pos[i] = current_node.leg_pos[i];
        }
        else
        {
            // 現在遊脚中の脚は自身の脚状態で接地できるか検討する．
            Vector3 res_ground_pos;

            if (IsGroundableLeg(i, current_node, &res_ground_pos))
            {
                is_groundable_leg[i] = true;  // 接地可能にする．
                ground_pos[i] = res_ground_pos;
            }
            else
            {
                is_groundable_leg[i] = false;  // 接地不可能にする．
                com_func::RemoveLegGroundPatternFromNotGroundableLeg(i, &is_able_leg_ground_pattern);
            }
        }
    }


    // 子ノードを生成する．
    for (int i = 0; i < com_func::GetLegGroundPatternNum(); i++)
    {
        // その重心タイプが可能であれば，追加する
        if (is_able_leg_ground_pattern[i])
        {
            RobotStateNode res_node = current_node;

            res_node.ChangeToNextNode(current_num, next_move_);


            // 遊脚・接地を書き換える．
            leg_func::LegGroundedBit new_is_ground =
                com_func::GetLegGroundedBitFromLegGroundPatternIndex(i);

            leg_func::ChangeAllLegGround(new_is_ground, &res_node.leg_state);


            // 脚位置を書き換える．
            for (int j = 0; j < HexapodConst::kLegNum; j++)
            {
                if (new_is_ground[j])
                {
                    res_node.leg_pos[j] = ground_pos[j];

                    res_node.leg_reference_pos[j] = ground_pos[j];
                }
                else
                {
                    res_node.leg_pos[j].z = presenter_ptr_->GetFreeLegPosLegCoordinate(j).z;

                    res_node.leg_reference_pos[j].x = presenter_ptr_->GetFreeLegPosLegCoordinate(j).x;
                    res_node.leg_reference_pos[j].y = presenter_ptr_->GetFreeLegPosLegCoordinate(j).y;
                }
            }

            if (checker_ptr_->IsStable(res_node.leg_state, res_node.leg_pos))
            {
                (*output_graph).push_back(res_node);
            }
        }  // if is_able_leg_ground_pattern[i]
    }  // for i
}


bool NodeCreatorLegUpDownRadius::IsGroundableLeg(const int now_leg_num,
                                           const RobotStateNode& current_node,
                                           Vector3* output_ground_pos) const
{
    // 脚座標がマップのどこに当たるか調べて，そのマスの2つ上と2つ下の範囲内を全て探索する．
    const Vector3 global_leg_base_pos = converter_ptr_->ConvertLegToGlobalCoordinate(
      current_node.leg_reference_pos[now_leg_num],
      now_leg_num,
      current_node.center_of_mass_global_coord,
      current_node.posture,
      true);


    int max_x_dev = map_.GetDividedMapIndexX(global_leg_base_pos.x) + 2;
    int min_x_dev = map_.GetDividedMapIndexX(global_leg_base_pos.x) - 2;
    int max_y_dev = map_.GetDividedMapIndexY(global_leg_base_pos.y) + 2;
    int min_y_dev = map_.GetDividedMapIndexY(global_leg_base_pos.y) - 2;

    // 値がマップの範囲外にあるときは丸める．
    max_x_dev = DividedMapState::ClampDividedMapIndex(max_x_dev);
    min_x_dev = DividedMapState::ClampDividedMapIndex(min_x_dev);
    max_y_dev = DividedMapState::ClampDividedMapIndex(max_y_dev);
    min_y_dev = DividedMapState::ClampDividedMapIndex(min_y_dev);

    // マップ内を全探索して，現在の脚位置(離散化した物)に適した脚設置可能点が存在するか調べる．

    Vector3 candidate_pos;  // 現在の脚位置に合致する候補座標群．
    bool is_candidate_pos = false;  // 候補座標が存在するかどうか．

    // 範囲内の点を全て調べる．
    for (int x = min_x_dev; x < max_x_dev; x++)
    {
        for (int y = min_y_dev; y < max_y_dev; y++)
        {
            const int kPosNum = map_.GetPointNum(x, y);

            for (int n = 0; n < kPosNum; n++)
            {
                // 脚設置可能点の座標を取り出す．
                Vector3 map_point_pos = map_.GetPointPos(x, y, n);
                map_point_pos = converter_ptr_->ConvertGlobalToLegCoordinate(
                  map_point_pos,
                  now_leg_num,
                  current_node.center_of_mass_global_coord,
                  current_node.posture,
                  true);

                // 脚位置を更新したノードを作成する．
                RobotStateNode new_node = current_node;

                new_node.leg_pos[now_leg_num] = map_point_pos;


                // 前の候補地点と比較して，より良い候補地点の時のみ実行すする
                if (is_candidate_pos)
                {
                    // 反対方向をむいている場合は候補地点として採用しない．
                    if (new_node.leg_reference_pos[now_leg_num].ProjectedXY().Cross(
                        candidate_pos.ProjectedXY()) *
                        new_node.leg_reference_pos[now_leg_num].ProjectedXY().Cross(
                        map_point_pos.ProjectedXY()) < 0 - MathConst<float>::kAllowableError)
                    {
                        continue;
                    }

                    // 現在の脚位置と候補地点の間に障害物がある場合は候補地点として採用しない．
                    if (map_point_pos.ProjectedXY().Cross(
                        candidate_pos.ProjectedXY()) *
                        map_point_pos.ProjectedXY().Cross(
                        new_node.leg_reference_pos[now_leg_num].ProjectedXY()) < 0 - MathConst<float>::kAllowableError)
                    {
                        continue;
                    }
                }

                leg_func::ChangeGround(now_leg_num, true, &new_node.leg_state);

                // 脚が範囲外ならば追加せずに続行．
                if (!checker_ptr_->IsLegInRange(now_leg_num, new_node.leg_pos[now_leg_num]))
                {
                    continue;
                }

                // 候補座標として，適していないならば追加せずに続行．
                if (!IsAbleLegPos(new_node, now_leg_num)) { continue; }

                // 脚先が干渉しないか調べる．
                if (checker_ptr_->IsLegInterfering(new_node.leg_pos))
                {
                    continue;
                }

                is_candidate_pos = true;
                candidate_pos = map_point_pos;
            }
        }
    }


    // 候補点を全列挙したのち，候補点が一つもなければ false．
    if (!is_candidate_pos) { return false; }

    // 存在するなら，その中で最も適したものを結果として返し，true．
    (*output_ground_pos) = candidate_pos;

    return true;
}

bool NodeCreatorLegUpDownRadius::IsAbleLegPos(const RobotStateNode& _node, const int leg_index) const
{
    const enums::DiscreteLegPos discrete_leg_pos =
        leg_func::GetDiscreteLegPos(_node.leg_state, leg_index);  // 脚位置を取得(1～7)

    // まず最初に脚位置4のところにないか確かめる．
    if ((_node.leg_reference_pos[leg_index].ProjectedXY() - _node.leg_pos[leg_index].ProjectedXY()).GetSquaredLength() <
        math_util::Squared(kLegMargin))
    {
        if (discrete_leg_pos == enums::DiscreteLegPos::kCenter)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (discrete_leg_pos == enums::DiscreteLegPos::kCenter)
        {
            return false;
        }
    }

    // 脚位置4と比較して前か後ろか．
    if (_node.leg_reference_pos[leg_index].ProjectedXY()
        .Cross(_node.leg_pos[leg_index].ProjectedXY()) *
        _node.leg_pos[leg_index].ProjectedXY().Cross({ 1, 0 }) > 0)
    {
        // 前．
        if (discrete_leg_pos == enums::DiscreteLegPos::kLowerBack ||
            discrete_leg_pos == enums::DiscreteLegPos::kBack ||
            discrete_leg_pos == enums::DiscreteLegPos::kUpperBack)
        {
            return false;
        }
    }
    else
    {
        // 後ろ．
        if (discrete_leg_pos == enums::DiscreteLegPos::kLowerFront ||
            discrete_leg_pos == enums::DiscreteLegPos::kFront ||
            discrete_leg_pos == enums::DiscreteLegPos::kUpperFront)
        {
            return false;
        }
    }


    // 脚位置4と半径を比較して上か下か．
    if (discrete_leg_pos == enums::DiscreteLegPos::kLowerFront ||
        discrete_leg_pos == enums::DiscreteLegPos::kLowerBack)
    {
        // 脚位置4と比較して下．
        if (_node.leg_reference_pos[leg_index].ProjectedXY().GetSquaredLength() -
            _node.leg_pos[leg_index].ProjectedXY().GetSquaredLength() - kLegMargin > 0)
        {
            return true;
        }
    }
    else if (discrete_leg_pos == enums::DiscreteLegPos::kUpperFront ||
             discrete_leg_pos == enums::DiscreteLegPos::kUpperBack)
    {
        // 脚位置4と比較して上．
        if (_node.leg_pos[leg_index].ProjectedXY().GetSquaredLength() -
            _node.leg_reference_pos[leg_index].ProjectedXY().GetSquaredLength() - kLegMargin > 0)
        {
            return true;
        }
    }
    else
    {
        // 脚位置4と同じくらい．
        if (std::abs(_node.leg_reference_pos[leg_index].ProjectedXY().GetSquaredLength() -
            _node.leg_pos[leg_index].ProjectedXY().GetSquaredLength()) < kLegMargin)
        {
            return true;
        }
    }

    return false;
}

}  // namespace designlab
