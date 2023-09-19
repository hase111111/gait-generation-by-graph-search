//! @file graphic_main_sample.h
//! @brief IGraphicMainの使い方解説

#ifndef DESIGNLAB_GRAPHIC_MAIN_SAMPLE_H_
#define DESIGNLAB_GRAPHIC_MAIN_SAMPLE_H_

#include "interface_graphic_main.h"

#include <memory>

#include "application_setting_recorder.h"


//! @class GraphicMainSample
//! @brief サンプル描画クラス．サンプルメッセージを表示する．
//! @details このクラスは実際に使用する目的で作ったものではなく，インターフェイスを用いたクラスの実装法の解説のために作ったもの．
//! @n 最低限，このような要素があれば継承できる．
//! @n
//! @n [構文について] 
//! @n ごちゃごちゃいろいろついているが，
//! @n final … これ以上継承はできないという意味．このクラスを更に継承しようとするとエラーになる．ちなみに，継承しないクラスには基本的に付けておくとよい．
//! @n public IGraphicMain … IGraphicMainクラスを継承した，という意味．
//! @n override … 継承元の関数をオーバーライドしたという意味，付けなくとも問題ないが付けておくとコンパイラに明示的にオーバーライドを知らせられるのでバグを防ぎやすい．

class GraphicMainSample final : public IGraphicMain
{
public:
	GraphicMainSample(const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr);
	~GraphicMainSample() = default;

	bool Update() override;

	void Draw() const override;

private:

	int kBoxSizeX, kBoxSizeY;

	int counter_ = 0;
};


#endif // !DESIGNLAB_GRAPHIC_MAIN_SAMPLE_H_