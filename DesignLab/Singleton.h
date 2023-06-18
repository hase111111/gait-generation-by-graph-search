//! @file Singleton.h
//! @brief Singletonクラスの実装．
//! @author 長谷川

#pragma once

//! @class Singleton
//! @brief このクラスを継承するとSingletonクラスになる．グラフ探索では絶対に使わないこと!
//! @details Singletonクラスとは，C言語でいうところのグローバル変数（どこからでも値を変更できる変数）である．<br>
//! Singletonパターンは悪しき構築の一つなのでなるべく使わないほうがよい．<br>
//! このプロジェクトでは画像表示クラスでキーボードとマウスの入力を管理するために使用している．<br> <br>
//! グラフ探索では""絶対""に使わないこと!! <br> 絶対だよ!!!!
//! @author 長谷川
template <typename _T>
class Singleton 
{
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton& r) = default;
    Singleton& operator=(const Singleton& r) = default;

public:

	static _T* getIns() 
	{
		static _T inst;
		return &inst;
	};

};
