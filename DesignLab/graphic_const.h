//! @file graphic_const.h
//! @brief 画像表示処理用の定数．

#ifndef DESIGNLAB_GRAPHIC_CONST_H_
#define DESIGNLAB_GRAPHIC_CONST_H_

#include <string>


//! @class GraphicConst
//! @brief 画像表示処理用の定数をまとめたもの．
//! @details 定数クラスの詳細はdefine.hを参照．
//! @n カメラに関する値を変更する時は慎重に行うこと，下げすぎると何も見えなくなるし，上げすぎると尋常じゃなく重くなります．

class GraphicConst final
{
public:

	//コンストラクタを削除して，実体を生成できないようにする．
	GraphicConst() = delete;
	GraphicConst(GraphicConst& other) = delete;
	GraphicConst(GraphicConst&& other) = delete;
	GraphicConst& operator=(GraphicConst& other) = delete;


	const static std::string WIN_NAME;	//!< ウィンドウの名前．
	const static int COLOR_BIT;			//!< 色を表現するbit数．通常32で良いが軽くするなら16にする．

	const static int BACK_COLOR_R;		//!< ウィンドウ背景色． 赤色成分．0～255の範囲で指定．
	const static int BACK_COLOR_G;		//!< ウィンドウ背景色． 緑色成分．0～255の範囲で指定．
	const static int BACK_COLOR_B;		//!< ウィンドウ背景色． 青色成分．0～255の範囲で指定．

	const static float CAMERA_FAR;		//!< カメラが表示できる最も遠い座標．
	const static float CAMERA_NEAR;		//!< カメラが表示できる最も近い座標．

	const static float CAMERA_TO_TARGET_MAX;	//!< カメラと注視目標の最大距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．
	const static float CAMERA_TO_TARGET_MIN;	//!< カメラと注視目標の最小距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．
};


#endif // !DESIGNLAB_GRAPHIC_CONST_H_