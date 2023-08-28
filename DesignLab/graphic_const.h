#pragma once

#include <string>


//! @class GraphicConst
//! @date 2023/08/21
//! @author 長谷川
//! @brief 画像表示処理用の定数をまとめたもの．
//! @details 定数クラスの詳細はDefine.hを参照．@n 
//! @n このクラスの注意点としては，まず，WIN_X, WIN_Yに大きすぎる値を代入しないこと．また，16:9の画面比は崩さないほうがいいと思う．
//! @n 次にカメラに関する値を変更する時は慎重に行うこと，下げすぎると何も見えなくなるし，上げすぎると尋常じゃなく重くなります．
class GraphicConst final
{
public:
	const static std::string WIN_NAME;	//!< ウィンドウの名前．
	const static int COLOR_BIT;			//!< 色を表現するbit数．通常32で良いが軽くするなら16にする．

	const static int BACK_COLOR_R;		//!< ウィンドウ背景色． 赤色成分
	const static int BACK_COLOR_G;		//!< ウィンドウ背景色． 緑色成分
	const static int BACK_COLOR_B;		//!< ウィンドウ背景色． 青色成分

	const static float CAMERA_FAR;		//!< カメラが表示できる最も遠い座標．
	const static float CAMERA_NEAR;		//!< カメラが表示できる最も近い座標．

	const static float CAMERA_TO_TARGET_MAX;	//!< カメラと注視目標の最大距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．
	const static float CAMERA_TO_TARGET_MIN;	//!< カメラと注視目標の最小距離．CAMERA_FARとCAMERA_NEARの間の値じゃないとなにも表示されなくなる．

private:

	//コンストラクタとコピーコンストラクタを削除して実体を生成できないようにする．
	GraphicConst() = delete;
	GraphicConst(GraphicConst& other) = delete;
};


//! @file graphic_const.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief 画像表示処理用の定数．
//! @n 行数 : @lineinfo
