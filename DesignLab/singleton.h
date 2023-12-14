//! @file singleton.h
//! @brief Singletonクラス作成のためのテンプレートクラス．

#ifndef DESIGNLAB_SINGLETON_H_
#define DESIGNLAB_SINGLETON_H_


//! @class Singleton
//! @brief Singletonクラス作成のためのテンプレートクラス．
//! @details このクラスを継承するとSingletonクラスになる．Singletonクラスとは，C言語でいうところのグローバル変数（どこからでも値を変更できる変数）である．
//! 多用するとプログラムが複雑になりがちなので，グラフ探索では絶対に使わないこと．
//! このプロジェクトでは画像表示クラスでキーボードとマウスの入力を管理するために使用している．
//! 参考 https://dixq.net/rp2/ 
//! @see FontLoader
template <typename _T>
class Singleton
{
public:

	//! @brief インスタンスを取得する．
	//! @n このクラスを継承したクラスは クラス名::getIns()-> の形式でメンバ関数を呼び出す．
	//! @return _T* インスタンスのポインタ
	static _T* GetIns()
	{
		static _T inst;
		return &inst;
	};

protected:

	//コンストラクタ，デストラクタ，コピーコンストラクタ，代入演算子は外部から呼び出せないようにする．
	Singleton() = default;
	virtual ~Singleton() = default;
	Singleton(const Singleton& r) = default;
	Singleton& operator=(const Singleton& r) = default;

};


#endif // DESIGNLAB_SINGLETON_H_