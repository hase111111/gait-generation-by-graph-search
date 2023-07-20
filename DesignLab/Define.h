#pragma once


class Define final
{
public:

	const static int SIMURATE_NUM;	//!< 連続でシミュレーションを行う回数

	const static char GRAPH_SEARCH_DEPTH;	//!< グラフ探索の探索深さ

	const static int GATE_PATTERN_GENERATE_NUM;	//!< 1シミュレーション当たりの最大歩容生成回数

	const static int GOAL_TAPE;	//!< 直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了

	const static bool FLAG_GRAPHIC_AVAILABLE;	//!< グラフィックを表示するときはtrue

	const static bool FLAG_DO_PRUNING;	//!< グラフ探索において枝刈りを行うときはtrue

private:

	Define() = delete;
	Define(const Define& _other) = delete;
};


//! @file Define.h
//! @brief プロジェクト全体の定数
//! @author 長谷川
//! @date 2023/06/17

//! @class Define
//! @brief
//! @details Effective C++ という本 (私が持っているのはかなり古い版のものなので正直正しいのかはよくわからないけど) によると，<br>
//! C++においてはあまり #defineを使用するべきではないようである．結構いろんなサイトでも同じことが言われている．<br> 
//! https://qiita.com/jonichonpa/items/595ed7914751787ebaee <br>
//! https://myon.info/blog/2015/12/18/avoid-defining-macros/ <br>
//! <br>
//! const statisな定数．inline 関数．constexpr定数を使うべき．<br>
//! <br>
//! 以下参考資料．<br> 
//! クラスメンバに constexpr static 変数はおすすめしない ― 現象と対策<br>
//! https://qiita.com/Nabetani/items/d8a3ebccaef03cd18d81
//! @attention GRAPH_SEARCH_DEPTHの値を大きくしすぎると動作しなくなる．<br>
//! 先行研究では深さ4～5あたりで動作せていたのでそのあたりで，<br>
//! ちなみに，深さを1つ深くしただけで，探索する範囲は膨大に増えるので注意が必要．深さ6以上を実時間内に探索し終わるのは，そこそこの性能のPCが必要．
//! @author 長谷川
//! @date 2023/06/17