
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "dxlib_util.h"

#include <Dxlib.h>

#include "math_vector3.h"
#include "graphic_const.h"


namespace designlab::dxlib_util
{

void InitDxlib3DSetting(const bool high_quality)
{
    if (high_quality)
    {
        SetUseLighting(TRUE);
        SetLightEnable(TRUE);

        // ライトの設定．
        ChangeLightTypeDir(ConvertToDxlibVec(Vector3::GetUpVec()));
    }
    else
    {
        SetUseLighting(FALSE);
        SetLightEnable(FALSE);
    }
}


void SetZBufferEnable()
{
    // Ｚバッファを有効にする．
    SetUseZBuffer3D(TRUE);

    // Ｚバッファへの書き込みを有効にする．
    SetWriteZBuffer3D(TRUE);
}


void DrawCube3D(const VECTOR& center_pos, const float side_len, const unsigned int color)
{
    // 立方体は8つの頂点を持つので，それらの座標を計算する．
    const std::array<VECTOR, 8> vertex =
    {
        VGet(center_pos.x - side_len / 2, center_pos.y - side_len / 2, center_pos.z - side_len / 2),
        VGet(center_pos.x + side_len / 2, center_pos.y - side_len / 2, center_pos.z - side_len / 2),
        VGet(center_pos.x + side_len / 2, center_pos.y - side_len / 2, center_pos.z + side_len / 2),
        VGet(center_pos.x - side_len / 2, center_pos.y - side_len / 2, center_pos.z + side_len / 2),
        VGet(center_pos.x - side_len / 2, center_pos.y + side_len / 2, center_pos.z - side_len / 2),
        VGet(center_pos.x + side_len / 2, center_pos.y + side_len / 2, center_pos.z - side_len / 2),
        VGet(center_pos.x + side_len / 2, center_pos.y + side_len / 2, center_pos.z + side_len / 2),
        VGet(center_pos.x - side_len / 2, center_pos.y + side_len / 2, center_pos.z + side_len / 2)
    };

    // 3D描画の関数は3角形を基本単位とするので，
    // 4角形の面を張りたい場合は，2つの三角形を組み合わせる必要がある．
    // つまり，6面×2つ＝12個の三角形で立方体が描画できる．

    DrawTriangle3D(vertex[0], vertex[1], vertex[2], color, TRUE);
    DrawTriangle3D(vertex[2], vertex[3], vertex[0], color, TRUE);

    DrawTriangle3D(vertex[4], vertex[5], vertex[6], color, TRUE);
    DrawTriangle3D(vertex[6], vertex[7], vertex[4], color, TRUE);

    DrawTriangle3D(vertex[4], vertex[7], vertex[0], color, TRUE);
    DrawTriangle3D(vertex[0], vertex[7], vertex[3], color, TRUE);

    DrawTriangle3D(vertex[1], vertex[2], vertex[5], color, TRUE);
    DrawTriangle3D(vertex[5], vertex[6], vertex[2], color, TRUE);

    DrawTriangle3D(vertex[0], vertex[1], vertex[5], color, TRUE);
    DrawTriangle3D(vertex[5], vertex[4], vertex[0], color, TRUE);

    DrawTriangle3D(vertex[2], vertex[3], vertex[7], color, TRUE);
    DrawTriangle3D(vertex[7], vertex[6], vertex[2], color, TRUE);
}


void DrawCube3DWithTopPos(const VECTOR& top_pos, const float side_len, const unsigned int color)
{
    DrawCube3D(VSub(top_pos, VGet(0, 0, side_len / 2)), side_len, color);
}


void DrawHexagon(const std::array<VECTOR, 6>& vertex, const unsigned int color)
{
    // 3D描画の関数は3角形を基本単位とするので，
    // 6角形の面を張りたい場合は，4つの三角形を組み合わせる必要がある．
    DrawTriangle3D(vertex[0], vertex[1], vertex[5], color, TRUE);
    DrawTriangle3D(vertex[1], vertex[2], vertex[4], color, TRUE);
    DrawTriangle3D(vertex[1], vertex[4], vertex[5], color, TRUE);
    DrawTriangle3D(vertex[2], vertex[3], vertex[4], color, TRUE);
}


void DrawHexagonalPrism(const std::array<VECTOR, 6>& vertex, const float height,
                        const unsigned int color)
{
    // 6角形面の法線方向のベクトルを取得する．
    // やっている処理としては，頂点0から1へ行くベクトルをv01，
    // 同様に頂点0から2へ行くベクトルをv02とすると，
    // v01とv02の外積(Cross)をとると法線方向のベクトルが取得できるため，
    // これを単位ベクトルに変換(Norm，ノーマライズのこと)し，高さの半分だけ倍にした．
    const VECTOR center_to_top = VScale(VNorm(VCross(VSub(vertex[0], vertex[1]),
                                        VSub(vertex[0], vertex[2]))), height / 2.0f);

    // 上面の頂点．
    const std::array<VECTOR, 6> vertex_top =
    {
        VAdd(vertex[0], center_to_top),
        VAdd(vertex[1], center_to_top),
        VAdd(vertex[2], center_to_top),
        VAdd(vertex[3], center_to_top),
        VAdd(vertex[4], center_to_top),
        VAdd(vertex[5], center_to_top)
    };

    // 底面の頂点．
    const std::array<VECTOR, 6> vertex_bottom =
    {
        VSub(vertex[0], center_to_top),
        VSub(vertex[1], center_to_top),
        VSub(vertex[2], center_to_top),
        VSub(vertex[3], center_to_top),
        VSub(vertex[4], center_to_top),
        VSub(vertex[5], center_to_top)
    };

    DrawHexagon(vertex_top, color);  // 上面を描画する．
    DrawHexagon(vertex_bottom, color);  // 底面を描画する．

    // 側面を描画していく．側面は四角形6つで構成されるので，3角形が12個必要になる．
    for (int i = 0; i < 6; i++)
    {
        DrawTriangle3D(vertex_top[i % 6],
                       vertex_top[(i + 1) % 6],
                       vertex_bottom[i % 6],
                       color,
                       TRUE);

        DrawTriangle3D(vertex_top[(i + 1) % 6],
                       vertex_bottom[i % 6],
                       vertex_bottom[(i + 1) % 6],
                       color,
                       TRUE);
    }
}

}  // namespace designlab::dxlib_util
