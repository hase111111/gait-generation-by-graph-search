
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "fps_controller.h"

#include <cmath>
#include <string>

#include <Dxlib.h>


namespace designlab
{

FpsController::FpsController(const int target_fps) :
    kTargetFpsValue(target_fps),
    kOneFrameTime(static_cast<int>(1000.0 / target_fps)),
    kListMax(target_fps * 2),
    need_skip_draw_screen_(false)
{
}


void FpsController::Wait()
{
    if (!TargetFpsIsValid())
    {
        return;
    }

    // 待つべき時間を取得して待つ．
    int wait_time = 0;

    if (CheckNeedSkipDrawScreen(&wait_time))
    {
        WaitTimer(wait_time);  // 取得した時間分待つ．
        // Sleep(wait_time);    // windows API版．

        RegisterTime(GetNowCount());  // 現在の時刻を記録する．
    }
    else
    {
        // 時間オーバーしているので，コマ落ちの処理をする．

        // このフレームは理想的な処理をしたものとして，記録する．
        RegisterTime(time_list_.back() + kOneFrameTime);

        need_skip_draw_screen_ = true;  // 描画を飛ばすフラグを立てる．
    }
}


bool FpsController::SkipDrawScene()
{
    if (!TargetFpsIsValid())
    {
        return false;
    }

    // スキップフラグが立っているならば，そのフラグを折り，シーンをスキップする．
    if (need_skip_draw_screen_)
    {
        need_skip_draw_screen_ = false;
        return true;
    }

    return false;
}

void FpsController::DrawFps() const
{
    if (!TargetFpsIsValid())
    {
        printfDx("FPS:%dは推奨されません．(FPSの調整は行われません．)\n",
                 kTargetFpsValue);
        return;
    }

    if (time_list_.size() < 2)
    {
        printfDx("計測中です．\n");
        return;
    }

    // FPSを計算する．
    const int duration = (time_list_.back() - time_list_.front()) /
        static_cast<int>(time_list_.size() - 1);

    printfDx("FPS:%d\n", static_cast<int>(1000.0 / duration));
}


void FpsController::RegisterTime(const int now_time)
{
    time_list_.push_back(now_time);  // 現在の時刻を記憶．

    if (time_list_.size() > kListMax)
    {
        // 器から漏れたら削除する．
        time_list_.pop_front();
    }
}


bool FpsController::CheckNeedSkipDrawScreen(int* time) const
{
    // 時刻を初期化．
    (*time) = 0;

    // 時刻リストが空なら，Wait時間は0秒．
    if (time_list_.empty())
    {
        (*time) = 0;
        return true;
    }

    // 実際にかかった時間を求める．
    int actually_took_time = GetNowCount() - time_list_.back();

    // 計算上かかるべき時間 - 実際にかかった時間　はすなわち待つべき時間．
    int wait_time = kOneFrameTime - actually_took_time;

    if (wait_time >= 0)
    {
        // 待ち時間が正の値であるとき，
        // (つまり，かかるべき時間より実際にかかった時間が小さい時)はそのまま値を返す．

        (*time) = wait_time;
        return true;
    }
    else
    {
        // 待ち時間が負の値であるとき．

        if (static_cast<int>(abs(wait_time)) < kOneFrameTime)
        {
            // １フレーム以上遅れていないならば，処理を行う．
            return false;
        }
    }

    // どれにも引っかからなかった場合０を返す．
    (*time) = 0;
    return true;
}


bool FpsController::TargetFpsIsValid() const
{
    // マイナスの値は許容しない．
    if (kTargetFpsValue <= 0)
    {
        return false;
    }

    // １秒間に１フレーム以上は許容しない．
    if (kTargetFpsValue > 60)
    {
        return false;
    }

    return true;
}

}  // namespace designlab
