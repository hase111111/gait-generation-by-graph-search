#pragma once
#include "Singleton.h"

class Mouse final : public Singleton<Mouse> 
{
private:
	Mouse();
	friend Singleton<Mouse>;

public:

	//! @brief マウス入力を更新する．これを毎フレーム実行しないと，マウス入力を取得できない．
	void update();	

	//! @brief マウスカーソルの位置を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX座標．
	int getPosX() const;

	//! @brief マウスカーソルの位置を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY座標．
	int getPosY() const;

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

private:

	//クリックカウンタ
	int m_posx, m_poy;
	int m_pushing_count_right, m_pushing_count_left, m_pushing_count_middle;
	int m_releasing_count_right, m_releasing_count_left, m_releasing_count_middle;
};


//! @file Mouse.h
//! @brief Dxlibのマウス入力を取得するクラスの実装．
//! @author 長谷川

//! @class Mouse
//! @brief Dxlibのマウス入力を取得するクラス．
//! @details 注意点はKeyboardクラスと全く同じ．そちらを参照して確認してほしい．
//! @author 長谷川
