//! @file asyncable_data.h
//! @brief 非同期処理を行う際に，データの更新回数とデータをまとめて扱うためのクラス．


#ifndef DESIGNLAB_ASYNCABLE_DATA_H_
#define DESIGNLAB_ASYNCABLE_DATA_H_

#include <vector>

#include <boost/thread.hpp>


// テンプレートの実装はヘッダーに書く． https://qiita.com/i153/items/38f9688a9c80b2cb7da7


//! @class AsyncableData
//! @brief 非同期処理を行う際に，データの更新回数とデータをまとめて扱うためのクラス．(コピー・ムーブは禁止)
//! @details この構造体は，データの更新回数とデータをまとめて扱うためのクラス．
//! @n 値の変更を行う際に，データの更新回数をインクリメント(++のこと)することで，データの更新回数をカウントする．
//! @n また，値の参照と変更を行う際にミューテックスを用いて，同時に変更されることを防ぐ．ミューテックスについては以下を参照．
//! @n
//! @n [非同期処理について] 
//! @n 非同期処理 (並列・同時に処理を行うこと) を行う際に，一つのにデータに同じタイミングで操作すると危険(未定義処理になり，成功か失敗かが不定になる)．
//! @n このクラスはそれを防ぐためにboost::shared_mutexを使用している．
//! @n 詳しくは https://www.mathkuro.com/c-cpp/boost/how-to-use-boost-thread/#toc10 の5章を参照してほしい．
//! @n このクラス内ではread lock, write lockを使っている． 
//! @n 参考 https://iorate.hatenablog.com/entry/20130222/1361538198 
//! @n 
//! @n メンバのm_mtxについているmutable は constなメンバ関数(メンバの値を変更できないメンバ関数)においても変更できるようになるメンバ変数を表す．
//! @n 通常絶対使うべきではないが，今回のような場合(boost::shared_mutexを使う場合)は有効的．
//! @tparam T 非同期処理を行うデータ．代入を行うことができる型を指定すること．
//! @n C++20ではconceptを使うことで，代入を行うことができる型を指定できる．アップデートしたらそれを使用する用に変更する．
template <typename T>
class AsyncableData
{
	static_assert(std::is_copy_assignable<T>::value, "代入を行うことができる型を指定してください．");

public:

	AsyncableData() : update_count_(0) {};
	explicit AsyncableData(const T& data) : data_(data), update_count_(0) {};

	//!< コピー・ムーブは禁止
	AsyncableData(const AsyncableData&) = delete;
	AsyncableData& operator=(const AsyncableData&) = delete;
	AsyncableData(AsyncableData&&) = delete;

	//! @brief 値をコピーして返す．
	//! @n この時，read lockをかける．
	//! @n 当然，データの更新回数はインクリメントされない．
	//! @return T 値のコピー
	T GetData() const
	{
		//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
		boost::shared_lock<boost::shared_mutex> read_lock(mtx_);
		return data_;
	};

	//! @brief 値を変更する．
	//! @n この時，write lockをかける．
	//! @n データの更新回数をインクリメントする．
	//! @param [in] data セットする値．const参照渡しされる．
	void SetData(const T& data)
	{
		//書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
		boost::upgrade_lock<boost::shared_mutex> upgrade_lock(mtx_);

		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

			data_ = data;
			++update_count_;
		}
	};

	//! @brief データの更新回数を返す．
	//! @n この時，read lockをかける．
	//! @n この値を調べて，データの更新回数が変わっているかを確認することで，データの更新が必要かを確認する．
	//! @return int データの更新回数
	int GetUpdateCount() const
	{
		//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
		boost::shared_lock<boost::shared_mutex> read_lock(mtx_);
		return update_count_;
	};

private:

	mutable boost::shared_mutex mtx_;	//!< ロック用のmutex

	T data_;

	int update_count_;
};



//! @brief 非同期処理を行う際に，データの更新回数とデータをまとめて扱うための構造体 (vector版)
//! @n コピー・ムーブは禁止
//! @details vector版のAsyncableData．vectorを入れてAsyncableDataを作成すると，こちらが呼ばれる．
template <typename T>
class AsyncableData <std::vector<T> >
{
public:

	AsyncableData() : data_({}), update_count_(0) {};
	explicit AsyncableData(const std::vector<T>& data) : data_(data), update_count_(0) {}

	//!< コピー・ムーブは禁止
	AsyncableData(const AsyncableData&) = delete;
	AsyncableData& operator=(const AsyncableData&) = delete;
	AsyncableData(AsyncableData&&) = delete;

	//! @brief 値をコピーして返す．
	//! @n この時，read lockをかける．
	//! @n 当然，データの更新回数はインクリメントされない．
	//!	@return T 値のコピー．
	std::vector<T> GetData() const
	{
		//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
		boost::shared_lock<boost::shared_mutex> read_lock(mtx_);
		return data_;
	};

	//! @brief 値を変更する．
	//! @n この時，write lockをかける．
	//! @n データの更新回数をインクリメントする．
	//! @param [in] data セットする値．const参照渡しされる．
	void SetData(const std::vector<T>& data)
	{
		//書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
		boost::upgrade_lock<boost::shared_mutex> upgrade_lock(mtx_);

		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

			data_ = data;
			++update_count_;
		}
	};

	//! @brief 最後尾に値を追加する．
	//! @n この時，write lockをかける．
	//! @n データの更新回数をインクリメントする．
	//! @param [in] data 後ろに追加する値．const参照渡しされる．
	void PushBack(const T& data)
	{
		//書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
		boost::upgrade_lock<boost::shared_mutex> upgrade_lock(mtx_);

		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

			data_.push_back(data);
			++update_count_;
		}
	};

	//! @brief 値をすべて削除する.
	//! @n この時，write lockをかける．
	//! @n データの更新回数をインクリメントする．
	void Clean()
	{
		//書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
		boost::upgrade_lock<boost::shared_mutex> upgrade_lock(mtx_);

		{
			boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

			data_.clear();
			++update_count_;
		}
	};

	//! @brief sizeを返す．要素の数をsize_tで返す．
	//! @n この時，read lockをかける．
	//! @return size_t 要素の数．
	size_t GetSize() const
	{
		//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
		boost::shared_lock<boost::shared_mutex> read_lock(mtx_);
		return data_.size();
	};

	//! @brief データの更新回数を返す．
	//! @n この時，read lockをかける．
	//! @n この値を調べて，データの更新回数が変わっているかを確認することで，データの更新が必要かを確認する．
	//! @return int データの更新回数．
	int GetUpdateCount() const
	{
		//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
		boost::shared_lock<boost::shared_mutex> read_lock(mtx_);
		return update_count_;
	};

private:

	mutable boost::shared_mutex mtx_;	//!< ロック用のmutex．

	std::vector<T> data_;

	int update_count_;
};


#endif	// DESIGNLAB_ASYNCABLE_DATA_H_