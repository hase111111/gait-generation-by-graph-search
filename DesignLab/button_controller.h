//! @file button_controller.h
//! @brief ボタンの処理，描画を管理するクラス．


#ifndef DESIGNLAB_BUTTON_CONTROLLER_H_
#define DESIGNLAB_BUTTON_CONTROLLER_H_


#include <string>


//! @class ButtomController
//! @brief ボタンの処理，描画を管理するクラス．
class ButtomController final
{
public:

	ButtomController();
	ButtomController(int x_pos, int y_pos, int x_size, int y_size, const std::string& text);
	~ButtomController() = default;

	//! @brief ボタンの状態を更新する．毎フレーム実行すること．
	void Update();

	//! @brief ボタンを描画する．デザインを変更したいならばここを変更してください．
	void Draw() const;


	//! @biref ボタンが押された瞬間にtrueを返す．
	//! @return bool ボタンが押された瞬間にtrueを返す．押され続けていても一度しかtrueを返さない．
	bool IsPushedNow() const;

	//! @brief ボタンが押されているならばtrueを返す．
	//! @return bool ボタンが押されているならばtrueを返す．
	bool IsPushed() const;

	//! @brief ボタンが何フレームに渡って押され続けているのかを返す．
	//! @return int ボタンが何フレームに渡って押され続けているのかを返す．整数値[フレーム]．60フレームで1秒．
	int GetPushingFlame() const;

private:

	const int kXPos, kYPos;			//!< ボタンの座標
	const int kXSize, kYSize;		//!< ボタンの横幅と縦幅

	bool is_mouse_in_button_;	//!< ボタンの中にマウスがいるか．
	bool is_pushed_;			//!< ボタンは押されているか．
	int pushing_frame_;			//!< ボタンが押され続けている時間．単位はフレーム．
	std::string text_;			//!< ボタンに書かれているテキスト
};


#endif // DESIGNLAB_BUTTON_CONTROLLER_H_