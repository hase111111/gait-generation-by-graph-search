#include "Dxlib3DFunction.h"

#include "DxLib.h"


#include "graphic_const.h"


void myDxlib3DFunc::initDxlib3D()
{
	//カメラの描画範囲を設定する
	SetCameraNearFar(GraphicConst::CAMERA_NEAR, GraphicConst::CAMERA_FAR);

	SetUseLighting(FALSE);					// ライティングの計算をしないように設定を変更	
	SetUseBackCulling(FALSE);				// ポリゴンの両面を描画する．
	SetUseZBuffer3D(TRUE);					// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);				// Ｚバッファへの書き込みを有効にする
	SetFogEnable(FALSE);					// フォグは使用しない．
}


void myDxlib3DFunc::drawCube3D(const VECTOR _center_pos, const float _side_len, const unsigned int _color)
{
	//立方体は8つの頂点を持つので，それらの座標を計算する．
	const VECTOR _vertex[8] =
	{
		VGet(_center_pos.x - _side_len / 2,_center_pos.y - _side_len / 2,_center_pos.z - _side_len / 2),
		VGet(_center_pos.x + _side_len / 2,_center_pos.y - _side_len / 2,_center_pos.z - _side_len / 2),
		VGet(_center_pos.x + _side_len / 2,_center_pos.y - _side_len / 2,_center_pos.z + _side_len / 2),
		VGet(_center_pos.x - _side_len / 2,_center_pos.y - _side_len / 2,_center_pos.z + _side_len / 2),
		VGet(_center_pos.x - _side_len / 2,_center_pos.y + _side_len / 2,_center_pos.z - _side_len / 2),
		VGet(_center_pos.x + _side_len / 2,_center_pos.y + _side_len / 2,_center_pos.z - _side_len / 2),
		VGet(_center_pos.x + _side_len / 2,_center_pos.y + _side_len / 2,_center_pos.z + _side_len / 2),
		VGet(_center_pos.x - _side_len / 2,_center_pos.y + _side_len / 2,_center_pos.z + _side_len / 2)
	};

	// 3D描画の関数は3角形を基本単位とするので，4角形の面を張りたい場合は，2つの三角形を組み合わせる必要がある．つまり，6面×2つ＝12個の三角形で立方体が描画できる．

	DrawTriangle3D(_vertex[0], _vertex[1], _vertex[2], _color, TRUE);
	DrawTriangle3D(_vertex[2], _vertex[3], _vertex[0], _color, TRUE);

	DrawTriangle3D(_vertex[4], _vertex[5], _vertex[6], _color, TRUE);
	DrawTriangle3D(_vertex[6], _vertex[7], _vertex[4], _color, TRUE);

	DrawTriangle3D(_vertex[4], _vertex[7], _vertex[0], _color, TRUE);
	DrawTriangle3D(_vertex[0], _vertex[7], _vertex[3], _color, TRUE);

	DrawTriangle3D(_vertex[1], _vertex[2], _vertex[5], _color, TRUE);
	DrawTriangle3D(_vertex[5], _vertex[6], _vertex[2], _color, TRUE);

	DrawTriangle3D(_vertex[0], _vertex[1], _vertex[5], _color, TRUE);
	DrawTriangle3D(_vertex[5], _vertex[4], _vertex[0], _color, TRUE);

	DrawTriangle3D(_vertex[2], _vertex[3], _vertex[7], _color, TRUE);
	DrawTriangle3D(_vertex[7], _vertex[6], _vertex[2], _color, TRUE);

}


void myDxlib3DFunc::drawCube3DWithTopPos(const VECTOR _top_pos, const float _side_len, const unsigned int _color)
{
	drawCube3D(VSub(_top_pos, VGet(0, 0, _side_len / 2)), _side_len, _color);
}


void myDxlib3DFunc::drawHexagon(const VECTOR _vertex[6], const unsigned int _color)
{
	// 3D描画の関数は3角形を基本単位とするので，6角形の面を張りたい場合は，4つの三角形を組み合わせる必要がある．
	DrawTriangle3D(_vertex[0], _vertex[1], _vertex[5], _color, TRUE);
	DrawTriangle3D(_vertex[1], _vertex[2], _vertex[4], _color, TRUE);
	DrawTriangle3D(_vertex[1], _vertex[4], _vertex[5], _color, TRUE);
	DrawTriangle3D(_vertex[2], _vertex[3], _vertex[4], _color, TRUE);
}


void myDxlib3DFunc::drawHexagonalPrism(const VECTOR _vertex[6], const float _height, const unsigned int _color)
{
	//6角形面の法線方向のベクトルを取得する．やっている処理としては，頂点0から1へ行くベクトルをv01，同様に頂点0から2へ行くベクトルをv02とすると，
	// v01とv02の外積(Cross)をとると法線方向のベクトルが取得できるため，これを単位ベクトルに変換(Norm，ノーマライズのこと)し，高さの半分だけ倍にした．
	const VECTOR _center_to_top = VScale(VNorm(VCross(VSub(_vertex[0], _vertex[1]), VSub(_vertex[0], _vertex[2]))), _height / 2.0f);

	//上面の頂点．
	const VECTOR _vertex_top[6] =
	{
		VAdd(_vertex[0],_center_to_top),
		VAdd(_vertex[1],_center_to_top),
		VAdd(_vertex[2],_center_to_top),
		VAdd(_vertex[3],_center_to_top),
		VAdd(_vertex[4],_center_to_top),
		VAdd(_vertex[5],_center_to_top)
	};

	//底面の頂点
	const VECTOR _vertex_bottom[6] =
	{
		VSub(_vertex[0],_center_to_top),
		VSub(_vertex[1],_center_to_top),
		VSub(_vertex[2],_center_to_top),
		VSub(_vertex[3],_center_to_top),
		VSub(_vertex[4],_center_to_top),
		VSub(_vertex[5],_center_to_top)
	};

	drawHexagon(_vertex_top, _color);		//上面を描画する．
	drawHexagon(_vertex_bottom, _color);	//底面を描画する．

	//側面を描画していく．側面は四角形6つで構成されるので，3角形が12こ必要になる．
	for (int i = 0; i < 6; i++)
	{
		DrawTriangle3D(_vertex_top[i % 6], _vertex_top[(i + 1) % 6], _vertex_bottom[i % 6], _color, TRUE);
		DrawTriangle3D(_vertex_top[(i + 1) % 6], _vertex_bottom[i % 6], _vertex_bottom[(i + 1) % 6], _color, TRUE);
	}

}
