
//! @file      map_renderer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_RENDERER_H_
#define DESIGNLAB_MAP_RENDERER_H_

#include "divided_map_state.h"
#include "math_vector3.h"
#include "interface_dxlib_node_setter.h"
#include "interface_dxlib_3d_renderer.h"
#include "map_state.h"


namespace designlab
{

//! @class MapRenderer
//! @brief マップの描画を行うクラス．
class MapRenderer final : public IDxlibNodeSetter, public IDxlib3dRenderer
{
public:
    MapRenderer();
    ~MapRenderer() = default;

    //! @brief ロボットの重心のグローバル座標を設定する．
    //! @n DividedMap はロボットの重心を中心にしているので，
    //! ロボットの重心のグローバル座標を設定する必要がある．
    //! @n 代入と同時に DividedMap も更新される．
    //! @param[in] pos ロボットの重心のグローバル座標．
    void SetNode(const RobotStateNode& pos) override;

    //! @brief マップの状態を設定する．代入と同時に DividedMap も更新される．
    //! @param[in] map マップの状態．
    void SetMapState(const MapState& map);

    //! @brief マップの描画を行う．
    //! @param[in] map マップの状態．
    void Draw() const override;

private:
    const unsigned int kColorGray;
    const unsigned int kColorLightGray;
    const unsigned int kColorDarkGray;
    const float kCubeSize;

    MapState map_;
    DividedMapState divided_map_;
    Vector3 hexapod_pos_;
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_RENDERER_H_
