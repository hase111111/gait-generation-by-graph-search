#include "fps.h"

#include <cmath>
#include <string>

#include "DxLib.h"

#include "Define.h"


void Fps::wait()
{
	//待つべき時間を取得して待つ
	int wait_time = 0;

	if (getWaitTime(&wait_time) == true)
	{
		WaitTimer(wait_time);  //取得した時間分待つ
		//Sleep(wait_time);    //windows API版

		regist(GetNowCount());  //現在の時刻を記録する
	}
	else
	{
		//時間オーバーしているので，コマ落ちの処理をする．

		//このフレームは理想的な処理をしたものとして，記録する
		regist(m_list.back() + ONE_FRAME_MILLI_SECOND);

		m_do_skip_draw = true;     //描画を飛ばすフラグを立てる
	}
}


bool Fps::skipDrawScene()
{
	//スキップフラグが立っているならば，そのフラグを折り，シーンをスキップする
	if (m_do_skip_draw == true)
	{
		m_do_skip_draw = false;
		return true;
	}

	return false;
}


void Fps::regist(const int now_time)
{
	m_list.push_back(now_time);   //現在の時刻を記憶

	if (m_list.size() > LIST_MAX)
	{
		//器から漏れたら削除する
		m_list.pop_front();
	}
}


bool Fps::getWaitTime(int* time) const
{
	//時刻を初期化
	(*time) = 0;

	//時刻リストが空なら，Wait時間は0秒
	if (m_list.empty() == true)
	{
		(*time) = 0;
		return true;
	}

	int actually_took_time = GetNowCount() - m_list.back();   //実際にかかった時間を求める

	//計算上かかるべき時間 - 実際にかかった時間　はすなわち待つべき時間
	int wait_time = ONE_FRAME_MILLI_SECOND - actually_took_time;

	//待ち時間が正の値であるとき(つまり，かかるべき時間より実際にかかった時間が小さい時)はそのまま値を返す．
	if (wait_time >= 0)
	{
		(*time) = wait_time;
		return true;
	}

	//待ち時間が負の値であるとき
	else
	{
		if ((int)abs(wait_time) < ONE_FRAME_MILLI_SECOND)
		{
			//１フレーム以上遅れていないならば，処理を行う．
			return false;
		}
	}

	//どれにも引っかからなかった場合０を返す
	(*time) = 0;
	return true;
}
