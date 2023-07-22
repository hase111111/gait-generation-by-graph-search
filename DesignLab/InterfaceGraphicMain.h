#pragma once
#include "GraphicDataBroker.h"


class IGraphicMain
{
public:

	//! @brief このクラスの継承先では，const GraphicDataBroker* _brokerを引数に持つコンストラクタを実装する必要がある．
	IGraphicMain(const GraphicDataBroker* _broker);
	virtual ~IGraphicMain() = default;

	//! @brief 描画画面の更新を行う．純粋仮想関数のため，継承先では必ずoverrideする必要がある．
	//! @param bool ループを抜け，グラフィックの表示を終了するならばfalseをかえす．
	virtual bool update() = 0;

	//! @brief 描画を行う．ここでは描画系の処理のみを行い内部のデータを更新しないためconstを付けている．純粋仮想関数．
	virtual void draw() const = 0;

protected:

	const GraphicDataBroker* mp_Broker;	//!< 画像表示を行うこのクラスと，データ処理を行う外部のクラスを繋ぐ仲介人クラスのポインタを受け取る．

};


//! @file InterfaceGraphicMain.h
//! @brief IGraphicMainクラスの宣言．IGraphicMainはインターフェイスであるので実態が作れない．
//! @author 長谷川

//! @class IGraphicMain
//! @brief GraphicMainのインターフェース
//! @details 描画の処理を行うクラスは必ずこのクラスを継承すること．逆に言えば継承すれば自由に処理が交換可能になる．<br> 
//! <br> 
//! [インターフェイスについて] <br>
//! このクラスはインターフェース(純粋仮想関数のみをもつ実体を生成できないクラス)といえる．<br>
//! 所謂「依存性の注入」というやつ．GraphicLoopはGraphicMainの処理に依存してしまうため，これをインターフェイスにすることでGraphicLoopはGraphicMainの
//! インターフェイス(update関数の戻り値)にのみ関心を持つようになり，依存性を解消できるようになる．<br>
//! まぁ「依存性の注入」については正直自分もよくわかっていないので，理解しなくても大丈夫．<br>
//! 参考: https://qiita.com/okazuki/items/a0f2fb0a63ca88340ff6 <br>
//! https://qiita.com/okazuki/items/0c17a161a921847cd080 <br>
//! ともかく，交換可能なグラフィックのパーツを作るための型がこの親クラスだということ．<br> 
//! <br>
//! 継承先のクラスではupdateとdrawをoverrideする必要がある．<br>
//! よくわからないなら，「クラスの継承」「関数のオーバーライド（オーバーロードとは別物）」「完全仮想関数」あたりを調べてみるとよいと思う．ぶっちゃけ，よくわからなくても大丈夫．<br>
//! ひとまず，GraphicMainSampleを確認すればなんとなくわかると思う．<br>
//! @author 長谷川
