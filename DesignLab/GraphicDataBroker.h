#pragma once
#include "boost/thread.hpp"
#include "MapState.h"

//画像表示部と，データ処理部を結びつける仲介人(Broker:ブローカー)がこのクラスです．
//データ処理部が更新されたデータをこのクラスに渡し，
//画像表示部がこのクラスから更新されたデータを持っていきます．
//何をしているか分からない場合，「Pub / Subパターン」で調べてみてください．

class GraphicDataBroker final
{
public:
	GraphicDataBroker() = default;
	~GraphicDataBroker() = default;

	void setMapState(const MapState& _map);	//マップの状態をセットする
	MapState getMapState() const;			//マップの状態を返す

	void pushNode(const SNode _node);							//新しい状態を示すノードを渡す．
	void copyNode(std::vector<SNode>& _node_vec) const;			//ノードの集合をvectorを用いて参照渡しする．全データをコピーする．
	void copyOnlyNewNode(std::vector<SNode>& _node_vec) const;	//ノードの集合をvectorを用いて参照渡しする．渡したvectorより，持っているデータが多い場合のみ差分をコピーする．

private:
	
	//非同期処理(同時に処理を行うこと)を行う際に，一つにデータに同じタイミングでアクセスすると危険です(未定義処理になり，成功か失敗かが不定になる)．これを使うとそれを防げます．
	//詳しくは https://www.mathkuro.com/c-cpp/boost/how-to-use-boost-thread/#toc10 の5章をみてください．
	// mutable は constな関数(メンバの値を変更できない関数)においても変更できるようになるメンバ変数を表します．通常絶対使うべきではないですが，今回のような場合は有効的です．
	mutable boost::shared_mutex m_mtx;

	MapState m_Map;

	std::vector<SNode> m_node;
};