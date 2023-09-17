#include "graphic_main_sample.h"

#include "DxLib.h"

#include "graphic_const.h"


bool GraphicMainSample::Update()
{
	m_counter++;

	return true;
}


void GraphicMainSample::Draw() const
{
	//背景を白で染める．
	DrawBox(0, 0, mp_setting->window_size_x, mp_setting->window_size_y, GetColor(255, 255, 255), TRUE);

	//メッセージをウィンドウに表示する
	printfDx("GraphicMainではこのように，描画のみを行うdraw関数と，値の更新のみをおこなうupdate関数によってウィンドウを制御します\n\n");
	printfDx("起動してから...%lf秒", static_cast<double>(m_counter) / 60.0);
}
