#pragma once
#include "Node.h"
#include "MyPolygon.h"
#include "ComType.h"
#include <vector>

//! @brief 重心位置の候補地点を示す多角形を作成するクラス
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する<br>
//! 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと<br>
//! 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，<br>
//! ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
private:

	//デバッグ用に出力を行う場合はtrueにする．テストコードを書きたいけど抽象化できていない...
	const bool DO_DEBUG_OUTPUT = false;

public:
	ComCandidatePolygonMaker() = default;

	//! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する
	//! @param[in] node 現在のロボットの状態を表すノード
	//! @param[out] _output_poly 重心位置の候補地点を示す多角形
	void makeCandidatePolygon(const SNode& node, std::vector<std::pair<my_vec::SPolygon2, ComType::EComPattern>>& _output_poly) const;

private:

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は4角形か5角形を用いて表現する．
	my_vec::SPolygon2 makeCandidateBox(const my_vec::SVector2 _leg_pos[HexapodConst::LEG_NUM], const int _start_leg_num) const;

	//! @brief 重心位置の候補地点を示す多角形を作成する．中心周りの図形は3角形を用いて表現する．
	//! @param[in] _leg_pos 脚の位置
	void makeCandidateTriangle(const my_vec::SVector2 _leg_pos[HexapodConst::LEG_NUM], my_vec::SPolygon2& _out_poly, ComType::EComPattern& _out_com_pattern) const;

	//! @brief 正しい多角形が生成されているかを確認する
	//! @param[in] _poly 確認する多角形
	//! @return 正しい多角形が生成されているか
	bool checkPolygon(const my_vec::SPolygon2& _poly) const;
};