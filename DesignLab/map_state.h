
//! @file      map_state.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_STATE_H_
#define DESIGNLAB_MAP_STATE_H_

#include <map>
#include <vector>

#include "cassert_define.h"
#include "math_vector3.h"


namespace designlab
{

//! @class MapState
//! @brief マップを表すクラス．
//! @details
//! この研究の手法では，ロボットが歩くマップは脚設置可能点の集合で表現される．
//! 面ではなく点の集合．
//! @n 脚設置可能地点のデータは位置ベクトルの配列で実装している．
//! @n 実質的に std::vector<Vector3> のラッパークラスといえる．
//! メンバ変数のデータへのアクセスは，メンバ関数のGet関数で行う．
//! 直接データのやり取りを行わないのは，生データであると値を変更可能になってしまうからである．
//! constな関数を使えば，間違っても値の変更ができないので，データのやり取りに優れる．
class MapState final
{
public:
    MapState() : map_point_({}) {}
    explicit MapState(const std::vector<Vector3>& map_point) : map_point_(map_point) {}
    MapState(const MapState& other) = default;                  //!< コピーコンストラクタ
    MapState(MapState&& other) noexcept = default;              //!< ムーブコンストラクタ
    MapState& operator = (const MapState& other) = default;     //!< 代入演算子

    //! @brief 脚設置可能点の座標を返す．
    //! @param [in] index 何番目の脚設置可能点の座標を返すか．
    //! @n 範囲外にアクセスした場合，assertで止まる．
    //! @return 脚設置可能点の座標．グローバル座標．
    inline Vector3 GetMapPoint(const size_t index) const noexcept
    {
        assert(index < map_point_.size());

        return map_point_[index];
    }

    //! @brief 脚設置可能点の総数を返す．
    //! @return 脚設置可能点の総数
    inline size_t GetMapPointSize() const noexcept
    {
        return map_point_.size();
    }

    //! @brief 脚設置可能点の座標を1つ選び上書きする．
    //! @n 一応作ったけど，使うことはないと思う．
    //! @n 内容を書き換えたいならば Clear した後，AddMapPoint を使うこと．
    //! @param[in] index 変更する脚設置可能点の番号．
    //! @n 範囲外にアクセスした場合，assertで止まる．
    //! @param[in] point 脚設置可能点の座標．
    inline void SetMapPoint(const size_t index, const Vector3& point) noexcept
    {
        assert(index < map_point_.size());
        map_point_[index] = point;
    }

    //! @brief 脚設置可能点の座標を設定する
    //! @param[in] point 脚設置可能点の座標．
    inline void SetMapPointVec(const std::vector<Vector3>& point) noexcept
    {
        map_point_ = point;
    }

    //! @brief 脚設置可能点の座標を追加する．
    //! @param[in] point 脚設置可能点の座標．グローバル座標．
    inline void AddMapPoint(const Vector3& point) noexcept
    {
        map_point_.push_back(point);
    }

    //! @brief 脚設置可能点の座標を消去する．
    inline void ClearMapPoint() noexcept
    {
        map_point_.clear();
    }


    //! z軸から(上から)みたとき，格子点状に分けられた脚接地可能点の間隔 [mm]．
    static constexpr float kMapPointDistance{ 20.0f };
    static constexpr float kMapMinZ = { -10000000.0f };  //!< マップの最低のZ座標

private:
    std::vector<Vector3> map_point_;  //!< ロボットが歩くマップ．脚設置可能点の集合で表現される．

    static_assert(kMapPointDistance > 0.0f, "kMapPointDistanceは正の実数である必要があります．");
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_STATE_H_
