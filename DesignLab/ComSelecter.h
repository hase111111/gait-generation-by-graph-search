#pragma once
#include "MyVector.h"
#include "MyPolygon.h"
#include "ComType.h"
#include "Node.h"
#include "HexapodStateCalculator.h"

//! @brief 重心を求めるクラス．波東さんのプログラムにおけるCCCの処理を行っている．
class ComSelecter final
{
private:

	const int DISCRETIZATION_NUM = 10; // 重心を求める際の分割数

	const float STABILITY_MARGIN = 10.0f; // 絶対安全余裕

	const bool DO_DEBUG_PRINT = false; // デバッグ用の出力を行うかどうか．テストコードを書きたいが抽象化できていない...

	SNode m_current_node; //!< 現在のノード

	HexapodStateCalclator m_calclator; //!< 状態計算クラス

public:

	//! @brief 現在のノードを設定する
	//! @param [in] _current_node 現在のノード
	void setCurrentNode(const SNode& _current_node) { m_current_node = _current_node; } //!< 現在のノードを設定する

	//! @brief 重心を求める
	//! @param [in] polygon 重心を求める対象のポリゴン．この中に入る点を出力する．
	//! @param [in] _com_pattren 重心の求め方
	//! @param [out] _output_com 重心
	//! @return 重心を求めることができたかどうか
	//! @details 重心位置の決め方は波東さんのプログラムに準拠している． <br>
	//! まずは，候補地点の多角形を囲む四角形を生成，その中に等間隔で候補点を打つ． <br>
	//! 次に多角形の中に入っていない点を除外する． <br>
	//! そして，絶対安全余裕を計算し，マージンを外れた点を除外する． <br>
	//! また，移動後の座標において，脚が他の脚と干渉する場合は除外する． <br>
	//! 以上の処理を行った後，残った点の重心を求める． <br>
	bool getComFromPolygon(const my_vec::SPolygon2& polygon, const ComType::EComPattern _com_pattren, my_vec::SVector& _output_com) const;

private:

	//! @brief 候補地点を生成する
	void makeComCandidatePoint(const my_vec::SPolygon2& polygon, std::vector<my_vec::SVector2>& _output_coms) const;

	//! @brief 絶対安全余裕を計算し，マージンを外れていないか調べる
	bool isInMargin(const my_vec::SPolygon2& polygon, const my_vec::SVector2& _com) const;
};
