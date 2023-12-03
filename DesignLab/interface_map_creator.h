//! @file interface_map_creator.h
//! @brief マップ生成クラスのインターフェース


#ifndef DESIGNLAB_INTERFACE_MAP_CREATOR_H_
#define DESIGNLAB_INTERFACE_MAP_CREATOR_H_

#include "map_state.h"


//! @class IMapCreator
//! @brief マップ生成クラスのインターフェース．
class IMapCreator
{
public:

	virtual ~IMapCreator() = default;

	//! @brief マップの初期化を行う．
	//! @details シミュレーションにおいてはこの機能のみあればよいが，
	//! @n 実機を動作させる場合，カメラが正確に認識できる距離の関係で，マップを読み直す必要がある．
	//! @n そのため，実機試験時はこちらの関数で初期化した後，UpdateMap()でマップを更新する必要がある．
	//! @return 初期化したマップ
	virtual MapState InitMap() = 0;

	//! @brief マップの更新を行う．
	//! @details 実機を動作させる場合に，マップを読み直す必要がある．
	//! @n シミュレーションではこの機能は不要．
	//! @param [in,out] current_map 現在のマップを受け取り，更新したマップを返す．
	virtual void UpdateMap(MapState* current_map) = 0;

};


#endif	// DESIGNLAB_INTERFACE_MAP_CREATOR_H_