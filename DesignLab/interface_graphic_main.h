#pragma once

#include "graphic_data_broker.h"


//! @class IGraphicMain
//! @date 2023/08/09
//! @author 長谷川
//! @brief GraphicMainのインターフェース
//! @details 描画の処理を行うクラスは必ずこのクラスを継承すること．逆に言えば継承すれば自由に処理が交換可能になる．@n 
//! @n [インターフェイスについて] 
//! @n このクラスはインターフェース(純粋仮想関数のみをもつ実体を生成できないクラス)といえる．
//! @n 所謂「依存性の注入」というやつ．GraphicLoopはGraphicMainの処理に依存してしまうため，これをインターフェイスにすることでGraphicLoopはGraphicMainの
//! @n インターフェイス(update関数の戻り値)にのみ関心を持つようになり，依存性を解消できるようになる．
//! @n まぁ「依存性の注入」については正直自分もよくわかっていないので，理解しなくても大丈夫．
//! @n 参考: https://qiita.com/okazuki/items/a0f2fb0a63ca88340ff6 
//! @n https://qiita.com/okazuki/items/0c17a161a921847cd080 
//! @n ともかく，交換可能なグラフィックのパーツを作るための型がこの親クラスだということ．@n
//! @n 継承先のクラスではupdateとdrawをoverrideする必要がある．
//! @n よくわからないなら，「クラスの継承」「関数のオーバーライド（オーバーロードとは別物）」「完全仮想関数」あたりを調べてみるとよいと思う．ぶっちゃけ，よくわからなくても大丈夫．
//! @n ひとまず，継承先のGraphicMainSampleを確認すればなんとなくわかると思う．
class IGraphicMain
{
public:

	//! @brief このクラスの継承先では，GraphicDataBrokerクラスのポインタを引数に持つコンストラクタを実装する必要がある．
	IGraphicMain(const GraphicDataBroker* broker);
	virtual ~IGraphicMain() = default;

	//! @brief 描画画面の更新を行う．純粋仮想関数のため，継承先では必ずoverrideする必要がある．
	//! @return bool ループを抜け，グラフィックの表示を終了するならばfalseをかえす．
	virtual bool update() = 0;

	//! @brief 描画を行う．ここでは描画系の処理のみを行い内部のデータを更新しないためconstを付けている．純粋仮想関数．
	virtual void draw() const = 0;

protected:

	const GraphicDataBroker* const mp_broker;	//!< 画像表示を行うこのクラスと，データ処理を行う外部のクラスを繋ぐ仲介人クラスのポインタを受け取る．

};


//! @file InterfaceGraphicMain.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief IGraphicMainクラス．IGraphicMainはインターフェイスであるので実態が作れない．
