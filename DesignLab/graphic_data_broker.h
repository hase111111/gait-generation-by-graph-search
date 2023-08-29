#pragma once

#include <boost/thread.hpp>

#include "map_state.h"
#include "Node.h"


//! @class GraphicDataBroker
//! @date 2023/08/08
//! @author 長谷川
//! @brief 画像表示部と，データ処理部を結びつける仲介人クラス
//! @details Broker:ブローカー，仲介人のこと．@n データ処理部(グラフ探索)が更新したデータをこのクラスに渡し，画像表示部がこのクラスから更新されたデータを持っていき，描画する．@n
//! @n 何をしているか分からない場合，「Pub / Subパターン」で調べてほしい．@n 
//! @n [非同期処理について] 
//! @n 非同期処理 (並列・同時に処理を行うこと) を行う際に，一つのにデータに同じタイミングでアクセスすると危険(未定義処理になり，成功か失敗かが不定になる)．
//! @n このクラスはそれを防ぐためにboost::shared_mutexを使用している．
//! @n 詳しくは https://www.mathkuro.com/c-cpp/boost/how-to-use-boost-thread/#toc10 の5章を参照してほしい．
//! @n このクラス内ではread lock, write lockを使っている． 
//! @n 参考 https://iorate.hatenablog.com/entry/20130222/1361538198 @n 
//! @n メンバのm_mtxについているmutable は constなメンバ関数(メンバの値を変更できないメンバ関数)においても変更できるようになるメンバ変数を表す．
//! @n 通常絶対使うべきではないが，今回のような場合(boost::shared_mutexを使う場合)は有効的．
class GraphicDataBroker final
{
public:

	GraphicDataBroker() = default;
	~GraphicDataBroker() = default;

	//! マップの状態を仲介人に渡す．
	//! @param [in] map マップを参照渡しする
	void setMapState(const MapState& map);

	//! マップの状態を返す
	//! @return MapState マップの状態を値渡しする．@n 基本的には大きなクラスは値渡しするべきでないが，マップは何度も更新されないデータだし値渡しする．
	MapState getMapState() const;


	//! @brief ロボットの新しい状態を示すノードを仲介人に渡す．ノードは動的配列で管理されており，新しいのを後ろに追加(push)する．
	//! @param [in] node ロボットの新しい状態
	void pushNode(const SNode& node);

	//! @brief ノードの集合を全て削除する．
	void deleteAllNode();

	//! @brief ノードの集合をvectorを用いて参照渡しする．全データをコピーする．
	//! @param [out] node_vec この関数の中で一度中身を空にしてから，データを代入する．
	void copyAllNode(std::vector<SNode>* node_vec) const;

	//! @brief ノードの集合をvectorを用いて参照渡しする．渡したvectorより，持っているデータが多い場合のみ差分をコピーする．@n copyAllNodeより軽い処理になる．
	//! @param [out] node_vec 差分以外に変更はされない．差分のみpushされる
	void copyOnlyNewNode(std::vector<SNode>* node_vec) const;

	//! @brief セットされているノードの数を返す．
	//! @return size_t ノードの数
	size_t getNodeNum() const;


	//! @brief シミュレーションが終了したことを仲介人に伝える．
	void setSimuEnd();

	//! @brief シミュレーションが終了するノードのインデックスを取得する
	//! @param [in] simu_num シミュレーション番号( 0 , 1, 2, ...)
	//! @return size_t シミュレーションが終了するノードのインデックス，シミュレーションが終了していない場合は最後のノードのインデックスを返す．
	//! @n シミュレーションが開始されていない場合は-1を返す．
	size_t getSimuEnd(const int simu_num);

	//! @brief シミュレーションの終了を示すノードのインデックスをvectorを用いて参照渡しする．
	void copySimuEndIndex(std::vector<size_t>* simu_end_index) const;

private:

	mutable boost::shared_mutex m_mtx;

	MapState m_map_state;

	std::vector<SNode> m_node;

	std::vector<size_t> m_simu_end;	//!< シミュレーションが終了するノードのインデックスを格納する．
};


//! @file graphic_data_broker.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief グラフ探索の結果を別スレッドのグラフィッククラスに連絡するGraphicDataBrokerクラス．
//! @n 行数 : @lineinfo
