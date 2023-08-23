#pragma once

#include "singleton.h"


//! @class Mouse
//! @date 2023/08/07
//! @author 長谷川
//! @brief Dxlibのマウス入力を取得するクラス．
//! @details 注意点はKeyboardクラスと全く同じ．そちらを参照して確認してほしい．
class Mouse final : public Singleton<Mouse>
{
public:

	//! @brief マウス入力を更新する．これを毎フレーム実行しないと，マウス入力を取得できない．
	void update();

	//! @brief マウスカーソルの位置を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX座標．
	int getPosX() const;

	//! @brief マウスカーソルの移動量を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX方向の移動量．
	int getDiffPosX() const;

	//! @brief マウスカーソルの位置を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY座標．
	int getPosY() const;

	//! @brief マウスカーソルの移動量を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY方向の移動量．
	int getDiffPosY() const;

	//! @brief マウスカーソルの移動量を取得する．
	//! @return double マウスカーソルの移動量．
	double getDiffPos() const;

	//! @brief マウスの左クリックの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int getPushingCountLeft() const;

	//! @brief マウスの右クリックの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int getPushingCountRight() const;

	//! @brief マウスのホイールボタンの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int getPushingCountMiddle() const;

	//! @brief マウスの左クリックの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int getReleasingCountLeft() const;

	//! @brief マウスの右クリックの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int getReleasingCountRight() const;

	//! @brief マウスのホイールボタンの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int getReleasingCountMiddle() const;

	//! @brief マウスのホイールの回転量を取得する．
	//! @n 1フレームで回転した量を取得する．
	//! @n 手前に回した分はマイナスの値として、奥に回した分はプラスの値として返る
	//! @return int マウスホイールの回転量．
	int getWheelRot() const;

private:

	Mouse();
	friend Singleton<Mouse>;

	//クリックカウンタ
	int m_posx, m_poy;
	int m_past_posx, m_past_posy;
	int m_pushing_count_right, m_pushing_count_left, m_pushing_count_middle;
	int m_releasing_count_right, m_releasing_count_left, m_releasing_count_middle;
	int m_wheel_rot;
};


//! @file mouse.h
//! @date 2023/08/07
//! @author 長谷川
//! @brief Dxlibのマウス入力を取得するクラス．
//! @n 行数 : @lineinfo
