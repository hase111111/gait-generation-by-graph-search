//! @file map_renderer.h
//! @brief マップの描画を行うクラス．

#ifndef DESIGNLAB_MAP_RENDERER_H_
#define DESIGNLAB_MAP_RENDERER_H_

#include "map_state.h"


//! @class MapRenderer
//! @brief Mapの描画を行うクラス．
class MapRenderer final
{
public:
	MapRenderer();
	~MapRenderer() = default;

	//! @brief マップの描画を行う．
	//! @param [in] map マップの状態．
	void Draw(const MapState& map) const;

private:

	const unsigned int kColorGray;
	const unsigned int kColorLightGray;
	const float kCubeSize;
};


#endif //DESIGNLAB_MAP_RENDERER_H_