#pragma once
#include <boost/thread.hpp>
#include "MapState.h"
#include "Node.h"


class GraphicDataBroker final
{
public:

	GraphicDataBroker() = default;
	~GraphicDataBroker() = default;

	//! マップの状態を仲介人に渡す．
	//! @param [in] _map マップを参照渡しする
	void setMapState(const MapState& _map);	

	//! マップの状態を返す
	//! @return MapState マップの状態を値渡しする．<br>基本的には大きなクラスは値渡しするべきでないが，マップは何度も更新されないデータだし値渡しする．
	MapState getMapState() const;


	//! ロボットの新しい状態を示すノードを仲介人に渡す．ノードは動的配列で管理されており，新しいのを後ろに追加(push)する．
	//! @param [in] _node ロボットの新しい状態
	void pushNode(const SNode &_node);				

	//! ノードの集合をvectorを用いて参照渡しする．全データをコピーする．
	//! @param [out] _node_vec この関数の中で一度中身を空にしてから，データを代入する．
	void copyAllNode(std::vector<SNode>& _node_vec) const;

	//! ノードの集合をvectorを用いて参照渡しする．渡したvectorより，持っているデータが多い場合のみ差分をコピーする．<br> copyAllNodeより軽い処理になります．
	//! @param [out] _node_vec 差分以外に変更はされない．差分のみpushされる
	void copyOnlyNewNode(std::vector<SNode>& _node_vec) const;

private:
	
	mutable boost::shared_mutex m_mtx;

	MapState m_Map;

	std::vector<SNode> m_node;
};


//! @file GraphicDataBroker.h
//! @brief GraphicDataBrokerクラスの実装を行っている．
//! @author 長谷川

//! @class GraphicDataBroker
//! @brief 画像表示部と，データ処理部を結びつける仲介人クラス
//! @details Broker:ブローカー，仲介人のこと．<br> データ処理部(グラフ探索)が更新したデータをこのクラスに渡し，画像表示部がこのクラスから更新されたデータを持っていき，描画します．<br>
//! 何をしているか分からない場合，「Pub / Subパターン」で調べてみてください．<br> 
//! <br> 
//! [非同期処理について] <br>
//! 非同期処理(同時に処理を行うこと)を行う際に，一つのにデータに同じタイミングでアクセスすると危険です(未定義処理になり，成功か失敗かが不定になる)．<br>
//! このクラスはそれを防ぐためにboost::shared_mutexを使用しています．<br>
//! 詳しくは https://www.mathkuro.com/c-cpp/boost/how-to-use-boost-thread/#toc10 の5章をみてください．<br>
//! このクラス内ではread lock, write lockを使っています．<br> 
//! 参考 https://iorate.hatenablog.com/entry/20130222/1361538198 <br> 
//! <br>
//! メンバのm_mtxについているmutable は constなメンバ関数(メンバの値を変更できないメンバ関数)においても変更できるようになるメンバ変数を表します．<br>
//! 通常絶対使うべきではないですが，今回のような場合(boost::shared_mutexを使う場合)は有効的です．
//! @author 長谷川

