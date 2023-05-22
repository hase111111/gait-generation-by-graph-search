#include "Dxlib3DFunction.h"
#include "GraphicConst.h"
#include "DxLib.h"

void myDxlib3DFunc::initDxlib3D()
{
	//カメラの描画範囲を設定する
	SetCameraNearFar(GraphicConst::CAMERA_NEAR, GraphicConst::CAMERA_FAR);	

	SetUseLighting(FALSE);					// ライティングの計算をしないように設定を変更	
	SetUseBackCulling(FALSE);				// ポリゴンの両面を描画する．
	SetUseZBuffer3D(TRUE);					// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);				// Ｚバッファへの書き込みを有効にする
	SetFogEnable(FALSE);					// フォグは使用しない．
	SetBackgroundColor(0xDA, 0xEC, 0xED);	// 背景色の設定
}
