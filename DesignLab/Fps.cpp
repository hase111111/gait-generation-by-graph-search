#include "DxLib.h"
#include "Fps.h"
#include <cmath>
#include <string>
#include "Define.h"

void Fps::wait()
{
    //待つべき時間を取得して待つ
    int _wait_time = 0;

    if (getWaitTime(_wait_time) == true)
    {
        WaitTimer(_wait_time);  //取得した時間分待つ
        //Sleep(_wait_time);    //windows API版

        regist(GetNowCount());  //現在の時刻を記録する
    }
    else
    {
        //時間オーバーしているので，コマ落ちの処理をする．

        //このフレームは理想的な処理をしたものとして，記録する
        regist(_list.back() + ONE_FRAME_MILLI_SECOND);

        m_skip_draw = true;     //描画を飛ばすフラグを立てる
    }
}

bool Fps::skipDrawScene()
{
    //スキップフラグが立っているならば，そのフラグを折り，シーンをスキップする
    if (m_skip_draw == true)
    {
        m_skip_draw = false;
        return true;
    }

    return false;
}

void Fps::regist(const int _now_time)
{
    _list.push_back(_now_time);   //現在の時刻を記憶

    if (_list.size() > LIST_MAX) 
    {  
        //器から漏れたらポップ
        _list.pop_front();
    }
}

bool Fps::getWaitTime(int& _time) const
{
    //時刻を初期化
    _time = 0;

    //時刻リストが空なら，Wait時間は0秒
    if (_list.empty() == true)
    {
        _time = 0;
        return true;
    }

    int actuallyTookTime = GetNowCount() - _list.back();   //実際にかかった時間を求める

    //計算上かかるべき時間 - 実際にかかった時間　はすなわち待つべき時間
    int waitTime = ONE_FRAME_MILLI_SECOND - actuallyTookTime;

    //待ち時間が正の値であるとき(つまり，かかるべき時間より実際にかかった時間が小さい時)はそのまま値を返す．
    if (waitTime >= 0)
    {
        _time = waitTime;
        return true;
    }

    //待ち時間が負の値であるとき
    else
    {
        if ((int)abs(waitTime) < ONE_FRAME_MILLI_SECOND)
        {
            //１フレーム以上遅れていないならば，処理を行う．
            return false;
        }
    }

    //どれにも引っかからなかった場合０を返す
    _time = 0;
    return true;
}
