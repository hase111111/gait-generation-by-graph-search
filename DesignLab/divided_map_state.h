
//! @file      divided_map_state.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DIVIDED_MAP_STATE_H_
#define DESIGNLAB_DIVIDED_MAP_STATE_H_

#include <vector>

#include "map_state.h"
#include "math_vector3.h"


namespace designlab
{

//! @class DividedMapState
//! @brief マップを格子状に分割して管理するクラス．
//! @details
//! ロボットのセンサには限界があるため，マップのデータを全て保持することはできない．
//! そのため，ロボットの近くのデータのみを保持すればよい．
//! 処理を軽くするために，マップが存在する領域を長方形に切り分けて，
//! その中に存在する脚設置可能点を集めたものが，このクラスである．
//! @n 要素は参考の「1次元の配列を多次元配列として使う」の要領で並んでいる．
//! @n 座標はグローバル座標である．
//! @subsubsection 参考
//! @li https://atcoder.jp/contests/APG4b/tasks/APG4b_t (アクセス日 2023/12/28)
class DividedMapState final
{
public:
    //!< 1つのマスに存在する脚設置可能点の数は kDividedMapPointNum × kDividedMapPointNum 個．
    static constexpr int kDividedMapPointNum{ 4 };

    //!< 1つのマスの一辺の長さ．
    static constexpr float kDividedAreaLength{ MapState::kMapPointDistance * kDividedMapPointNum };

    static constexpr int kDividedNum{ 15 };  //!< 格子の数．

    //! マップの最大のX座標．
    static constexpr float kDividedMapMaxX{ kDividedAreaLength * kDividedNum / 2.0f };

    //! マップの最小のX座標．
    static constexpr float kDividedMapMinX{ -kDividedMapMaxX };

    //! マップの最大のY座標．
    static constexpr float kDividedMapMaxY{ kDividedAreaLength * kDividedNum / 2.0f };

    //! マップの最小のY座標．
    static constexpr float kDividedMapMinY{ -kDividedMapMaxY };


    //! @param[in] min_z マップの最低のZ座標．
    explicit DividedMapState(float min_z = MapState::kMapMinZ);

    //! @brief マップのデータを初期化する．
    //! ロボットの重心座標を中心にマップのデータを格子状に分割し，
    //! その中に存在する脚設置可能点を集める．
    //! @param[in] map_state マップのデータ．
    //! @param[in] global_robot_com ロボットの重心のグローバル座標．
    void Init(const MapState& map_state, const Vector3 global_robot_com);

    //! @brief マップのデータを消去する．
    void Clear();

    //! @brief 指定した座標がマップの範囲内に存在するかどうかを返す．
    //! @param[in] x グローバル座標．
    //! @param[in] y グローバル座標．
    //! @retval true 範囲内に存在する．
    //! @retval false 範囲内に存在しない．
    constexpr bool IsInMap(const float x, const float y) const
    {
        if (x < global_robot_com_.x + kDividedMapMinX ||
            global_robot_com_.x + kDividedMapMaxX < x)
        {
            return false;
        }

        if (y < global_robot_com_.y + kDividedMapMinY ||
            global_robot_com_.y + kDividedMapMaxY < y)
        {
            return false;
        }

        return true;
    }

    //! @brief 指定した座標がマップの範囲内に存在するかどうかを返す．
    //! @param[in] pos グローバル座標．
    //! @retval true 範囲内に存在する．
    //! @retval false 範囲内に存在しない．
    constexpr bool IsInMap(const Vector3& pos) const noexcept
    {
        return IsInMap(pos.x, pos.y);
    }

    //! @brief 指定した座標を DividedMap のインデックスに変換する．
    //! 範囲外の値を指定した場合でも，値を丸めずに返す．
    //! そのため，IsInMap で範囲内に存在するかどうかを確認する必要がある．
    //! @see DividedMapState::IsInMap
    //! @param[in] pos_x グローバル座標のx座標．
    //! @return DividedMap のインデックス．
    constexpr int GetDividedMapIndexX(const float pos_x) const noexcept
    {
        return static_cast<int>(
            (pos_x - global_robot_com_.x - kDividedMapMinX) *
            static_cast<float>(kDividedNum) / (kDividedMapMaxX - kDividedMapMinX));
    }

    //! @brief 指定した座標を DividedMap のインデックスに変換する．
    //! 範囲外の値を指定した場合でも，値を丸めずに返す．
    //! そのため，IsInMap で範囲内に存在するかどうかを確認する必要がある．
    //! @see DividedMapState::IsInMap
    //! @param[in] pos_y グローバル座標のy座標．
    //! @return DividedMap のインデックス．
    constexpr int GetDividedMapIndexY(const float pos_y) const noexcept
    {
        return static_cast<int>(
            (pos_y - global_robot_com_.y - kDividedMapMinY) *
            static_cast<float>(kDividedNum) / (kDividedMapMaxY - kDividedMapMinY));
    }

