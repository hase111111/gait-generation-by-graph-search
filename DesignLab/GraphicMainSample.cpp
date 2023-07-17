#include "GraphicMainSample.h"
#include "DxLib.h"
#include "GraphicConst.h"

bool GraphicMainSample::update()
{
	m_counter++;

	return true;
}

void GraphicMainSample::draw() const
{
	//背景を白で染める．
	DrawBox(0, 0, GraphicConst::WIN_X, GraphicConst::WIN_Y, GetColor(255, 255, 255), TRUE);

	//メッセージをウィンドウに表示する
	printfDx("GraphicMainではこのように，描画のみを行うdraw関数と，値の更新のみをおこなうupdate関数によってウィンドウを制御します\n\n");
	printfDx("起動してから...%lf秒", (double)m_counter / 60.0);
}
