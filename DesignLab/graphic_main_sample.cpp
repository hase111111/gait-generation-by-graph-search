#include "graphic_main_sample.h"

#include "DxLib.h"

#include "graphic_const.h"
#include "keyboard.h"
#include "mouse.h"


GraphicMainSample::GraphicMainSample(const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr) :
	kBoxSizeX(setting_ptr ? setting_ptr->window_size_x : 100),
	kBoxSizeY(setting_ptr ? setting_ptr->window_size_y : 100),
	counter_(0)
{
}

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
	DrawBox(0, 0, kBoxSizeX, kBoxSizeY, GetColor(255, 255, 255), TRUE);

	//メッセージをウィンドウに表示する
	printfDx("GraphicMainではこのように，描画のみを行うdraw関数と，値の更新のみをおこなうupdate関数によってウィンドウを制御する\n\n");
	printfDx("起動してから...%lf秒", static_cast<double>(counter_) / 60.0);
	printfDx("\n\nXキーかマウスの左クリックで終了する．");
}
