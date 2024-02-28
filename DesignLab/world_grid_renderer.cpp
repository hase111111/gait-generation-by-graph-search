
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "world_grid_renderer.h"

#include <Dxlib.h>


namespace designlab
{

WorldGridRenderer::WorldGridRenderer() :
    kMainGridXColor(GetColor(217, 0, 0)),
    kMainGridYColor(GetColor(0, 217, 0)),
    kSubGridXColor(GetColor(63, 0, 0)),
    kSubGridYColor(GetColor(0, 63, 0)),
    kMainGridNum(20),
    kMainGridInterval(500.0f),
    kSubGridDividedNum(5),
    kGridLineZPos(-50.0f)
{
}


void WorldGridRenderer::Draw() const
{
    // 格子線をどこまで描画するか．
    const float kGridMaxX = static_cast<float>(kMainGridNum) * kMainGridInterval;
    const float kGridMinX = -kGridMaxX;
    const float kGridMaxY = kGridMaxX;
    const float kGridMinY = -kGridMaxY;


    // X軸とY軸の格子線を描画する．
    DrawLine3D(VGet(kGridMinX, 0.0f, kGridLineZPos),
               VGet(kGridMaxX, 0.0f, kGridLineZPos), kMainGridXColor);

    DrawLine3D(VGet(0.0f, kGridMinY, kGridLineZPos),
               VGet(0.0f, kGridMaxY, kGridLineZPos), kMainGridYColor);


    // 格子線を 3D空間に描画する．

    const int kMainGridAlpha = 96;  // メインの格子線の透明度．
    const int kSubGridAlpha = 32;  // サブの格子線の透明度．

    for (int i = 0; i < kMainGridNum + 1 + kMainGridNum; i++)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, kMainGridAlpha);  // 半透明にする．

        // メインの格子線を描画する．
        VECTOR start_pos = VGet(kGridMinX, kGridMinY + kMainGridInterval * i, kGridLineZPos);
        VECTOR end_pos = VGet(kGridMaxX, kGridMinY + kMainGridInterval * i, kGridLineZPos);
        DrawLine3D(start_pos, end_pos, kMainGridXColor);

        start_pos = VGet(kGridMinX + kMainGridInterval * i, kGridMinY, kGridLineZPos);
        end_pos = VGet(kGridMinX + kMainGridInterval * i, kGridMaxY, kGridLineZPos);
        DrawLine3D(start_pos, end_pos, kMainGridYColor);


        // サブの格子線を描画する．
        for (int j = 0; j < kSubGridDividedNum - 1; j++)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, kSubGridAlpha);  // 半透明にする．

            start_pos = VGet(kGridMinX, kGridMinY + kMainGridInterval * i + kMainGridInterval / kSubGridDividedNum * (j + 1), kGridLineZPos);
            end_pos = VGet(kGridMaxX, kGridMinY + kMainGridInterval * i + kMainGridInterval / kSubGridDividedNum * (j + 1), kGridLineZPos);
            DrawLine3D(start_pos, end_pos, kSubGridXColor);

            start_pos = VGet(kGridMinX + kMainGridInterval * i + kMainGridInterval / kSubGridDividedNum * (j + 1), kGridMinY, kGridLineZPos);
            end_pos = VGet(kGridMinX + kMainGridInterval * i + kMainGridInterval / kSubGridDividedNum * (j + 1), kGridMaxY, kGridLineZPos);
            DrawLine3D(start_pos, end_pos, kSubGridYColor);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // 半透明を解除する．これを忘れると描画がおかしくなる．
    }
}

}  // namespace designlab
