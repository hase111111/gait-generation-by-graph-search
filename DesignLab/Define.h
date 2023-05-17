#pragma once

// Effective C++ という本 (私が持っているのはかなり古い版のものなので正直正しいのかはよくわからないけど) によると，
// C++においてはあまり #defineを使用するべきではないようです．
// 結構いろんなサイトでも同じことが言われています
// https://qiita.com/jonichonpa/items/595ed7914751787ebaee
// https://myon.info/blog/2015/12/18/avoid-defining-macros/
// const statisな定数．inline 関数．constexpr定数を使うべきです．
// 以下参考資料．
//
//	クラスメンバに constexpr static 変数はおすすめしない ― 現象と対策
//	https://qiita.com/Nabetani/items/d8a3ebccaef03cd18d81

class Define 
{
	Define() = delete;
	~Define() = delete;

public:
//main.cpp

	//連続でシミュレーションを行う回数
	const static int SIMURATE_NUM;

	//1シミュレーション当たりの最大歩容生成回数
	const static int GATE_PATTERN_GENERATE_NUM;

	//直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了	//合計1000動作前後を越すとエラーが出る？←でる、原因の詳細は分からないが、fstreamが関係してる？20201117hato
	const static int GOAL_TAPE;

	//グラフィックを表示するときはtrue
	const static bool FLAG_GRAPHIC_AVAILABLE;

	//これ以上小さい値は0とみなす．allowable error，許容誤差のこと
	inline constexpr static double ALLOWABLE_ERROR = 0.0001;

	// 円周率
	const static double MY_PI;

//PassFinding.h

	//グラフ探索において枝刈りを行うときはtrue
	const static bool FLAG_DO_PRUNING;
};
