
//! @file      singleton.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_SINGLETON_H_
#define DESIGNLAB_SINGLETON_H_


namespace designlab
{

//! @class Singleton
//! @brief Singletonクラス作成のためのテンプレートクラス．
//! @details
//! このクラスを継承すると Singleton クラスになる．
//! Singleton クラスとは，C言語でいうところのグローバル変数
//! （どこからでも変更できる変数）である．
//! 多用するとプログラムが複雑になりがちなので，グラフ探索では絶対に使わないこと．
//!
//! 参考
//! @li 龍神録2プログラミングの館 https://dixq.net/rp2/ ( アクセス日 2024/3/2 )
//! @see FontLoader
template <typename _T>
class Singleton
{
public:
    //! @brief インスタンスを取得する．
    //! @n このクラスを継承したクラスは
    //! クラス名::getIns()-> の形式でメンバ関数を呼び出す．
    //! @return _T* インスタンスのポインタ．
    static _T* GetIns()
    {
        static _T inst;
        return &inst;
    };

protected:
    //! コンストラクタ，デストラクタ，コピーコンストラクタ，
    //! 代入演算子は外部から呼び出せないようにする．
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton& r) = default;
    Singleton& operator=(const Singleton& r) = default;
};

}  // namespace designlab


#endif  // DESIGNLAB_SINGLETON_H_
