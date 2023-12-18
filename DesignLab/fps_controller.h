//! @file fps_controller.h
//! @brief FPSを一定にキープするためのクラス．

#ifndef DESIGNLAB_FPS_CONTROLLER_H_
#define DESIGNLAB_FPS_CONTROLLER_H_

#include <list>


namespace designlab
{

//! @class FpsController
//! @brief FPSを一定にキープするためのクラス．
//! @details 基本的には何もしなくともFPSは一定になるのだけど，144Fpsとか240Fpsくらいのゲーミングモニターを使っている場合，画面がおかしくなることがある．
//! @n よってこのクラスを使ってFPSを制御する．FPSは Frames per Second：1秒当たりの画面更新回数のこと．First Person Shootingのことではない．
//! @n 当方のPCのモニターの問題なので，必要ないならば消してしまってもかまわないが，あったところで問題はないので放置しておいてもらって構わない．
//! @n
//! @n [参考] 
//! @li FPSの制御を行う https://dixq.net/rp2/07.html ←このサイトのプログラムを参考にしているが，機能を色々追加している．
//! @li DxlibのFPSとリフレッシュレートについて https://dixq.net/forum/viewtopic.php?t=20224
class FpsController final
{
public:
	FpsController() = delete;
	FpsController(int target_fps);

	//! @brief 処理が早すぎる場合，FPSを一定にするために待つ．
	void Wait();

	//! @brief 60Hz以上のモニター使用時に処理が詰まって画面がちらつかないように，描画処理をスキップするかどうかを判定する．
	//! @return bool 処理が詰まって描画を飛ばしたいときにtrueを返す．その後フラグをfalseにする．
	bool SkipDrawScene();

	void DrawFps() const;

private:

	//! @brief 現在の時刻を記録する関数．
	//! @param [in] now_time 現在の時刻(ミリ秒) 
	void RegistTime(int now_time);

	//! @brief どれだけ待てばよいか返す関数．また，コマ落ちしているかどうかも判定する．
	//! @n 機能が2つあるので分けたほうがよいが，めんどいので直していない．
	//! @param [out] wait_time 待つべき時間 [msec]．
	//! @return bool コマ落ちしている場合はfalse．
	bool CheckNeedSkipDrawScreen(int* wait_time) const;

	//! @brief 目標のFPSが正しいかどうかを判定する関数．
	//! @return bool 負の値，または60より大きい値であればfalse．
	bool TargetFpsIsVaild() const;


	const int kTargetFpsValue;	//!< 目標のFPS，推奨値は 60 or 30．Dxlibの使用上 60 を上回る値を推奨しない．

	const int kOneFrameTime;	//!< 1フレーム当たりにかかる時間(ミリ秒)

	const int kListMax;			//!< リストに2秒分のフレームごとにかかった時間を記録するため，リストの最大サイズを決める．


	std::list<int> time_list_;		//!< 1フレームごとにかかった時間を記録するリスト．

	bool need_skip_draw_screen_;	//!< コマ落ちを実装するためのフラグ．trueであれば 1フレーム描画を飛ばし，その後フラグを折る
};

} // namespace designlab


#endif // DESIGNLAB_FPS_CONTROLLER_H_