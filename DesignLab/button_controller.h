#pragma once

#include <string>


//! @class ButtomController
//! @date 2023/08/08
//! @author 長谷川
//! @brief ボタンの処理，描画を管理するクラス．
class ButtomController final
{
public:

	ButtomController();
	ButtomController(const int xpos, const int ypos, const int xsize, const int ysize, const std::string text);
	~ButtomController() = default;

	//! @brief ボタンの状態を更新する．毎フレーム実行すること．
	void Update();

	//! @brief ボタンを描画する．デザインを変更したいならばここを変更してください．
	void Draw() const;


	//! @biref ボタンが押された瞬間にtrueを返す．
	//! @return bool ボタンが押された瞬間にtrueを返す．押され続けていても一度しかtrueを返さない．
	bool isPushedNow() const;

	//! @brief ボタンが押されているならばtrueを返す．
	//! @return bool ボタンが押されているならばtrueを返す．
	bool isPushed() const;

	//! @brief ボタンが何フレームに渡って押され続けているのかを返す．
	//! @return int ボタンが何フレームに渡って押され続けているのかを返す．整数値[フレーム]．60フレームで1秒．
	int getPushingFlame() const;

private:

	bool m_is_mouse_in_buttom = false;	//!< ボタンの中にマウスがいるか．
	bool m_is_pushed = false;			//!< ボタンは押されているか．
	int m_pushing_frame = 0;			//!< ボタンが押され続けている時間．単位はフレーム．

	const int kXPos, kYPos;			//!< ボタンの座標
	const int kXSize, kYSize;		//!< ボタンの横幅と縦幅
	std::string m_text;				//!< ボタンに書かれているテキスト
};

//! @file button_controller.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief ボタンの処理，描画を管理するクラス．
//! @n 行数 : @lineinfo
