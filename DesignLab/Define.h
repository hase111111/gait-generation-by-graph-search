#pragma once

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

//mapData.h
	enum { MAPDATA3D_MAX = 15000 };		//enumハック．邪悪な解決法

//PassFinding.h

	//グラフ探索において枝刈りを行うときはtrue
	const static bool FLAG_DO_PRUNING;

//Hexapod.h
	const static int LEG_NUM;

};