    //! @brief 指定した座標がマップのインデックスの範囲内になるように丸める．
    static constexpr int ClampDividedMapIndex(const int index) noexcept
    {
        if (index < 0) { return 0; }
        if (kDividedNum <= index) { return kDividedNum - 1; }

        return index;
    }

    //! @brief 格子状に切り分けられたマップから，脚設置可能点の数を取得する．
    //! @n 範囲外の値を指定した場合は，0を返す．
    //! @param[in] x_index X座標，切り分けられたタイルの位置で指定する．
    //! @param[in] y_index Y座標，切り分けられたタイルの位置で指定する．
    //! @return 脚設置可能点の数．
    int GetPointNum(int x_index, int y_index) const;

    //! @brief 格子状に切り分けられたマップから，脚設置可能点の実際の座標を取得する．
    //! @n 範囲外の値を指定した場合は，(0,0,0)を返す．
    //! @param[in] x_index x座標，切り分けられたタイルの位置で指定する．
    //! @param[in] y_index y座標，切り分けられたタイルの位置で指定する．
    //! @param[in] divided_map_index 何番目の脚設置可能点か．
    //! @return 脚設置可能点の座標．
    Vector3 GetPointPos(int x_index, int y_index, int divided_map_index) const;

    //! @brief 格子状に切り分けられたマップから，脚設置可能点の vector を取得する
    //! @n 範囲外の値を指定した場合は，空の vector を返す．
    //! @param[in] x_index x座標，切り分けられたタイルの位置で指定する．
    //! @param[in] y_index y座標，切り分けられたタイルの位置で指定する．
    //! @param[out]  point_vec 脚設置可能点の座標の配列．
    void GetPointVector(int x_index, int y_index, std::vector<Vector3>* point_vec) const;

    //! @brief 格子状に切り分けられたマップから，最も高いZ座標を返す．
    //! @param[in] x_index X座標，切り分けられたタイルの位置で指定する．
    //! @param[in] y_index Y座標，切り分けられたタイルの位置で指定する．
    //! @return 最も高いZ座標．
    float GetTopZ(int x_index, int y_index) const;

    float GetMapMinZ() const noexcept { return kMapMinZ; }

private:
    //! @brief このクラスでは，2次元の配列を1次元の配列として扱っている．
    //! @n そのため，2次元の配列のインデックスを1次元の配列のインデックスに変換する．
    //! @param[in] x_index x座標，切り分けられたタイルの位置で指定する．
    //! @param[in] y_index y座標，切り分けられたタイルの位置で指定する．
    //! @return 1次元の配列のインデックス．
    constexpr int GetDividedMapIndex(const int x_index, const int y_index) const noexcept
    {
        return x_index * kDividedNum + y_index;
    }

    //! @brief 与えられたインデックスが有効な値かどうかを返す．
    //! @param[in] index マップのインデックス．
    //! @retval true 有効な値．
    //! @retval false 無効な値．
    constexpr bool IsValidIndex(const int index) const noexcept
    {
        if (index < 0 || kDividedNum <= index)
        {
            return false;
        }

        return true;
    }

    const float kMapMinZ;  //!< マップの最低のZ座標．

    Vector3 global_robot_com_;  //!< ロボットの重心のグローバル座標．

    //!< マップが存在する領域を格子状に切り分けて，その中に存在する脚設置可能点を集めたもの．
    std::vector<std::vector<Vector3> > divided_map_point_;

    //!< 格子の中の最も高いz座標をまとめたもの，要素が存在しないなら，kMapMinZ が入る．
    std::vector<float> divided_map_top_z_;


    static_assert(kDividedMapPointNum > 0,
                  "kDividedMapPointNum は正の整数である必要があります．");

    static_assert(kDividedAreaLength > 0.0f,
                  "kDividedAreaLengthは正の実数である必要があります．");

    static_assert(kDividedNum > 0,
                  "kDividedNum は正の整数である必要があります．");

    static_assert(kDividedMapMaxX > kDividedMapMinX,
                  " kDividedMapMaxX は kDividedMapMinX より大きい必要があります．");

    static_assert(kDividedMapMaxY > kDividedMapMinY,
                  "kDividedMapMaxY は kDividedMapMinY より大きい必要があります．");
};

}  // namespace designlab


#endif  // DESIGNLAB_DIVIDED_MAP_STATE_H_
