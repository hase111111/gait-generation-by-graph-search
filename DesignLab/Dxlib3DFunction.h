#pragma once
#include "Dxlib.h"
#include "vectorFunc.h"

// Dxlib の 3Dで表示する機能はハッキリ言って死ぬほど使いづらいので，ここである程度使いやすくなるように書き直す．
namespace myDxlib3DFunc 
{
	// 3D処理を行う上で必要な初期化処理をまとめたもの．
	void initDxlib3D();

	// Dxlibの座標を示すVECTORと，このプログラムで使用しているSVectorを変換する．
	inline VECTOR convertToDxVec(const myvector::SVector& _vec) { return VGet(_vec.x, _vec.y, _vec.z); }

	// 3D空間に立方体を描画する．_center_pos…立方体の中心の座標．_side_len…立方体の1辺の長さ．_color…立方体の色．dxlibのGetColorで取得する.
	void drawCube3D(const VECTOR _center_pos, const float _side_len, const unsigned int _color);

	// 3D空間に立方体を描画する．_center_pos…立方体の上面の中心の座標．_side_len…立方体の1辺の長さ．_color…立方体の色．dxlibのGetColorで取得する.
	void drawCube3DWithTopPos(const VECTOR _top_pos, const float _side_len, const unsigned int _color);

	// 3D空間に六角形を描画する．_vertex…各頂点の座標．_color…色．dxlibのGetColorで取得する.
	void drawHexagon(const VECTOR _vertex[6], const unsigned int _color);

	// 3D空間に六角柱を描画する．
	//_vertex…六角柱の真ん中の各頂点の座標，同一平面上にあるものにすること．_height…六角柱の高さ．_color…色．dxlibのGetColorで取得する.
	void drawHexagonalPrism(const VECTOR _vertex[6], const float _height, const unsigned int _color);
}
