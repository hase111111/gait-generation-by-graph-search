#include "dxlib_util.h"

#include "DxLib.h"

#include "graphic_const.h"


namespace designlab
{
	namespace dxlib_util
	{

		void InitDxlib3DSetting()
		{
			//カメラの描画範囲を設定する
			SetCameraNearFar(GraphicConst::CAMERA_NEAR, GraphicConst::CAMERA_FAR);

			SetUseLighting(FALSE);			// ライティングの計算をしないように設定を変更	
			SetUseBackCulling(FALSE);		// ポリゴンの両面を描画する．
			SetFogEnable(FALSE);			// フォグは使用しない．
		}


		void SetZBufferEnable()
		{
			// Ｚバッファを有効にする
			SetUseZBuffer3D(TRUE);

			// Ｚバッファへの書き込みを有効にする
			SetWriteZBuffer3D(TRUE);
		}


		void DrawCube3D(const VECTOR& center_pos, const float side_len, const unsigned int color)
		{
			//立方体は8つの頂点を持つので，それらの座標を計算する．
			const std::array<VECTOR, 8> kVertex =
			{
				VGet(center_pos.x - side_len / 2,center_pos.y - side_len / 2,center_pos.z - side_len / 2),
				VGet(center_pos.x + side_len / 2,center_pos.y - side_len / 2,center_pos.z - side_len / 2),
				VGet(center_pos.x + side_len / 2,center_pos.y - side_len / 2,center_pos.z + side_len / 2),
				VGet(center_pos.x - side_len / 2,center_pos.y - side_len / 2,center_pos.z + side_len / 2),
				VGet(center_pos.x - side_len / 2,center_pos.y + side_len / 2,center_pos.z - side_len / 2),
				VGet(center_pos.x + side_len / 2,center_pos.y + side_len / 2,center_pos.z - side_len / 2),
				VGet(center_pos.x + side_len / 2,center_pos.y + side_len / 2,center_pos.z + side_len / 2),
				VGet(center_pos.x - side_len / 2,center_pos.y + side_len / 2,center_pos.z + side_len / 2)
			};

			// 3D描画の関数は3角形を基本単位とするので，4角形の面を張りたい場合は，2つの三角形を組み合わせる必要がある．つまり，6面×2つ＝12個の三角形で立方体が描画できる．

			DrawTriangle3D(kVertex[0], kVertex[1], kVertex[2], color, TRUE);
			DrawTriangle3D(kVertex[2], kVertex[3], kVertex[0], color, TRUE);

			DrawTriangle3D(kVertex[4], kVertex[5], kVertex[6], color, TRUE);
			DrawTriangle3D(kVertex[6], kVertex[7], kVertex[4], color, TRUE);

			DrawTriangle3D(kVertex[4], kVertex[7], kVertex[0], color, TRUE);
			DrawTriangle3D(kVertex[0], kVertex[7], kVertex[3], color, TRUE);

			DrawTriangle3D(kVertex[1], kVertex[2], kVertex[5], color, TRUE);
			DrawTriangle3D(kVertex[5], kVertex[6], kVertex[2], color, TRUE);

			DrawTriangle3D(kVertex[0], kVertex[1], kVertex[5], color, TRUE);
			DrawTriangle3D(kVertex[5], kVertex[4], kVertex[0], color, TRUE);

			DrawTriangle3D(kVertex[2], kVertex[3], kVertex[7], color, TRUE);
			DrawTriangle3D(kVertex[7], kVertex[6], kVertex[2], color, TRUE);

		}


		void DrawCube3DWithTopPos(const VECTOR& top_pos, const float side_len, const unsigned int color)
		{
			DrawCube3D(VSub(top_pos, VGet(0, 0, side_len / 2)), side_len, color);
		}


		void DrawHexagon(const std::array<VECTOR, 6>& vertex, const unsigned int color)
		{
			// 3D描画の関数は3角形を基本単位とするので，6角形の面を張りたい場合は，4つの三角形を組み合わせる必要がある．
			DrawTriangle3D(vertex[0], vertex[1], vertex[5], color, TRUE);
			DrawTriangle3D(vertex[1], vertex[2], vertex[4], color, TRUE);
			DrawTriangle3D(vertex[1], vertex[4], vertex[5], color, TRUE);
			DrawTriangle3D(vertex[2], vertex[3], vertex[4], color, TRUE);
		}


		void DrawHexagonalPrism(const std::array<VECTOR, 6>& vertex, const float height, const unsigned int color)
		{
			// 6角形面の法線方向のベクトルを取得する．やっている処理としては，頂点0から1へ行くベクトルをv01，同様に頂点0から2へ行くベクトルをv02とすると，
			// v01とv02の外積(Cross)をとると法線方向のベクトルが取得できるため，これを単位ベクトルに変換(Norm，ノーマライズのこと)し，高さの半分だけ倍にした．
			const VECTOR kCenterToTop = VScale(VNorm(VCross(VSub(vertex[0], vertex[1]), VSub(vertex[0], vertex[2]))), height / 2.0f);

			//上面の頂点．
			const std::array<VECTOR, 6> kVertexTop =
			{
				VAdd(vertex[0],kCenterToTop),
				VAdd(vertex[1],kCenterToTop),
				VAdd(vertex[2],kCenterToTop),
				VAdd(vertex[3],kCenterToTop),
				VAdd(vertex[4],kCenterToTop),
				VAdd(vertex[5],kCenterToTop)
			};

			//底面の頂点
			const std::array<VECTOR, 6> kVertexBottom =
			{
				VSub(vertex[0],kCenterToTop),
				VSub(vertex[1],kCenterToTop),
				VSub(vertex[2],kCenterToTop),
				VSub(vertex[3],kCenterToTop),
				VSub(vertex[4],kCenterToTop),
				VSub(vertex[5],kCenterToTop)
			};

			DrawHexagon(kVertexTop, color);		//上面を描画する．
			DrawHexagon(kVertexBottom, color);	//底面を描画する．

			//側面を描画していく．側面は四角形6つで構成されるので，3角形が12個必要になる．
			for (int i = 0; i < HexapodConst::LEG_NUM; i++)
			{
				DrawTriangle3D(kVertexTop[i % HexapodConst::LEG_NUM], kVertexTop[(i + 1) % HexapodConst::LEG_NUM], kVertexBottom[i % HexapodConst::LEG_NUM], color, TRUE);
				DrawTriangle3D(kVertexTop[(i + 1) % HexapodConst::LEG_NUM], kVertexBottom[i % HexapodConst::LEG_NUM], kVertexBottom[(i + 1) % HexapodConst::LEG_NUM], color, TRUE);
			}

		}

	}	//namespace dxlib_util

}	//namespace designlab
