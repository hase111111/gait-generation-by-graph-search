#pragma once

//このクラスを継承するとSingletonクラスになる．
//Singletonパターンは悪しき構築の一つなのでなるべく使わないようにしよう．
//このプロジェクトでは画像表示クラスでキーボードとマウスの入力を管理するために使用しています．
//グラフ探索では絶対に使わないこと!

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
