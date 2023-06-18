#pragma once
#include "InterfaceGraphicMain.h"


class GraphicMainSample final : public IGraphicMain
{
public:
	GraphicMainSample(const GraphicDataBroker* _broker) : IGraphicMain(_broker) {};
	~GraphicMainSample() = default;

	bool update() override;

	void draw() const override;

private:

	int m_counter = 0;
};


//! @file GraphicMainSample.h
//! @brief IGraphicMainの使い方解説
//! @author 長谷川

//! @class GraphicMainSample
//! @brief サンプル描画クラス．サンプルメッセージを表示する．
//! @details このクラスは実際に使用する目的で作ったものではなく，インターフェイスを用いたクラスの実装法の解説のために作ったもの．<br>
//! 最低限，このような要素があれば継承できる．<br>
//! [構文について] <br>
//! ごちゃごちゃいろいろついているが，<br>
//! final … これ以上継承はできないという意味．このクラスを更に継承しようとするとエラーになる．ちなみに，継承しないクラスには基本的に付けておくとよい．<br>
//! public AbstractGraphicMain … AbstractGraphicMainクラスを継承した，という意味．<br>
//! override … 継承元の関数をオーバーライドしたという意味，付けなくとも問題ないが付けておくとコンパイラに明示的にオーバーライドを知らせられるのでバグを防ぎやすい．<br>
//! GraphicMainSample(const GraphicDataBroker* _broker) : IGraphicMain(_broker) {}; … コンストラクタにおいて，親クラスのコンストラクタを呼び出す，という意味．これを書き忘れるとエラーになるので注意．
//! @author 長谷川
