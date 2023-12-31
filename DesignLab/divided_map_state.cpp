
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "divided_map_state.h"

#include "math_util.h"


namespace designlab
{

DividedMapState::DividedMapState(const float min_z) :
    kMapMinZ(min_z),
    global_robot_com_{},
    divided_map_point_(math_util::Squared(kDividedNum)),
    divided_map_top_z_(math_util::Squared(kDividedNum))
{
    // コンストラクタで，配列のサイズを確保しておく．
    Clear();
}

void DividedMapState::Init(const MapState& map_state, const Vector3 global_robot_com)
{
    // 配列のサイズが確保されているか確認．
    assert(divided_map_point_.size() == math_util::Squared(kDividedNum));
    assert(divided_map_top_z_.size() == math_util::Squared(kDividedNum));

    Clear();

    global_robot_com_ = global_robot_com;  // ロボットの位置を更新する．

    // マップのデータ全てを参照し，切り分ける．
    const size_t kMapPointSize = map_state.GetMapPointSize();

    for (size_t i = 0; i < kMapPointSize; ++i)
    {
        // xy方向のブロック番号をそれぞれ求める．
        const designlab::Vector3 point = map_state.GetMapPoint(i);

        // 範囲内にいないならば処理をやめ，続行．
        if (!IsInMap(point)) { continue; }

        const int x = GetDividedMapIndexX(point.x);
        const int y = GetDividedMapIndexY(point.y);

        // マップの範囲内にいる時のみ追加する．
        if (IsValidIndex(x) && IsValidIndex(y))
        {
            divided_map_point_[GetDividedMapIndex(x, y)].push_back(point);

            // 最大値を更新する．
            divided_map_top_z_[GetDividedMapIndex(x, y)] =
                (std::max)(point.z, divided_map_top_z_[GetDividedMapIndex(x, y)]);
        }
    }
}

void DividedMapState::Clear()
{
    // vectorの中身を全てクリアする．
    for (auto& i : divided_map_point_)
    {
        i.clear();
    }

    // 最小値で埋める．
    for (auto& i : divided_map_top_z_)
    {
        i = kMapMinZ;
    }
}

int DividedMapState::GetPointNum(const int x_index, const int y_index) const
{
    // 存在していなければ終了
    if (!IsValidIndex(x_index) || !IsValidIndex(y_index))
    {
        return 0;
    }

    return static_cast<int>(divided_map_point_[GetDividedMapIndex(x_index, y_index)].size());
}

Vector3 DividedMapState::GetPointPos(int x_index, int y_index, int devide_map_index) const
{
    // 存在していなければ終了．
    if (!IsValidIndex(x_index) || !IsValidIndex(y_index))
    {
        return Vector3{ 0, 0, 0 };
    }

    const size_t size = divided_map_point_[GetDividedMapIndex(x_index, y_index)].size();

    if (devide_map_index < 0 || static_cast<int>(size) <= devide_map_index)
    {
        return Vector3::GetZeroVec();
    }

    // 存在しているならば値を返す．
    return divided_map_point_[GetDividedMapIndex(x_index, y_index)][devide_map_index];
}

void DividedMapState::GetPointVector(int x_index, int y_index,
                                     std::vector<Vector3>* point_vec) const
{
    if (point_vec == nullptr)
    {
        return;
    }

    // 存在していなければ終了．
    if (!IsValidIndex(x_index) || !IsValidIndex(y_index))
    {
        return;
    }

    (*point_vec) = divided_map_point_[GetDividedMapIndex(x_index, y_index)];
}

float DividedMapState::GetTopZ(int x_index, int y_index) const
{
    // 存在していなければ終了．
    if (!IsValidIndex(x_index) || !IsValidIndex(y_index))
    {
        return kMapMinZ;
    }

    return divided_map_top_z_[GetDividedMapIndex(x_index, y_index)];
}

}  // namespace designlab
