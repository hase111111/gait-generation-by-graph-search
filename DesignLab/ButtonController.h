#pragma once
#include <string>

// UIのボタンの操作を行う関数．
class ButtomController final
{
private:
	bool m_is_mouse_in_buttom = false;	//ボタンの中にマウスがいるか．
	bool m_is_pushed = false;			//ボタンは押されているか．
	int m_pushing_frame = 0;			//ボタンが押され続けている時間．
	
	const int X_POS, Y_POS;	//ボタンの座標．
	const int X_SIZE, Y_SIZE;		//ボタンの横幅と縦幅
	std::string m_text;				//ボタンに書かれているテキスト．

public:
	ButtomController();
	ButtomController(const int _xpos, const int _ypos, const int _xsize, const int _ysize, const std::string _text);
	~ButtomController() = default;

	void update();				//ボタンの状態を更新する．毎フレーム実行すること．
	void draw() const;			//ボタンを描画する．デザインを変更したいならばここを変更してください．

	bool isPushedNow() const;	//ボタンが押された瞬間にtrueを返す．
	bool isPushed() const;		//ボタンが押されているならばtrueを返す．
	int getPushingFlame() const;//ボタンが何フレームに渡って押され続けているのかを返す．

};
