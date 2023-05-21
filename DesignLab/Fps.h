#pragma once
#include <list>
#include "GraphicConst.h"

// FPSを一定にキープするためのクラス．
// 基本的には何もしなくとも一定になるのだけど，144Fpsとか240Fpsくらいのゲーミングモニターを使っている場合，画面がおかしくなることがあるのでこのクラスを使って制御します．
// 当方のPCのモニターの問題なので，必要ないっていうならば消してしまって大丈夫です．
// 参考 
//  FPSの制御を行う https://dixq.net/rp2/07.html
//  DxlibのFPSとリフレッシュレートについて https://dixq.net/forum/viewtopic.php?t=20224

class Fps final
{
public:
    Fps() = default;
    ~Fps() = default;

    //処理が早すぎる場合，FPSを一定にするために待つ．
    void wait();

    //処理が詰まって描画を飛ばしたいときにtrueを返す．
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

