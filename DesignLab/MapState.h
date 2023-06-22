#pragma once
#include <map>
#include "MapCreator.h"
#include "MapConst.h"


class MapState final
{
private:

	std::vector<my_vec::SVector> m_map_data;					//ロボットが歩くマップ．脚設置可能点の集合で表現される．

	std::vector<std::vector<my_vec::SVector> > m_devide_map;	//マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．

	std::vector<float> m_devide_map_top_z;						//m_devide_mapの中の最も高いz座標をまとめたもの，要素が存在しないなら，float型の最小値が入る．


	// m_map_data をもとに m_devide_mapに値をセットする
	void makeDevideMap();

	// m_devide_mapでは要素は1次元の配列として並んでいるので，2つの入力から，どの値を参照すればよいのかを計算する．
	inline int getDevideMapNum(const int _x, const int _y) const { return _x * MapConst::LP_DIVIDE_NUM + _y; }

public:
	MapState() = default;

	//! 地形データの初期化を行う．
	//! @param [in] _mode 列挙体 EMapCreateModeでどのような地形を生成するか指定する．
	//! @param [in] _option メンバ変数の OPTION_????で生成するマップのオプションを指定する．_modeでReadFromFileが指定されているなら無視される．
	//! また，bit演算を利用して複数指定できる．例えば穴あきかつ，階段状にしたいならば，OPTION_PERFORATED | OPTION_STEP と指定する．
	//! @param [in] _do_output 生成したマップをファイル出力するならtrue，しないならばfalse．する場合は生成した瞬間にファイル出力される．
	void init(const EMapCreateMode _mode, const int _option, const bool _do_output);

	//! 長方形状に切り分けられたマップから，脚設置可能点の数を取得する．
	//! @param [in] _x X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] _y Y座標，切り分けられたタイルの位置で指定する．
	//! @return int 脚設置可能点の数
	int getPointNumFromDevideMap(const int _x, const int _y) const;

	//! 長方形状に切り分けられたマップから，脚設置可能点の実際の座標を取得する．
	//! @param [in] _x X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] _y Y座標，切り分けられたタイルの位置で指定する．
	//! @param [in] _num 何番目の脚設置可能点か． 
	//! @return SVector 脚設置可能点の座標．
	my_vec::SVector getPosFromDevideMap(const int _x, const int _y, const int _num) const;

	//! 長方形状に切り分けられたマップから，最も高いZ座標を返す．
	//! @param [in] _x X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] _y Y座標，切り分けられたタイルの位置で指定する．
	//! @return float 最も高いZ座標．
	inline float getTopZFromDevideMap(const int _x, const int _y) const 
	{
		return m_devide_map_top_z.at(getDevideMapNum(_x, _y)); 
	};

	//! 脚設置可能点の座標を出力する．長方形状に切り分けられたマップから値を取得するわけではないので，描画にのみ利用することを推奨．
	//! @param [in] _num 
	//! @return SVector 脚設置可能点の座標．
	my_vec::SVector getPos(const int _num) const;

	//! 脚設置可能点の座標の数を出力する．getPos関数と併用して使用する．
	//! @return int 脚設置可能点の数
	inline int getPosNum() const 
	{ 
		return (int)m_map_data.size(); 
	}

	//! グローバルのx座標の値から，m_devide_mapのどこをさしているか計算して返す．
	//! @param [in] _posx グローバルのx座標，グローバルはマップの原点を0とする座標系．座標系の向きはSvector構造体を参照．
	//! @return int m_devide_mapのX座標のどこをさしているか
	inline static int getDevideMapNumY(const float _posx)
	{
		int tmp = (int)((_posx - MapConst::MAP_MIN_HORIZONTAL) / (((float)MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / MapConst::LP_DIVIDE_NUM));
		if (tmp < 0) { return 0; }
		if (tmp >= MapConst::LP_DIVIDE_NUM) { return MapConst::LP_DIVIDE_NUM - 1; }
		return tmp;
	}

	//! グローバルのy座標の値から，m_devide_mapのどこをさしているか計算して返す．
	//! @param [in] _posy グローバルのy座標，グローバルはマップの原点を0とする座標系．座標系の向きはSvector構造体を参照．
	//! @return int m_devide_mapのY座標のどこをさしているか
	inline static int getDevideMapNumX(const float _posy)
	{
		int tmp = (int)( (_posy - MapConst::MAP_MIN_FORWARD) / ( ((float)MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / MapConst::LP_DIVIDE_NUM) );
		if (tmp < 0)return 0;
		if (tmp >= MapConst::LP_DIVIDE_NUM)return MapConst::LP_DIVIDE_NUM - 1;
		return tmp;
	}
};


//! @file MapState.h
//! @brief マップを表すクラスの実装．
//! @author 長谷川

//! @class MapState
//! @brief マップを表すクラス．
//! @details このプログラムでは，ロボットが歩くマップは脚設置可能点の集合で表現される．面ではなく点の集合．<br>
//! 脚設置可能地点のデータはベクトルの配列で実装している．<br>
//! また，処理を軽くするために，マップが存在する領域を長方形に切り分けて，その中に存在する脚設置可能点を集めたものであるdevide_mapがメンバ変数にある．<br>
//! devide_mapの要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．<br>
//! <br>
//! メンバ変数のデータへのアクセスは，メンバ関数のget????系の関数で行う．<br>
//! 直接データのやり取りを行わないのは，生データであると値を変更可能になってしまうからである．constな関数を使えば，間違って値の変更をしてしまう危険性が0になるので，データのやり取りに優れる．
//! @author 長谷川
