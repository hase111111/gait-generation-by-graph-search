#pragma once
#include "InterfaceGraphicMain.h"

//ごちゃごちゃいろいろついていますが，
// final … これ以上継承はできないという意味．このクラスを更に継承しようとするとエラーになります．継承しないクラスには基本的に付けておくとよいです．
// public AbstractGraphicMain … AbstractGraphicMainクラスを継承した，という意味
// override … 継承元の関数をオーバーライドしたという意味，付けなくとも問題ないが付けておくとコンパイラに明示的にオーバーライドを知らせられるのでバグを防ぎやすい．

//サンプル描画クラス．サンプルメッセージを表示します．
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
