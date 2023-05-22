#pragma once
#include "GraphicDataBroker.h"

//描画の処理を行うクラスは必ずこのクラスを継承すること．このクラスはインターフェース(完全仮想関数のみをもつ実体を生成できないクラス)といえます．
//所謂「依存性の注入」というやつです．「依存性の注入」については正直自分もよくわかっていないので，理解しなくても大丈夫だと思います．
//継承先のクラスではupdateとdrawをoverrideする必要があります．
//よくわからないなら，「クラスの継承」「関数のオーバーライド（オーバーロードとは別物です）」「完全仮想関数」あたりを調べてみてください．ぶっちゃけ，よくわからなくても大丈夫です．
//ひとまず，GraphicMainSampleを確認していただければなんとなくわかると思います．

class AbstractGraphicMain
{
public:
	//このクラスの継承先では，const GraphicDataBroker* _brokerを引数に持つコンストラクタを実装する必要があります．
	AbstractGraphicMain(const GraphicDataBroker* _broker);
	virtual ~AbstractGraphicMain() = default;

	//描画画面の更新を行う．完全仮想関数のため，継承先では必ずoverrideする必要がある．
	virtual bool update() = 0;

	//描画を行う．ここでは描画系の処理のみを行い内部のデータを更新しないためconstを付けている．完全仮想関数．
	virtual void draw() const = 0;

protected:

	//画像表示を行うこのクラスと，データ処理を行う外部のクラスを繋ぐクラスのポインタを受け取る．
	const GraphicDataBroker* mp_Broker;
};