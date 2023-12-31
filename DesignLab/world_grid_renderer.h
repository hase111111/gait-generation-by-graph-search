
//! @file      world_grid_renderer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_WORLD_GRID_RENDERER_H_
#define DESIGNLAB_WORLD_GRID_RENDERER_H_

#include "interface_dxlib_3d_renderer.h"


namespace designlab
{

//! @class WorldGridRenderer
//! @brief ワールドの格子線を描画するクラス．
class WorldGridRenderer final : public IDxlib3dRenderer
{
public:
    WorldGridRenderer();

    void Draw() const override;

private:
    const unsigned int kMainGridXColor;  //!< 格子線の色．

    const unsigned int kMainGridYColor;  //!< 格子線の色．

    const unsigned int kSubGridXColor;  //!< 細い格子線の色．

    const unsigned int kSubGridYColor;  //!< 細い格子線の色．

    const int kMainGridNum;  //!< メインの格子線の数．

    const float kMainGridInterval;  //!< 格子線の間隔．

    const int kSubGridDividedNum;  //!< メインの格子線を何分割してサブの格子線をいれるか．

    const float kGridLineZPos;  //!< 格子線のZ座標．
};

}  // namespace designlab


#endif  // DESIGNLAB_WORLD_GRID_RENDERER_H_
