#pragma once

#include <map>
#include <vector>

#include "designlab_vector.h"
#include "map_creator.h"
#include "map_const.h"


//! @class MapState
//! @date 2023/08/06
//! @author 長谷川
//! @brief マップを表すクラス．
//! @details このプログラムでは，ロボットが歩くマップは脚設置可能点の集合で表現される．面ではなく点の集合．
//! @n 脚設置可能地点のデータはベクトルの配列で実装している．
//! @n また，処理を軽くするために，マップが存在する領域を長方形に切り分けて，その中に存在する脚設置可能点を集めたものであるdevide_mapがメンバ変数にある．
//! @n devide_mapの要素は https://atcoder.jp/contests/APG4b/tasks/APG4b_t の 「1次元の配列を多次元配列として使う」の要領で並んでいる．@n 
//! @n メンバ変数のデータへのアクセスは，メンバ関数のget????系の関数で行う．
//! @n 直接データのやり取りを行わないのは，生データであると値を変更可能になってしまうからである．constな関数を使えば，間違っても値の変更ができないので，データのやり取りに優れる．
class MapState final
{
public:

	MapState() = default;

	MapState& operator = (const MapState& other);

	//! @breif 地形データの初期化を行う．
	//! @param [in] mode 列挙体 EMapCreateModeでどのような地形を生成するか指定する．
	//! @param [in] option メンバ変数の OPTION_????で生成するマップのオプションを指定する．modeでReadFromFileが指定されているなら無視される．
	//! @n また，bit演算を利用して複数指定できる．例えば穴あきかつ，階段状にしたいならば，OPTION_PERFORATED | OPTION_STEP と指定する．
	//! @param [in] do_output 生成したマップをファイル出力するならtrue，しないならばfalse．する場合は生成した瞬間にファイル出力される．
	void init(const EMapCreateMode mode, const int option, const bool do_output);

	//! @brief グローバルのx座標の値から，m_devide_mapのどこをさしているか計算して返す．
	//! @n 範囲外の値を指定した場合は，0またはマップの端の座標を返す．
	//! @param [in] posx グローバルのy座標，グローバルはマップの原点を0とする座標系．座標系の向きはSvector構造体を参照．
	//! @return int m_devide_mapのx座標のどこをさしているか
	inline static int getDevideMapNumX(const float posx)
	{
		const int res = static_cast<int>((posx - MapConst::MAP_MIN_FORWARD) / (((float)MapConst::MAP_MAX_FORWARD - MapConst::MAP_MIN_FORWARD) / MapConst::LP_DIVIDE_NUM));
		if (res < 0)return 0;
		if (res >= MapConst::LP_DIVIDE_NUM)return MapConst::LP_DIVIDE_NUM - 1;
		return res;
	}

	//! @brief グローバルのy座標の値から，m_devide_mapのどこをさしているか計算して返す．
	//! @n 範囲外の値を指定した場合は，0またはマップの端の座標を返す．
	//! @param [in] posy グローバルのy座標，グローバルはマップの原点を0とする座標系．座標系の向きは構造体SVectorを参照．
	//! @return int m_devide_mapのy座標のどこをさしているか
	inline static int getDevideMapNumY(const float posy)
	{
		const int res = static_cast<int>((posy - MapConst::MAP_MIN_HORIZONTAL) / (((float)MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / MapConst::LP_DIVIDE_NUM));
		if (res < 0) { return 0; }
		if (res >= MapConst::LP_DIVIDE_NUM) { return MapConst::LP_DIVIDE_NUM - 1; }
		return res;
	}

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点の数を取得する．
	//! @n 範囲外の値を指定した場合は，0を返す．
	//! @param [in] x X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y Y座標，切り分けられたタイルの位置で指定する．
	//! @return int 脚設置可能点の数
	int getPointNumFromDevideMap(const int x, const int y) const;

	//! @brief 長方形状に切り分けられたマップから，脚設置可能点の実際の座標を取得する．
	//! @n 範囲外の値を指定した場合は，(0,0,0)を返す．
	//! @param [in] x x座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y y座標，切り分けられたタイルの位置で指定する．
	//! @param [in] num 何番目の脚設置可能点か． 
	//! @return SVector 脚設置可能点の座標．
	dl_vec::SVector getPosFromDevideMap(const int x, const int y, const int num) const;

	//! @brief 長方形状に切り分けられたマップから，最も高いZ座標を返す．
	//! @param [in] x X座標，切り分けられたタイルの位置で指定する．
	//! @param [in] y Y座標，切り分けられたタイルの位置で指定する．
	//! @return float 最も高いZ座標．
	inline float getTopZFromDevideMap(const int x, const int y) const
	{
		return m_devide_map_top_z.at(getDevideMapNum(x, y));
	};

	//! @brief 脚設置可能点の座標を出力する．長方形状に切り分けられたマップから値を取得するわけではないので，描画やデバッグのみに利用することを推奨．
	//! @param [in] num 
	//! @return SVector 脚設置可能点の座標．
	dl_vec::SVector getPos(const int num) const;

	//! @brief 脚設置可能点の座標の数を出力する．getPos関数と併用して使用する．
	//! @return int 脚設置可能点の総数
	inline int getPosNum() const
	{
		return static_cast<int>(m_map_data.size());
	}


private:

	const float kMapMinZ = -100000.0f;		//マップの最低のZ座標


	// m_map_data をもとに m_devide_mapに値をセットする
	void makeDevideMap();

	// m_devide_mapでは要素は1次元の配列として並んでいるので，2つの入力から，どの値を参照すればよいのかを計算する．
	inline int getDevideMapNum(const int x, const int y) const { return x * MapConst::LP_DIVIDE_NUM + y; }


	std::vector<dl_vec::SVector> m_map_data;					//ロボットが歩くマップ．脚設置可能点の集合で表現される．

	std::vector<std::vector<dl_vec::SVector> > m_devide_map;	//マップが存在する領域を正方形に切り分けて，その中に存在する脚設置可能点を集めたもの．

	std::vector<float> m_devide_map_top_z;						//m_devide_mapの中の最も高いz座標をまとめたもの，要素が存在しないなら，float型の最小値が入る．

};


//! @file map_state.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief マップのデータを管理するクラス．
//! @n 行数 : @lineinfo
