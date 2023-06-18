//! @file Fps.h
//! @brief Fpsクラスの実装．
//! @author 長谷川

#pragma once
#include <list>
#include "GraphicConst.h"

//! @class Fps
//! @brief FPSを一定にキープするためのクラス．
//! @details 基本的には何もしなくともFPSは一定になるのだけど，144Fpsとか240Fpsくらいのゲーミングモニターを使っている場合，画面がおかしくなることがある．
//! よってこのクラスを使ってFPSを制御する．FPSは Frames per Second：1秒当たりの画面更新回数のこと．First Person Shootingのことではない．<br>
//! 当方のPCのモニターの問題なので，必要ないならば消してしまってもかまわないが，あったところで問題はないので放置しておいてもらって構わない．<br> <br>
//! [参考] <br> FPSの制御を行う https://dixq.net/rp2/07.html ←このサイトのプログラムをだいぶパクっているが，機能を色々追加している．<br>
//!  DxlibのFPSとリフレッシュレートについて https://dixq.net/forum/viewtopic.php?t=20224
class Fps final
{
public:
    Fps() = default;
    ~Fps() = default;

    //! 処理が早すぎる場合，FPSを一定にするために待つ．
    void wait();

    //! 60Hz以上のモニター使用時に処理が詰まって画面がちらつかないように，描画処理をスキップするかどうかのフラグ．
    //! @return bool 処理が詰まって描画を飛ばしたいときにtrueを返す．
    bool skipDrawScene();

private:
    std::list<int> _list;

    bool m_skip_draw = false;   //コマ落ちを実装するためのフラグ

    //現在の時刻を記録する関数
    void regist(const int _now_time);

    //どれだけ待てばよいか返す関数．
    bool getWaitTime(int& _time) const;

    const int ONE_FRAME_MILLI_SECOND = (int)(1000.0 / GraphicConst::GRAPHIC_FPS);  //１フレーム当たりにかかる時間(ミリ秒)
    const int LIST_MAX = (int)(GraphicConst::GRAPHIC_FPS * 2);                     //リストに2秒分のデータをしまっておく
};

