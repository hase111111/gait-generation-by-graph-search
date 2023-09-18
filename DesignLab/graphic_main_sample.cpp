#include "graphic_main_sample.h"

#include "DxLib.h"

#include "graphic_const.h"
#include "keyboard.h"
#include "mouse.h"


bool GraphicMainSample::Update()
{
	counter_++;

	if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_X) == 1 || Mouse::GetIns()->left_pushing_counter() == 1)
	{
		return false;	// Xキーかマウスの左クリックが押されたらループを抜ける．
	}

	return true;	// ループを抜けない限りtrueを返す．
}


void GraphicMainSample::Draw() const
{
	//背景を白で染める．
	DrawBox(0, 0, mp_setting->window_size_x, mp_setting->window_size_y, GetColor(255, 255, 255), TRUE);

	//メッセージをウィンドウに表示する
	printfDx("GraphicMainではこのように，描画のみを行うdraw関数と，値の更新のみをおこなうupdate関数によってウィンドウを制御する\n\n");
	printfDx("起動してから...%lf秒", double{ counter_ } / 60.0);
	printfDx("\n\nXキーかマウスの左クリックで終了する．");
}
