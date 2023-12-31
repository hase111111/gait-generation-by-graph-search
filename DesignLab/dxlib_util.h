
//! @file      dxlib_util.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DXLIB_UTIL_H_
#define DESIGNLAB_DXLIB_UTIL_H_

#include <array>

#include <Dxlib.h>

#include "math_rotation_matrix.h"
#include "math_vector3.h"


//! @namespace designlab::dxlib_util
//! @brief Dxlibの3D表示を行う処理を書き直した関数をまとめた名前空間．
//! @details Dxlib の 3Dで表示する機能は使いづらいので，
//! ここである程度使いやすくなるように処理を書いてまとめておく．
namespace designlab::dxlib_util
{

//! @brief 3D処理を行う上で必要な初期化処理をまとめたもの．
//! @param[in] high_quality trueなら高品質モードで初期化する．
void InitDxlib3DSetting(bool high_quality);

//! @brief Dxlibの座標を示すVECTORと，このプログラムで使用しているVectorを変換する．
//! @n ロボット座標系は右手座標系，
//! Dxlibは左手座標系（工学は右手・ゲームライブラリは左手が多い）なのでyを反転する．
//! @param[in] vec 変換前の座標．
//! @return 変換後の座標．
inline VECTOR ConvertToDxlibVec(const Vector3& vec)
{
    return VGet(vec.x, -vec.y, vec.z);
}

inline MATRIX ConvertToDxlibMat(const RotationMatrix3x3& mat)
{
    return {
      mat.element[0][0], mat.element[0][1], mat.element[0][2], 0,
      mat.element[1][0], mat.element[1][1], mat.element[1][2], 0,
      mat.element[2][0], mat.element[2][1], mat.element[2][2], 0,
      0, 0, 0, 1
    };
}

//! @brief このプログラムで使用しているVectorと，Dxlibの座標を示す VECTOR を変換する．
//! @n ロボット座標系は右手座標系，
//! Dxlibは左手座標系(工学は右手・ゲームライブラリは左手が多い気がする)なので y を反転する．
//! @param[in] vec 変換前の座標．
//! @return 変換後の座標．
inline Vector3 ConvertDesignLabVec(const VECTOR& vec) { return Vector3(vec.x, -vec.y, vec.z); }


//! @brief デフォルトだと描画処理を書いた順に描画されるが，
//! これをZバッファを使用して奥行きを考慮して描画するようにする．
//! 毎フレーム実行する必要がある．
void SetZBufferEnable();


//! @brief 3D空間に立方体を描画する．
//! @param[in] center_pos 立方体の中心の座標．
//! @param[in] side_len 立方体の1辺の長さ．
//! @param[in] color 立方体の色．dxlib の GetColor で取得する.
void DrawCube3D(const VECTOR& center_pos, float side_len, unsigned int color);


//! @brief 3D空間に立方体を描画する．立方体の上面の中心の座標から描画する．
//! @param[in] center_pos 立方体の上面の中心の座標．
//! @param[in] side_len 立方体の1辺の長さ．
//! @param[in] color 立方体の色．dxlib の GetColor で取得する.
void DrawCube3DWithTopPos(const VECTOR& top_pos, float side_len, unsigned int color);


//! @brief 3D空間に六角形を描画する．各点は同一平面上にあるものにしないと，描画が歪む．
//! @param[in] vertex 各頂点の座標．
//! @param[in] color 色．dxlib の GetColor で取得する.
void DrawHexagon(const std::array<VECTOR, 6>& vertex, unsigned int color);


//! @brief 3D空間に六角柱を描画する．
//! @param vertex 六角柱の真ん中の各頂点の座標，同一平面上にあるものにすること．
//! @param height 六角柱の高さ．
//! @param color 色．dxlib の GetColor で取得する.
void DrawHexagonalPrism(const std::array<VECTOR, 6>& vertex, float height, unsigned int color);


}  // namespace designlab::dxlib_util


namespace DxLib
{
// VECTORの算術演算子を自作する．

constexpr VECTOR operator+(const VECTOR& vec1, const VECTOR& vec2)
{
    return { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
};

constexpr VECTOR operator-(const VECTOR& vec1, const VECTOR& vec2)
{
    return { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
};

constexpr VECTOR operator*(const VECTOR& vec, const float s)
{
    return { vec.x * s, vec.y * s, vec.z * s };
};

constexpr VECTOR operator*(const float s, const VECTOR& vec)
{
    return vec * s;
};

constexpr VECTOR operator/(const VECTOR& vec, const float s)
{
    return { vec.x / s, vec.y / s, vec.z / s };
};

}  // namespace DxLib


#endif  // DESIGNLAB_DXLIB_UTIL_H_
