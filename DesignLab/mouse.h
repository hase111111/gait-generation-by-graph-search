//! @file mouse.h
//! @brief Dxlibのマウス入力を取得するクラス．


#ifndef DESIGNLAB_MOUSE_H_
#define DESIGNLAB_MOUSE_H_


#include "singleton.h"


//! @class Mouse
//! @brief Dxlibのマウス入力を取得するクラス．
//! @details 注意点はKeyboardクラスと全く同じ．そちらを参照して確認してほしい．

class Mouse final : public Singleton<Mouse>
{
public:

	//! @brief マウス入力を更新する．これを毎フレーム実行しないと，マウス入力を取得できない．
	void Update();

	//! @brief マウスカーソルの位置を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX座標．
	int cursor_pos_x() const { return cursor_pos_x_; };

	//! @brief マウスカーソルの移動量を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX方向の移動量．
	int GetDiffPosX() const;

	//! @brief マウスカーソルの位置を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY座標．
	int cursor_pos_y() const { return cursor_pos_y_; };

	//! @brief マウスカーソルの移動量を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY方向の移動量．
	int GetDiffPosY() const;

	//! @brief マウスカーソルの移動量を取得する．
	//! @return double マウスカーソルの移動量．
	double getDiffPos() const;

	//! @brief マウスの左クリックの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int left_pushing_counter() const { return left_pushing_counter_; };

	//! @brief マウスの右クリックの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int right_pushing_counter() const { return right_pushing_counter_; };

	//! @brief マウスのホイールボタンの押され続けているフレーム数を取得する．
	//! @return int 押されているフレーム数．
	int middle_pushing_counter() const { return middle_pushing_counter_; };

	//! @brief マウスの左クリックの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int left_releasing_counter() const { return left_releasing_counter_; };

	//! @brief マウスの右クリックの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int right_releasing_counter() const { return right_releasing_counter_; };

	//! @brief マウスのホイールボタンの離され続けているフレーム数を取得する．
	//! @return int 離されているフレーム数．
	int middle_releasing_counter() const { return middle_releasing_counter_; };

	//! @brief マウスのホイールの回転量を取得する．
	//! @n 1フレームで回転した量を取得する．
	//! @n 手前に回した分はマイナスの値として、奥に回した分はプラスの値として返る
	//! @return int マウスホイールの回転量．
	int wheel_rot() const { return wheel_rot_; };

private:

	Mouse();
	friend Singleton<Mouse>;


	int cursor_pos_x_, cursor_pos_y_;				//!< マウスカーソルの位置
	int cursor_past_pos_x_, cursor_past_pos_y_;		//!< 1フレーム前のマウスカーソルの位置

	int right_pushing_counter_, left_pushing_counter_, middle_pushing_counter_;			//!< マウスのボタンが押され続けているフレーム数
	int right_releasing_counter_, left_releasing_counter_, middle_releasing_counter_;	//!< マウスのボタンが離され続けているフレーム数

	int wheel_rot_;		//!< マウスホイールの回転量
};


#endif // !DESIGNLAB_MOUSE_H_