#pragma once

//このクラスを継承するとSingletonクラスになる．
//Singletonパターンは悪しき構築の一つなのでなるべく使わないようにしよう．

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
