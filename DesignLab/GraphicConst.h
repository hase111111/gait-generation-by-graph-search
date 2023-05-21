#pragma once
#include <string>

class GraphicConst final
{
public:
	const static std::string WIN_NAME;	//ウィンドウの名前．
	const static int WIN_X;				//ウィンドウの横幅．dxlibではウィンドウの横方向に，右向きを正として X 軸をとります．
	const static int WIN_Y;				//ウィンドウの縦幅．dxlibではウィンドウの縦方向に，下向きを正として Y 軸をとります．
	const static int COLOR_BIT;			//色を表現するbit数．通常32で良いが軽くするなら16にする．

	//ウィンドウのFPS(フレーム / 秒．秒間画面を何回更新するか．TVは30fps，Nintendo Switchは60fps)．
	//60より大きい値にしても意味はない．30とかにしてもいいけどそこまで処理負荷は変わらないと思う．
	constexpr static unsigned int GRAPHIC_FPS = 60;

private:

	//コンストラクタとコピーコンストラクタを削除して実体を生成できないようにする．
	GraphicConst() = delete;
	GraphicConst(GraphicConst& _other) = delete;
};