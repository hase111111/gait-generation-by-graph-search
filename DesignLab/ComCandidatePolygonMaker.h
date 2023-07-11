#pragma once
#include "Node.h"
#include "MyPolygon.h"
#include <vector>

//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する<br>
//! 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと<br>
//! 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，<br>
//! ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
public:
	ComCandidatePolygonMaker() = default;

	//! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
	//! @param[in] node 現在のロボットの状態を表すノード
	//! @param[out] _output_poly 重心位置の候補地点を示す多角形
	void makeCandidatePolygon(const SNode& node, std::vector<my_vec::SPolygon2>& _output_poly);

private:

};