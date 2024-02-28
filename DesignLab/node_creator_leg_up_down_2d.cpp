
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "node_creator_leg_up_down_2d.h"

#include <algorithm>

#include <boost/dynamic_bitset.hpp>

#include "com_type.h"
#include "math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace designlab
{

NodeCreatorLegUpDown2d::NodeCreatorLegUpDown2d(
  const DividedMapState& divided_map,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  enums::HexapodMove next_move) :
    kLegMargin(20),
    map_(divided_map),
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr),
    next_move_(next_move)
{
    assert(converter_ptr_ != nullptr);
    assert(presenter_ptr_ != nullptr);
    assert(checker_ptr_ != nullptr);
};

void NodeCreatorLegUpDown2d::Create(const RobotStateNode& current_node,
                                    int current_node_index,
                                    std::vector<RobotStateNode>* output_graph) const
{
    // 脚の遊脚・接地によって生じるとりうる重心を com type として仕分けている．
    // (詳しくは com_type.h を参照)．
    // vector<bool> を使用したいが，vector<bool> はテンプレートの特殊化で
    // 通常の vector とは違う挙動をするので，boost::dynamic_bitset<> を使用する．
    boost::dynamic_bitset<> is_able_leg_ground_pattern(com_func::GetLegGroundPatternNum());

    is_able_leg_ground_pattern.set();  // 全て true にする．


    // まず離散化された重心位置から取り得ない接地パターンを除外する．
    com_func::RemoveLegGroundPatternFromCom(leg_func::GetDiscreteComPos(current_node.leg_state), &is_able_leg_ground_pattern);


    // 次に脚が地面に接地可能か調べる．

    // 脚が設置可能ならば true になる．既に接地しているならば true になる．
    bool is_groundable_leg[HexapodConst::kLegNum];
    Vector3 ground_pos[HexapodConst::kLegNum];  // 脚が接地する座標．

    for (int i = 0; i < HexapodConst::kLegNum; i++) { ground_pos[i] = current_node.leg_pos[i]; }

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
        // その重心タイプが可能であれば，追加する．
        if (is_able_leg_ground_pattern[i])
        {
            RobotStateNode res_node = current_node;

            res_node.ChangeToNextNode(current_node_index, next_move_);


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
                    res_node.leg_pos[j] = presenter_ptr_->GetFreeLegPosLegCoordinate(j);

                    res_node.leg_reference_pos[j].x = res_node.leg_pos[j].x;
                    res_node.leg_reference_pos[j].y = res_node.leg_pos[j].y;
                }
            }

            if (checker_ptr_->IsStable(res_node.leg_state, res_node.leg_pos))
            {
                (*output_graph).push_back(res_node);
            }
        }
    }  // for i
}

bool NodeCreatorLegUpDown2d::IsGroundableLeg(int now_leg_num,
                                             const RobotStateNode& current_node,
                                             Vector3* output_ground_pos) const
{
    // for 文の中の continue については以下を参照．
    // http://www9.plala.or.jp/sgwr-t/c/sec06-7.html (アクセス日 2023/12/27)

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

    // マップ内を全探索して，現在の脚位置(離散化した物)に適した脚設置可能点が，存在するか調べる．
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
                    if (new_node.leg_reference_pos[now_leg_num].ProjectedXY().Cross(candidate_pos.ProjectedXY()) *
                        new_node.leg_reference_pos[now_leg_num].ProjectedXY().Cross(map_point_pos.ProjectedXY()) < 0)
                    {
                        continue;
                    }

                    // 現在の脚位置と候補地点の間に障害物がある場合は候補地点として採用しない．
                    if (map_point_pos.ProjectedXY().Cross(candidate_pos.ProjectedXY()) *
                        map_point_pos.ProjectedXY().Cross(new_node.leg_reference_pos[now_leg_num].ProjectedXY()) < 0)
                    {
                        continue;
                    }
                }

                leg_func::ChangeGround(now_leg_num, true, &new_node.leg_state);

                if (!checker_ptr_->IsLegInRange(now_leg_num, new_node.leg_pos[now_leg_num]))
                {
                    // 脚が範囲外ならば追加せずに続行．
                    continue;
                }

                if (!IsAbleLegPos(new_node, now_leg_num))
                {
                    // 候補座標として，適していないならば追加せずに続行．
                    continue;
                }

                is_candidate_pos = true;
                candidate_pos = map_point_pos;
            }
        }  // for y
    }  // for x


    // 候補点を全列挙したのち，候補点が一つもなければ falseを返す．
    if (!is_candidate_pos) { return false; }

    // 存在するなら，その中で最も適したものを結果として返し，trueを返す．
    (*output_ground_pos) = candidate_pos;

    return true;
}

bool NodeCreatorLegUpDown2d::IsAbleLegPos(const RobotStateNode& node, const int leg_index) const
{
    const enums::DiscreteLegPos discrete_leg_pos = ConvertTo2D(leg_func::GetDiscreteLegPos(node.leg_state, leg_index));  // 脚位置を取得．

    // まず最初に脚位置4のところにないか確かめる．
    if ((node.leg_reference_pos[leg_index].ProjectedXY() - node.leg_pos[leg_index].ProjectedXY()).GetSquaredLength() < math_util::Squared(kLegMargin))
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
    if (node.leg_reference_pos[leg_index].ProjectedXY().Cross(node.leg_pos[leg_index].ProjectedXY()) * node.leg_pos[leg_index].ProjectedXY().Cross({ 1, 0 }) > 0)
    {
        // 前．
        if (discrete_leg_pos == enums::DiscreteLegPos::kBack)
        {
            return false;
        }
    }
    else
    {
        // 後ろ．
        if (discrete_leg_pos == enums::DiscreteLegPos::kFront)
        {
            return false;
        }
    }

    return true;
}

enums::DiscreteLegPos NodeCreatorLegUpDown2d::ConvertTo2D(enums::DiscreteLegPos leg_pos) const
{
    switch (leg_pos)
    {
        case enums::DiscreteLegPos::kBack:
        case enums::DiscreteLegPos::kCenter:
        case enums::DiscreteLegPos::kFront:
        {
            // 2Dの場合はそのまま返す．
            return leg_pos;
        }

        case enums::DiscreteLegPos::kLowerBack:
        case enums::DiscreteLegPos::kUpperBack:
        {
            // 3Dの場合は2Dに変換する．
            return enums::DiscreteLegPos::kBack;
        }

        case enums::DiscreteLegPos::kLowerFront:
        case enums::DiscreteLegPos::kUpperFront:
        {
            // 同様に，3Dの場合は2Dに変換する．
            return enums::DiscreteLegPos::kFront;
        }

        default:
        {
            // ここに来た場合はアサートを発生させる．
            assert(false);
            return enums::DiscreteLegPos::kCenter;
        }
    }
}

}  // namespace designlab
