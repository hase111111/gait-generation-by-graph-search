#pragma once

#include "Dxlib.h"

#include "designlab_vector.h"
#include "hexapod_const.h"


//! @namespace dl_dxlib
//! @date 2023/08/14
//! @author 長谷川
//! @brief Dxlibの3D表示を行う処理を書き直したもの．
//! @details Dxlib の 3Dで表示する機能はハッキリ言って死ぬほど使いづらいので，ここである程度使いやすくなるように書き直す．
namespace dl_dxlib
{
	//! @brief 3D処理を行う上で必要な初期化処理をまとめたもの．
	void initDxlib3D();


	//! @brief Dxlibの座標を示すVECTORと，このプログラムで使用しているSVectorを変換する．<br>ロボット座標系は右手座標系，Dxlibは左手座標系(工学は右手・ゲームライブラリは左手が多い気がする)なのでyを反転する．
	//! @param [in] vec 変換前の座標．
	//! @return VECTOR 変換後の座標．
	inline VECTOR convertToDxVec(const dl_vec::SVector& vec) { return VGet(vec.x, -vec.y, vec.z); }


	//! @brief デフォルトだと描画処理を書いた順に描画されるが，これをZバッファを使用して奥行きを考慮して描画するようにする．
	void setZBufferEnable();


	//! @brief 3D空間に立方体を描画する．
	//! @param [in] center_pos 立方体の中心の座標．
	//! @param [in] side_len 立方体の1辺の長さ．
	//! @param [in] color 立方体の色．dxlibのGetColorで取得する.
	void drawCube3D(const VECTOR& center_pos, const float side_len, const unsigned int color);


	//! @brief 3D空間に立方体を描画する．立方体の上面の中心の座標から描画する．
	//! @param [in] center_pos 立方体の上面の中心の座標．
	//! @param [in] side_len 立方体の1辺の長さ．
	//! @param [in] color 立方体の色．dxlibのGetColorで取得する.
	void drawCube3DWithTopPos(const VECTOR& top_pos, const float side_len, const unsigned int color);


	//! @brief 3D空間に六角形を描画する．
	//! @param [in] vertex 各頂点の座標．
	//! @param [in] color 色．dxlibのGetColorで取得する.
	void drawHexagon(const VECTOR vertex[HexapodConst::LEG_NUM], const unsigned int color);


	//! @brief 3D空間に六角柱を描画する．
	//! @param vertex 六角柱の真ん中の各頂点の座標，同一平面上にあるものにすること．
	//! @param height 六角柱の高さ．
	//! @param color 色．dxlibのGetColorで取得する.
	void drawHexagonalPrism(const VECTOR vertex[HexapodConst::LEG_NUM], const float height, const unsigned int color);

} // namespace dl_dxlib


//! @file designlab_dxlib.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief Dxlibの3D表示を行う処理を書き直した関数をまとめたもの．
//! @n 行数 : @lineinfo
