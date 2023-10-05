#include "fps_controller.h"

#include <cmath>
#include <string>

#include <Dxlib.h>


FpsController::FpsController(const int target_fps)
	: kTargetFpsValue(target_fps), kOneFrameTime((int)(1000.0 / target_fps)), kListMax(target_fps * 2),
	need_skip_draw_screen_(false)
{
}


void FpsController::Wait()
{
	if (!TargetFpsIsVaild()) { return; }


	//待つべき時間を取得して待つ
	int wait_time = 0;

	if (CheckNeedSkipDrawScreen(&wait_time))
	{
		WaitTimer(wait_time);  //取得した時間分待つ
		//Sleep(wait_time);    //windows API版

		RegistTime(GetNowCount());  //現在の時刻を記録する
	}
	else
	{
		//時間オーバーしているので，コマ落ちの処理をする．

		//このフレームは理想的な処理をしたものとして，記録する
		RegistTime(time_list_.back() + kOneFrameTime);

		need_skip_draw_screen_ = true;     //描画を飛ばすフラグを立てる
	}
}


bool FpsController::SkipDrawScene()
{
	if (!TargetFpsIsVaild()) { return false; }


	//スキップフラグが立っているならば，そのフラグを折り，シーンをスキップする
	if (need_skip_draw_screen_)
	{
		need_skip_draw_screen_ = false;
		return true;
	}

	return false;
}


void FpsController::RegistTime(const int now_time)
{
	time_list_.push_back(now_time);   //現在の時刻を記憶

	if (time_list_.size() > kListMax)
	{
		//器から漏れたら削除する
		time_list_.pop_front();
	}
}


bool FpsController::CheckNeedSkipDrawScreen(int* time) const
{
	//時刻を初期化
	(*time) = 0;

	//時刻リストが空なら，Wait時間は0秒
	if (time_list_.empty())
	{
		(*time) = 0;
		return true;
	}

	int actually_took_time = GetNowCount() - time_list_.back();   //実際にかかった時間を求める

	//計算上かかるべき時間 - 実際にかかった時間　はすなわち待つべき時間
	int wait_time = kOneFrameTime - actually_took_time;

	if (wait_time >= 0)
	{
		//待ち時間が正の値であるとき(つまり，かかるべき時間より実際にかかった時間が小さい時)はそのまま値を返す．

		(*time) = wait_time;
		return true;
	}
	else
	{
		//待ち時間が負の値であるとき

		if ((int)abs(wait_time) < kOneFrameTime)
		{
			//１フレーム以上遅れていないならば，処理を行う．
			return false;
		}
	}

	//どれにも引っかからなかった場合０を返す
	(*time) = 0;
	return true;
}


bool FpsController::TargetFpsIsVaild() const
{
	//マイナスの値は許容しない
	if (kTargetFpsValue <= 0)
	{
		return false;
	}

	//１秒間に１フレーム以上は許容しない
	if (kTargetFpsValue > 60)
	{
		return false;
	}

	return false;
}
