
//! @file      robot_ground_point_renderer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_ROBOT_GROUND_POINT_RENDERER_H_
#define DESIGNLAB_ROBOT_GROUND_POINT_RENDERER_H_

#include <array>
#include <memory>
#include <vector>

#include "math_vector3.h"
#include "hexapod_const.h"
#include "interface_hexapod_coordinate_converter.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class RobotGroundPointRenderer
//! @brief ロボットが接地点した地点の履歴を描画するクラス．
class RobotGroundPointRenderer final
{
public:
    RobotGroundPointRenderer(
        const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);


    //! ロボットが接地点した地点の履歴をセットする．
    //! @n また，シミュレーションが終了したノードのインデックスをセットする．
    //! @param[in] result_node ロボットが接地した座標の配列．
    //! @param[in] simulation_end_node_index シミュレーションが終了した
    //! ノードのインデックスの配列．
    void SetNodeAndSimulationEndNodeIndex(const std::vector<RobotStateNode>& result_node,
                                          const std::vector<size_t>& simulation_end_node_index);

    //! ロボットが接地点した地点の履歴の描画を行う．
    //! @param[in] draw_simulation_num 描画を行うシミュレーションの番号( 0, 1, 2, ...)
    //! @param[in] draw_all_simulation 上のパラメータを無視して，
    //! すべてのシミュレーションについて描画する．
    void Draw(size_t draw_simulation_num, bool draw_all_simulation = false) const;

private:
    struct VectorAndIsGround
    {
        Vector3 vec;  //!< 座標．

        bool is_ground;  //!< 接地しているかどうか．
    };


    const unsigned int kRightLegGroundPointColor;  //!< 脚接地点の色 (右足)

    const unsigned int kLeftLegGroundPointColor;  //!< 脚接地点の色 (左足)

    const unsigned int kRightLegGroundPointDarkColor;  //!< 現在のシミュレーション以外の色 (右足)

    const unsigned int kLeftLegGroundPointDarkColor;  //!< 現在のシミュレーション以外の色 (左足)

    //! ロボットの座標計算クラス．
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

    //! 読み込んだノードの数，データが更新されるたび毎回全部読込なおさないように，
    //! すでに読み込んだノードの数を記憶しておく．
    size_t loaded_node_num_;

    //! ロボットの脚接地点の座標，
    //! ground_point_[シミュレーション番号][ノード番号][脚番号]の順でアクセスする．
    std::vector<std::vector<std::array<VectorAndIsGround, HexapodConst::kLegNum>>> ground_point_;
};

}  // namespace designlab


#endif  // DESIGNLAB_ROBOT_GROUND_POINT_RENDERER_H_
