//! @file graphic_const.h
//! @brief 画像表示処理用の定数．

#ifndef DESIGNLAB_GRAPHIC_CONST_H_
#define DESIGNLAB_GRAPHIC_CONST_H_

#include <string>

#include "cassert_define.h"


//! @class GraphicConst
//! @brief 画像表示処理用の定数をまとめたもの．
//! @details 定数クラスの詳細はdefine.hを参照．
//! @n カメラに関する値を変更する時は慎重に行うこと，下げすぎると何も見えなくなるし，上げすぎると尋常じゃなく重くなる．
//! @n 値を編集したい場合は graphic_const cpp を編集すること．
class GraphicConst final
{
public:

	//コンストラクタを削除して，実体を生成できないようにする．
	GraphicConst() = delete;
	GraphicConst(GraphicConst& other) = delete;
	GraphicConst(GraphicConst&& other) = delete;
	GraphicConst& operator=(GraphicConst& other) = delete;


	const static std::string kWindowName;	//!< ウィンドウの名前．
	const static int kColorBit;				//!< 色を表現するbit数．通常32で良いが軽くするなら16にする．

	const static int kBackColorRed;		//!< ウィンドウ背景色． 赤色成分．0～255の範囲で指定．
	const static int kBackColorGreen;	//!< ウィンドウ背景色． 緑色成分．0～255の範囲で指定．
	const static int kBackColorBlue;	//!< ウィンドウ背景色． 青色成分．0～255の範囲で指定．

	const static float kCameraFar;		//!< カメラが表示できる最も遠い座標までの距離．
	const static float kCameraNear;		//!< カメラが表示できる最も近い座標までの距離．

	const static float kCameraToTargetMax;	//!< カメラと注視目標の最大距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．
	const static float kCameraToTargetMin;	//!< カメラと注視目標の最小距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．
};


#endif // !DESIGNLAB_GRAPHIC_CONST_H_