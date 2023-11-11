//! @file mouse.h
//! @brief Dxlibのマウス入力を取得するクラス．


#ifndef DESIGNLAB_MOUSE_H_
#define DESIGNLAB_MOUSE_H_


#include <array>
#include <map>

#include "singleton.h"


//! @class Mouse
//! @brief Dxlibのマウス入力を取得するクラス．
//! @details 注意点はKeyboardクラスと全く同じ．そちらを参照して確認してほしい．
class Mouse final : public Singleton<Mouse>
{
public:

	//! @brief マウス入力を更新する．これを毎フレーム実行しないと，マウス入力を取得できない．
	void Update();

	//! @brief mouseCodeのボタンが押されているフレーム数を取得する．
	//! @param [in] mouseCode どのボタンを調べたいか．
	//! @n 例えば右クリックだったら，MOUSE_INPUT_RIGHTとなる．
	//! @return int 押されているフレーム数．異常なmouse_codeが渡された場合は-1を返す．
	int GetPressingCount(int mouse_code) const;

	//! @brief mouseCodeのボタンが離されているフレーム数を取得する．
	//! @param [in] mouseCode どのボタンを調べたいか．
	//! @n 例えば右クリックだったら，MOUSE_INPUT_RIGHTとなる．
	//! @return int 離されているフレーム数．異常なmouse_codeが渡された場合は-1を返す．
	int GetReleasingCount(int mouse_code) const;

	//! @brief マウスカーソルの位置を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX座標．
	inline int GetCursorPosX() const { return cursor_pos_x_; };

	//! @brief マウスカーソルの移動量を取得する．X座標は画面の左端を0として，右向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのX方向の移動量．
	int GetDiffPosX() const;

	//! @brief マウスカーソルの位置を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY座標．
	inline int GetCursorPosY() const { return cursor_pos_y_; };

	//! @brief マウスカーソルの移動量を取得する．Y座標は画面の上端を0として，下向きが正．これはDxlibの仕様なので変更不能．
	//! @return int マウスカーソルのY方向の移動量．
	int GetDiffPosY() const;

	//! @brief マウスカーソルの移動量を取得する．
	//! @return double マウスカーソルの移動量．
	double GetDiffPos() const;

	//! @brief マウスのホイールの回転量を取得する．
	//! @n 1フレームで回転した量を取得する．
	//! @n 手前に回した分はマイナスの値として、奥に回した分はプラスの値として返る
	//! @return int マウスホイールの回転量．
	inline int GetWheelRot() const { return wheel_rot_; };

private:

	Mouse();
	friend Singleton<Mouse>;

	constexpr static int kMouseKeyNum = 8;		//!< マウスのボタンの数．右・左・中央 + 5つのボタン．

	const std::array<int, kMouseKeyNum> kMouseKeyCodes;	//!< マウスのボタンのキーコード

	int cursor_pos_x_, cursor_pos_y_;			//!< マウスカーソルの位置
	int cursor_past_pos_x_, cursor_past_pos_y_;	//!< 1フレーム前のマウスカーソルの位置

	std::map<int, int> pushing_counter_;	//!< マウスのボタンが押され続けているフレーム数．
	std::map<int, int> releasing_counter_;	//!< マウスのボタンが離され続けているフレーム数．

	int wheel_rot_;		//!< マウスホイールの回転量，手前に回した分はマイナスの値として、奥に回した分はプラスの値として返る
};


#endif // DESIGNLAB_MOUSE_H_