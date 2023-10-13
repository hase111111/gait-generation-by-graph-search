//! @file define.h
//! @brief プロジェクト全体で使用する定数をまとめたもの

#ifndef DESIGNLAB_DEFINE_H_
#define DESIGNLAB_DEFINE_H_

//#define DESIGNLAB_DONOT_USE_DXLIB	//!< DXライブラリを使用しない場合はコメントアウトを外す

#include <string>


//! @class Define
//! @brief プロジェクト全体で使用する定数をまとめた定数クラス
//! @details Effective C++ という本 (私が持っているのはかなり古い版のものなので正直正しいのかはよくわからないけど) によると，
//! @n C++においてはあまり定数を宣言するために #defineを使用するべきではないようである．結構いろんなサイトでも同じことが言われている．
//! @n https://qiita.com/jonichonpa/items/595ed7914751787ebaee
//! @n https://myon.info/blog/2015/12/18/avoid-defining-macros/
//! @n
//! @n const statisな定数．inline 関数．constexpr定数を使うべき．
//! @n
//! @n 以下参考資料． 
//! @n クラスメンバに constexpr static 変数はおすすめしない ― 現象と対策
//! @n https://qiita.com/Nabetani/items/d8a3ebccaef03cd18d81
//! @attention GRAPH_SEARCH_DEPTHの値を大きくしすぎると動作しなくなる．
//! @n 先行研究では深さ4～5あたりで動作せていたのでそのあたりで，
//! @n ちなみに，深さを1つ深くしただけで，探索する範囲は膨大に増えるので注意が必要．深さ6以上を実時間内に探索し終わるのは，そこそこの性能のPCが必要．
class Define final
{
public:

	// コンストラクタを全て削除して，インスタンス化できないようにする
	Define() = delete;
	Define(const Define& _other) = delete;
	Define(const Define&& _other) = delete;
	Define& operator = (const Define& _other) = delete;


	const static int kSimurateNum;	//!< 連続でシミュレーションを行う回数

	const static int kGaitPatternGenerationLimit;	//!< 1シミュレーション当たりの最大歩容生成回数

	const static int kGoalTape;	//!< 直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了

	const static std::string kResultFilePath;	//!< シミュレーション結果を保存するファイルのパス
};


#endif // !DESIGNLAB_DEFINE_H_