//! @file dxlib_util.h
//! @brief Dxlibの3D表示を行う処理を書き直した関数をまとめたもの．


#ifndef DESIGNLAB_DXLIB_H_
#define DESIGNLAB_DXLIB_H_

#include "Dxlib.h"

#include <array>

#include "designlab_vector.h"


namespace designlab
{

	//! @namespace dxlib_util
	//! @brief Dxlibの3D表示を行う処理を書き直したもの．
	//! @details Dxlib の 3Dで表示する機能はハッキリ言って死ぬほど使いづらいので，ここである程度使いやすくなるように処理を書いてまとめておく．

	namespace dxlib_util
	{
		//! @brief 3D処理を行う上で必要な初期化処理をまとめたもの．
		void InitDxlib3DSetting();


		//! @brief Dxlibの座標を示すVECTORと，このプログラムで使用しているVectorを変換する．
		//! @n ロボット座標系は右手座標系，Dxlibは左手座標系(工学は右手・ゲームライブラリは左手が多い気がする)なのでyを反転する．
		//! @param [in] vec 変換前の座標．
		//! @return VECTOR 変換後の座標．
		inline VECTOR ConvertToDxlibVec(const dl_vec::SVector& vec) { return VGet(vec.x, -vec.y, vec.z); }


		//! @brief このプログラムで使用しているVectorと，Dxlibの座標を示すVECTORを変換する．
		//! @n ロボット座標系は右手座標系，Dxlibは左手座標系(工学は右手・ゲームライブラリは左手が多い気がする)なのでyを反転する．
		//! @param [in] vec 変換前の座標．
		//! @return SVector 変換後の座標．
		inline dl_vec::SVector ConvertDesignLabVec(const VECTOR& vec) { return dl_vec::SVector(vec.x, -vec.y, vec.z); }


		//! @brief デフォルトだと描画処理を書いた順に描画されるが，これをZバッファを使用して奥行きを考慮して描画するようにする．
		//! @n なんか毎フレーム実行しなきゃいけないっぽい？仕様がよくわからん
		void SetZBufferEnable();


		//! @brief 3D空間に立方体を描画する．
		//! @param [in] center_pos 立方体の中心の座標．
		//! @param [in] side_len 立方体の1辺の長さ．
		//! @param [in] color 立方体の色．dxlibのGetColorで取得する.
		void DrawCube3D(const VECTOR& center_pos, float side_len, unsigned int color);


		//! @brief 3D空間に立方体を描画する．立方体の上面の中心の座標から描画する．
		//! @param [in] center_pos 立方体の上面の中心の座標．
		//! @param [in] side_len 立方体の1辺の長さ．
		//! @param [in] color 立方体の色．dxlibのGetColorで取得する.
		void DrawCube3DWithTopPos(const VECTOR& top_pos, float side_len, unsigned int color);


		//! @brief 3D空間に六角形を描画する．各点は同一平面上にあるものにしないと，描画が歪む．
		//! @param [in] vertex 各頂点の座標．
		//! @param [in] color 色．dxlibのGetColorで取得する.
		void DrawHexagon(const std::array<VECTOR, 6>& vertex, unsigned int color);


		//! @brief 3D空間に六角柱を描画する．
		//! @param vertex 六角柱の真ん中の各頂点の座標，同一平面上にあるものにすること．
		//! @param height 六角柱の高さ．
		//! @param color 色．dxlibのGetColorで取得する.
		void DrawHexagonalPrism(const std::array<VECTOR, 6>& vertex, float height, unsigned int color);

	} // namespace dxlib_util

} // namespace designlab


#endif // !DESIGNLAB_DXLIB_H_