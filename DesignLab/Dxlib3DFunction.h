#pragma once
#include "Dxlib.h"
#include "MyVector.h"


namespace myDxlib3DFunc 
{
	//! @brief 3D処理を行う上で必要な初期化処理をまとめたもの．
	void initDxlib3D();

	//! @brief Dxlibの座標を示すVECTORと，このプログラムで使用しているSVectorを変換する．<br>ロボット座標系は右手座標系，Dxlibは左手座標系(工学は右手・ゲームライブラリは左手が多い気がする)なのでyを反転する．
	//! @param [in] _vec 変換前の座標．
	//! @return VECTOR 変換後の座標．
	inline VECTOR convertToDxVec(const my_vec::SVector& _vec) { return VGet(_vec.x, -_vec.y, _vec.z); }

	//! @brief 3D空間に立方体を描画する．
	//! @param [in] _center_pos 立方体の中心の座標．
	//! @param [in] _side_len 立方体の1辺の長さ．
	//! @param [in] _color 立方体の色．dxlibのGetColorで取得する.
	void drawCube3D(const VECTOR _center_pos, const float _side_len, const unsigned int _color);

	//! @brief 3D空間に立方体を描画する．立方体の上面の中心の座標から描画する．
	//! @param [in] _center_pos 立方体の上面の中心の座標．
	//! @param [in] _side_len 立方体の1辺の長さ．
	//! @param [in] _color 立方体の色．dxlibのGetColorで取得する.
	void drawCube3DWithTopPos(const VECTOR _top_pos, const float _side_len, const unsigned int _color);

	//! @brief 3D空間に六角形を描画する．
	//! @param [in] _vertex 各頂点の座標．
	//! @param [in] _color 色．dxlibのGetColorで取得する.
	void drawHexagon(const VECTOR _vertex[6], const unsigned int _color);

	//! @brief 3D空間に六角柱を描画する．
	//! @param _vertex 六角柱の真ん中の各頂点の座標，同一平面上にあるものにすること．
	//! @param _height 六角柱の高さ．
	//! @param _color 色．dxlibのGetColorで取得する.
	void drawHexagonalPrism(const VECTOR _vertex[6], const float _height, const unsigned int _color);
}


//! @file Dxlib3DFunction.h
//! @brief Dxlibの3D表示を行う処理を書き直した関数をまとめたもの．
//! @author 長谷川

//! @namespace myDxlib3DFunc
//! @brief Dxlibの3D表示を行う処理を書き直したもの．
//! @details Dxlib の 3Dで表示する機能はハッキリ言って死ぬほど使いづらいので，ここである程度使いやすくなるように書き直す．
//! @author 長谷川
