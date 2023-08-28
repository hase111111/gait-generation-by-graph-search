#pragma once

#include <list>


//! @class Fps
//! @brief FPSを一定にキープするためのクラス．
//! @details 基本的には何もしなくともFPSは一定になるのだけど，144Fpsとか240Fpsくらいのゲーミングモニターを使っている場合，画面がおかしくなることがある．
//! @n よってこのクラスを使ってFPSを制御する．FPSは Frames per Second：1秒当たりの画面更新回数のこと．First Person Shootingのことではない．
//! @n 当方のPCのモニターの問題なので，必要ないならば消してしまってもかまわないが，あったところで問題はないので放置しておいてもらって構わない．@n
//! @n [参考] 
//! @n ・FPSの制御を行う https://dixq.net/rp2/07.html ←このサイトのプログラムを参考にしているが，機能を色々追加している．
//! @n ・DxlibのFPSとリフレッシュレートについて https://dixq.net/forum/viewtopic.php?t=20224
class Fps final
{
public:
	Fps() = delete;
	Fps(const int fps_);
	~Fps() = default;

	//! @brief 処理が早すぎる場合，FPSを一定にするために待つ．
	void wait();

	//! @brief 60Hz以上のモニター使用時に処理が詰まって画面がちらつかないように，描画処理をスキップするかどうかを判定する．
	//! @return bool 処理が詰まって描画を飛ばしたいときにtrueを返す．
	bool skipDrawScene();

private:

	//現在の時刻を記録する関数
	void regist(const int now_time);

	//! @brief どれだけ待てばよいか返す関数．
	//! @param [out] time 待つ時間
	//! @return bool コマ落ちしている場合はfalse
	bool getWaitTime(int* time) const;

	// 目標のFPSが正しいかどうかを判定する関数
	bool targetFpsIsVaild() const;


	const int TARGET_FPS;				//目標のFPS

	const int ONE_FRAME_MILLI_SECOND;	//１フレーム当たりにかかる時間(ミリ秒)

	const int LIST_MAX;					//リストに2秒分のデータをしまっておく


	std::list<int> m_list;

	bool m_do_skip_draw = false;   //コマ落ちを実装するためのフラグ
};


//! @file fps.h
//! @date 2023/08/07
//! @author 長谷川
//! @brief Fpsを一定に保つクラス．
//! @n 行数 : @lineinfo
