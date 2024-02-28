
//! @file      movement_locus_renderer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_MOVEMENT_LOCUS_RENDERER_H_
#define DESIGNLAB_MOVEMENT_LOCUS_RENDERER_H_

#include <vector>

#include "math_vector3.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class MovementLocusRenderer
//! @brief ロボットの移動軌跡を描画するクラス．
class MovementLocusRenderer final
{
public:
    MovementLocusRenderer();


    //! @brief ロボットの移動軌跡を記録する．ノードの配列から，重心位置の軌跡を取得する．
    //! @param[in] locus 選択されたノードの配列．
    void SetMoveLocusPoint(const std::vector<RobotStateNode>& locus);

    //! @brief シミュレーションの終了点を取得する．
    //! @param[in] index シミュレーションの終了点の配列．
    void SetSimulationEndIndexes(const std::vector<size_t>& index);

    //! @brief 高画質モードにするかどうかを設定する．
    //! @param[in] is_high_quality 高画質モードにするかどうか．
    inline void SetIsHighQuality(const bool is_high_quality)
    {
        is_high_quality_ = is_high_quality;
    }

    //! @brief ロボットの移動軌跡を描画する．
    //! @param[in] draw_simulation_num 描画を行うシミュレーションの番号( 0, 1, 2, ...)
    //! @param[in] draw_all_simulation 上のパラメータを無視して，
    //! すべてのシミュレーションについて描画する．
    void Draw(const size_t draw_simulation_num, bool draw_all_simulation = false) const;

private:
    const unsigned int kHiddenLocusLineColor;  //!< 現在表示中でない軌道の線の色．

    const unsigned int kDisplayLocusLineColor;  //!< 現在表示中の軌道の線の色．

    const int kHiddenLocusLineAlpha;  //!< 現在表示中でない軌跡の透明度．

    const float kLocusLineMaxLength;  //!< 軌跡の線の最大長さ．この値を上回る場合は表示しない．

    const float kLocusLineRadius;  //!< 軌跡の線の半径．


    std::vector<Vector3> move_locus_point_;  //! ロボットの動きの遷移を記録する配列．

    //! シミュレーションの終了点のインデックスの配列．
    std::vector<size_t> simulation_end_indexes_;

    bool is_high_quality_;  //!< 高画質モードかどうか．
};

}  // namespace designlab


#endif  // DESIGNLAB_MOVEMENT_LOCUS_RENDERER_H_
