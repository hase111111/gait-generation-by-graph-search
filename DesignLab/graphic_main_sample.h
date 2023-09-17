#pragma once

#include "abstract_graphic_main.h"


//! @class GraphicMainSample
//! @date 2023/08/09
//! @author 長谷川
//! @brief サンプル描画クラス．サンプルメッセージを表示する．
//! @details このクラスは実際に使用する目的で作ったものではなく，インターフェイスを用いたクラスの実装法の解説のために作ったもの．
//! @n 最低限，このような要素があれば継承できる．
//! @n [構文について] 
//! @n ごちゃごちゃいろいろついているが，
//! @n final … これ以上継承はできないという意味．このクラスを更に継承しようとするとエラーになる．ちなみに，継承しないクラスには基本的に付けておくとよい．
//! @n public AbstractGraphicMain … AbstractGraphicMainクラスを継承した，という意味．
//! @n override … 継承元の関数をオーバーライドしたという意味，付けなくとも問題ないが付けておくとコンパイラに明示的にオーバーライドを知らせられるのでバグを防ぎやすい．
//! @n GraphicMainSample(const GraphicDataBroker* broker) : AbstractGraphicMain(broker) {}; … コンストラクタにおいて，親クラスのコンストラクタを呼び出す，という意味．これを書き忘れるとエラーになるので注意．
class GraphicMainSample final : public AbstractGraphicMain
{
public:
	GraphicMainSample(const GraphicDataBroker* const  broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting)
		: AbstractGraphicMain(broker, calc, setting) {};
	~GraphicMainSample() = default;

	bool Update() override;

	void Draw() const override;

private:

	int m_counter = 0;
};


//! @file graphic_main_sample.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief IGraphicMainの使い方解説
//! @n 行数 : @lineinfo
